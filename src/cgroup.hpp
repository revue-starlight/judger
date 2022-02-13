#pragma once
#include <filesystem>
#include <map>
#include "utils/args.hpp"
#include "utils/log.hpp"
#include "result.hpp"
#include "config.hpp"
namespace cg{
    extern const std::filesystem::path JUDGER_DIR_NAME;
    extern const std::filesystem::path CG_PATH;
    extern const std::filesystem::path OUTPUT_PATH;
    enum cg_type{
        CPU,
        MEM,
    };

    
    extern std::map <cg_type,std::string> RESOURCE_NAME;
    
    /**
     * @brief return the cgroup path of specified item
     * 
     * @param type 
     * @return std::filesystem::path 
     */
    extern std::filesystem::path getPath(cg_type type);
    
    /**
     * @brief do resource controls works
     */
    class Cgroup{
        
        private:
            /**
             * @brief resource limitations to fill into the cgroup. With looser strictions in order to detect outbounds.
             * 
             */
            std::map <cg_type,unsigned int> limits;
            
        public:
            explicit Cgroup(const Config &config){
               limits[CPU]=config.getCPU();
               limits[MEM]=config.getMEM();
            }
            int getMem();
            int getCPU();
            /**
             * @brief create cgroup directory
             * 
             * @param type 
             * @return int 
             */
            int create(cg_type type);
            /**
             * @brief create all cgroup directories
             * 
             * @param type 
             * @return int 
             */
            int createAll();
            /**
             * @brief write limitations into cgroup
             * 
             * @param type cgroup type (memory/cpu)
             * @return int success = 1 
             */
            int write(cg_type type);
            /**
             * @brief write all limitations into cgroup
             */
            int writeAll();
            /**
             * @brief fill pid into cgroup/<type>/tasks
             * @param pid process id 
             * @param type 
             * @return int 
             */
            int bind(pid_t pid,cg_type type);
            int getStatus(Result &result);
            int clean();
    };
   
}
