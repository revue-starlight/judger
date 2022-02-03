#pragma once
#include "utils/log.hpp"
#include "utils/args.hpp"
#include <unistd.h>
#include <filesystem>
class Config{
    uid_t uid;
    gid_t gid;
    size_t rcpu,rmem;
    public:
    explicit Config(const Arg &arg){
        uid = 0;
        gid = 0;
        rcpu = 0;
        rmem = 0;
        if (arg.getval("uid")!=""){
            uid = atoi(arg.getval("uid").c_str());
        }
        if (arg.getval("gid")!=""){
            gid = atoi(arg.getval("gid").c_str());
        }
        if (arg.getval("rcpu")!=""){
            rcpu = atoi(arg.getval("rcpu").c_str());
        }
        if (arg.getval("rmem")!=""){
            rmem = atoi(arg.getval("rmem").c_str());
        }
    }
    uid_t getUid() const {
        return this->uid;
    }
    uid_t getGid() const {
        return this->gid;
    }
    uid_t getCPU() const {
        return this->rcpu;
    }
    uid_t getMEM() const {
        return this->rmem;
    }
    virtual ~Config();
};