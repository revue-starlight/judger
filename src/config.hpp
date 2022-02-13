#pragma once
#include "utils/log.hpp"
#include "utils/args.hpp"
#include <cmath>
#include <unistd.h>
#include <filesystem>
#include <stdlib.h>
#include <string>
#include <sys/resource.h>
class Config{
    std::string id;
    std::string outputName;
    uid_t uid_;
    gid_t gid_;
    size_t cpu_quota_us,time_limit_in_ms,mem_limit_in_byte; 
    size_t output_limit_in_byte;
    public:
    Config(const Arg &arg){
        id = std::to_string(random()%233*(random()%10)+(random()));
        outputName = id+".out";
        uid_ = 114;
        gid_ = 514;
        cpu_quota_us = 50000;
        mem_limit_in_byte = 1024*1024*1024;
        output_limit_in_byte = 1024*1024*4;
        time_limit_in_ms = 16000;
        if (arg.getval("uid")!=""){
            uid_ = atoi(arg.getval("uid").c_str());
        }
        if (arg.getval("gid")!=""){
            gid_ = atoi(arg.getval("gid").c_str());
        }
        if (arg.getval("cpu")!=""){
            cpu_quota_us = atoi(arg.getval("cpu").c_str());
        }
        if (arg.getval("mem")!=""){
            mem_limit_in_byte = atoi(arg.getval("mem").c_str());
        }
    }
    size_t getOutputLimitInByte() const {
        return output_limit_in_byte;
    }
    uid_t getUid() const {
        return this->uid_;
    }
    gid_t getGid() const {
        return this->gid_;
    }
    size_t getCPU() const {
        return this->cpu_quota_us;
    }
    // memory limitation in byte;
    size_t getMEM() const {
        return this->mem_limit_in_byte;
    }
    // time limitation in ms;
    size_t getTime() const {
        return this->time_limit_in_ms;
    }
    rlimit getTimeRlimit() const {
        rlimit CPURlimit;
        CPURlimit.rlim_cur = ceil(1.0 * time_limit_in_ms / 1000 * 2);
        CPURlimit.rlim_max = ceil(1.0 * time_limit_in_ms / 1000 * 3);
        return CPURlimit;
    }

    rlimit getMemRlimit() const {
        rlimit MemRlimit;
        MemRlimit.rlim_cur = ceil(1.5 * mem_limit_in_byte);
        MemRlimit.rlim_cur = ceil(2 * mem_limit_in_byte);
        return MemRlimit;
    }

    string getOutputPath() const {
        return "/output/"+this->outputName;
    }

};