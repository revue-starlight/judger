#pragma once
#include "utils/log.hpp"
#include "utils/args.hpp"
#include <cmath>
#include <unistd.h>
#include <filesystem>
#include <stdlib.h>
#include <sys/resource.h>
class Config{
    uid_t uid_;
    gid_t gid_;
    size_t rcpu_,rmem_;
    size_t time_,mem_;
    public:
    Config(const Arg &arg){
        uid_ = 114;
        gid_ = 514;
        rcpu_ = 50000;
        rmem_ = 1024*512;
        time_ = 1000;
        if (arg.getval("uid")!=""){
            uid_ = atoi(arg.getval("uid").c_str());
        }
        if (arg.getval("gid")!=""){
            gid_ = atoi(arg.getval("gid").c_str());
        }
        if (arg.getval("cpu")!=""){
            rcpu_ = atoi(arg.getval("cpu").c_str());
        }
        if (arg.getval("mem")!=""){
            rmem_ = atoi(arg.getval("mem").c_str());
        }
    }
    uid_t getUid() const {
        return this->uid_;
    }
    gid_t getGid() const {
        return this->gid_;
    }
    size_t getCPU() const {
        return this->rcpu_;
    }
    size_t getMEM() const {
        return this->rmem_;
    }
    rlimit getCPURlimit() const {
        rlimit CPURlimit;
        CPURlimit.rlim_cur = ceil(1.0 * time_ / 1000 * 2);
        CPURlimit.rlim_max = ceil(1.0 * time_ / 1000 * 3);
        return CPURlimit;
    }

    rlimit getMemRlimit() const {
        rlimit MemRlimit;
        MemRlimit.rlim_cur = ceil(1.5 * rmem_);
        MemRlimit.rlim_cur = ceil(2 * rmem_);
        return MemRlimit;
    }

};