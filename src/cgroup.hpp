#pragma once
#include <filesystem>
#include <map>
#include "utils/args.hpp"
#include "utils/log.hpp"
namespace cg{
    extern const std::filesystem::path JUDGER_TMP;
    extern const std::filesystem::path CG_ROOT;
    enum cg_type{
        CPU,
        MEM,
    };

    
    extern std::map <cg_type,std::string> type_name;
    
    extern std::filesystem::path getPath(cg_type type);
    
    /**
     * @brief set limitation to cpu resource and memory resource.
     * this class is generated for storing configurations and provide functions to configure. <- 什么塑料英语
     */
    class Cgroup{
        

        private:
            std::map <cg_type,unsigned int> limits;
            
        public:
            explicit Cgroup(const Config &config){
               limits[CPU]=config.getCPU();
               limits[MEM]=config.getMEM();
            }
            int getmem();
            int getcpu();
            void setcpu(int cpu);
            void setmem(int mem);
            int create(cg_type type);
            int createAll();
            int write(cg_type type);
            int writeAll();
            int bind(pid_t pid,cg_type type);
            int getStatus();
            int clean();
    };
   
}