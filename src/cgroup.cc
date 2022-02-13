#include <filesystem>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "cgroup.hpp"
#include "result.hpp"
#include "utils/log.hpp"
#include "utils/allutils.hpp"
namespace cg
{
    namespace fs = std::filesystem;
    const std::filesystem::path JUDGER_DIR_NAME = "judger_tmp";
    const std::filesystem::path CG_PATH = "/sys/fs/cgroup";
    std::map<cg_type, std::string> RESOURCE_NAME = {
        {CPU, "cpu"},
        {MEM, "memory"}};

    /**
     * @brief Get the Path of the judger
     *
     * @param type
     * @return fs::path
     */
    fs::path getPath(cg_type type)
    {
        return CG_PATH / RESOURCE_NAME[type] / JUDGER_DIR_NAME;
    }

    int Cgroup::getMem()
    {
        return this->limits[MEM];
    }
    int Cgroup::getCPU()
    {
        return this->limits[CPU];
    }

    int Cgroup::create(cg_type type)
    {
        fs::path path = cg::CG_PATH;
        std::string cat = RESOURCE_NAME[type];
        path = path / cat;
        INFO("writing into cgroup (path: %s)", path.c_str());
        if (fs::is_directory(path))
        {
            path /= JUDGER_DIR_NAME;
            bool exist = fs::is_directory(path);
            if (exist)
            {
                WARNING("old path exist,maybe last time, the process failed somewhere?");
                // NOTE: due to the feature of LINUX, you can only remove the directory, while it is forbidden to remove the file in the directory.
                try
                {
                    fs::remove(path);
                }
                catch (fs::filesystem_error &fse)
                {
                    ERROR("error happened when deleting old cgroup");
                    ERROR("%s", fse.what());
                    return -1;
                }
                SUCCESS("delete old path successfully");
            }
        }
        else
        {
            ERROR("path is not a formal cgroup directory (")
            return -1;
        }

        fs::create_directory(path);
        SUCCESS("%s created", path.c_str());
        return 1;
    }
    /**
     * @brief create directory
     *
     * @return int
     */
    int Cgroup::createAll()
    {
        for (auto [key, value] : RESOURCE_NAME)
        {
            if (create(key) == -1)
            {
                return -1;
            }
        }
        return 1;
    }

    int Cgroup::write(cg_type type)
    {

        std::fstream s;
        fs::path path = getPath(type);
        try
        {
            if (type == MEM)
            {
                INFO("write memory %d bytes to cgroup", getMem());
                s.open(path / "memory.limit_in_bytes");
                s << getMem();
                s.close();
                s.open(path / "memory.memsw.limit_in_bytes");
                s << getMem();
                s.close();
            }
            else if (type == CPU)
            {
                INFO("write cpu %d /1000 \% to cgroup", getCPU());
                s.open(path / "cpu.cfs_quota_us");
                s << getCPU();
                s.close();
            }
        }
        catch (const std::fstream::failure &e)
        {
            ERROR("failed writing limits into cgroup %s", RESOURCE_NAME[type].c_str());
        }
        return 1;
    }

    int Cgroup::writeAll()
    {
        for (const auto [key, value] : RESOURCE_NAME)
        {
            if (write(key) == -1)
            {
                return -1;
            };
        }
        return 1;
    }

    int Cgroup::bind(pid_t pid, cg_type type)
    {
        fs::path filename = getPath(type) / "tasks";
        std::fstream fs;
        fs.open(filename);
        if (!fs.is_open())
        {
            ERROR("unable to find %s", filename.c_str());
            return -1;
        }
        INFO("writing into cgroup tasks : pid == %d", pid);

        {
            std::string ss = std::to_string(pid);
            fs.write(ss.c_str(), ss.length());
        }
        fs.close();
        SUCCESS("fs bind successfully,closed");
        return 1;
    }

    int Cgroup::getStatus(Result &result)
    {
        std::fstream s;
        s.exceptions(std::fstream::badbit);
        fs::path CPUpath = CG_PATH / RESOURCE_NAME[CPU] / JUDGER_DIR_NAME / "cpuacct.usage";
        try
        {
            s.open(CPUpath);
            if (!s.is_open())
            {
                ERROR("open failed");
            }
            std::string str;
            s >> str;
            
            result.time = nanoToMilli(str);
        }
        catch (std::fstream::failure &e)
        {
            ERROR("failed getting status");
            return -1;
        }
        fs::path memPath = CG_PATH / RESOURCE_NAME[MEM] / JUDGER_DIR_NAME / "memory.max_usage_in_bytes";
        try
        {
            s.open(memPath);
            if (!s.is_open())
            {
                ERROR("open failed");
            }
            std::string str;
            s >> str;
            result.mem = stoi(str);
        }
        catch (std::fstream::failure &e)
        {
            ERROR("failed getting status");
            return -1;
        }
        return 1;
    }
    int Cgroup::clean()
    {
        for (const auto &[key, value] : RESOURCE_NAME)
        {
            fs::path path = CG_PATH / value / JUDGER_DIR_NAME;
            INFO("removing cgroup %s", path.c_str());
            try
            {
                fs::remove(path.c_str());
            }
            catch (fs::filesystem_error &fse)
            {
                ERROR("error happened when removing cgroup");
                ERROR("%s", fse.what());
                return -1;
            }
            SUCCESS("remove successfully");
        }
        return 1;
    }
}
