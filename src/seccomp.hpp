#include <seccomp.h>
#include <vector>
#include "config.hpp"
#include <string>
typedef std::vector <std::string> banlist_t;
class Seccomp{
    
    banlist_t syscallBanList;
    public:
    const banlist_t& getBanList() const {
        return syscallBanList;
    }
    bool indexOfBanList(std::string toCheck) const{
        int p = 0;
        for (const auto &item:syscallBanList){
            if (toCheck == item){
                return p;
            }
            p++;
        }
        return -1;
    }
    bool setBanList(banlist_t list) {
        this->syscallBanList = list;
    }

    int enable(const std::string &item) const {
        
    }

    scmp_filter_ctx generateCtx(const ScmpConfig &config) const{
        scmp_filter_ctx ctx;
        if (config.scmpWB==scmpWB_e::blacklist) ctx = seccomp_init(SCMP_ACT_KILL);
        if (config.scmpWB==scmpWB_e::whitelist) ctx = seccomp_init(SCMP_ACT_ALLOW);
        for (auto rule:config.scmpList)
            if (config.scmpWB == scmpWB_e::blacklist){
                if (seccomp_rule_add(ctx,SCMP_ACT_KILL,rule,0)){
                    exit(4);
                }
            }
            else {
                if (seccomp_rule_add(ctx,SCMP_ACT_ALLOW,rule,0)){
                    exit(4);
                }
            }
        return ctx;
    }
    
    int loadCtx(scmp_filter_ctx ctx){
        if (seccomp_load(ctx)){
            exit(5);
        }
    }
    
};