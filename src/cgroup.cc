#include <filesystem>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "cgroup.hpp"
#include "utils/log.hpp"
namespace cg{
    namespace fs = std::filesystem;
    const std::filesystem::path JUDGER_TMP = "judger_tmp";
    const std::filesystem::path CG_ROOT = "/sys/fs/cgroup";
    
    std::map <cg_type,std::string> type_name = {
        {CPU,"cpu"},
        {MEM,"memory"}
    };

    fs::path getPath(cg_type type){
        return CG_ROOT / type_name[type] / JUDGER_TMP;
    }

    int Cgroup::getmem(){
        return this->limits[MEM];
    }
    void Cgroup::setmem(int mem){
        this->limits[MEM] = mem;
    }
    int Cgroup::getcpu(){
        return this->limits[CPU];
    }
    void Cgroup::setcpu(int cpu){
        this->limits[CPU] = cpu;
    }

    int Cgroup::create(cg_type type){
        fs::path path = cg::CG_ROOT;
        std::string cat = type_name[type];
        path = path / cat;
        INFO("writing into cgroup (path: %s)",path.c_str());
        if (fs::is_directory(path)){
            path /= JUDGER_TMP;
            bool exist = fs::is_directory(path);
            if (exist) {
                WARNING("old path exist,maybe last time, the process failed somewhere?");
                // NOTE: due to the feature of LINUX, you can only remove the directory, while it is forbidden to remove the file in the directory.
                try{
                    fs::remove(path);
                } catch (fs::filesystem_error &fse){
                    ERROR("error happened when deleting old cgroup");
                    ERROR("%s",fse.what());
                    return -1;
                }
                SUCCESS("delete old path successfully");
            }
        } else {
            ERROR("path is not a formal cgroup directory (")
            return -1;
        }

        fs::create_directory(path);
        SUCCESS("%s created",path.c_str());        
        return 1;
    }

    int Cgroup::createAll(){
        for (auto [key,value]:type_name){
            if (create(key)==-1){
                return -1;
            }
        }
        return 1;
    }

    int Cgroup::write(cg_type type){
        
        std::fstream s;
        fs::path path = getPath(type);
        
        if (type == MEM){
            s.open(path / "memory.limit_in_bytes");
            s<<getmem();
            s.close();
            s.open(path / "memory.memsw.limit_in_bytes");
            s<<getmem();
            s.close();
        } else if (type == CPU) {
            s.open(path / "cpu.cfs_quota_us");
            s<<getcpu();
            s.close();
        }
        return 1;
    }
    
    int Cgroup::bind(pid_t pid,cg_type type){
        fs::path filename = getPath(type) / "tasks";
        std::fstream fs;
        fs.open(filename);
        if (!fs.is_open()){
            ERROR("unable to find %s",filename.c_str());
            return -1;
        }
        INFO("writing into cgroup tasks : pid == %d",pid);
        if (!(fs<<pid)){
            ERROR("write failed");
        }
        fs.close();
        SUCCESS("fs bind successfully,closed");
        return 1;
    }

    int Cgroup::clean(){
        for (const auto &[key,value]:type_name)
        {
            fs::path path = CG_ROOT / value / JUDGER_TMP;
            INFO("removing cgroup %s",path.c_str());
            try{
                    fs::remove(path.c_str());
                } catch (fs::filesystem_error &fse){
                    ERROR("error happened when removing cgroup");
                    ERROR("%s",fse.what());
                    return -1;
                }
                SUCCESS("remove successfully");
        }
        return 1;
    }

}