#pragma once
#include "utils/log.hpp"
#include "utils/args.hpp"
#include <unistd.h>
#include <filesystem>
#include <stdlib.h>
class Config{
    uid_t uid;
    gid_t gid;
    size_t rcpu,rmem;
    public:
    Config(const Arg &arg){
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
        if (arg.getval("cpu")!=""){
            rcpu = atoi(arg.getval("cpu").c_str());
        }
        if (arg.getval("mem")!=""){
            rmem = atoi(arg.getval("mem").c_str());
        }
    }
    uid_t getUid() const {
        return this->uid;
    }
    gid_t getGid() const {
        return this->gid;
    }
    size_t getCPU() const {
        return this->rcpu;
    }
    size_t getMEM() const {
        return this->rmem;
    }
};