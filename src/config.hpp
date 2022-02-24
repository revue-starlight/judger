#pragma once
#include "utils/log.hpp"
#include "utils/args.hpp"
#include <cmath>
#include <unistd.h>
#include <filesystem>
#include <stdlib.h>
#include <string>
#include <sys/resource.h>

enum class scmpWB_e {whitelist,blacklist};
struct ScmpConfig{
        std::vector <int> scmpList = {
            SCMP_SYS(read), SCMP_SYS(write), 
            SCMP_SYS(fstat), SCMP_SYS(mmap), 
            SCMP_SYS(mprotect), SCMP_SYS(munmap), 
            SCMP_SYS(brk), SCMP_SYS(access), 
            SCMP_SYS(exit_group)
        };
        scmpWB_e scmpWB=scmpWB_e::whitelist; // use Whitelist or Blacklist;
};

class Config{
    ScmpConfig scmpConfig;
    std::string id;
    std::string inputDirectoryToMount;
    std::string outputDirectoryToMount;
    std::string command;
    std::string outputName,inputName;
    uid_t uid_;
    gid_t gid_;
    size_t cpuQuotaUs,timeLimitInMs,MemLimitInByte,OutputLimitInByte;
    public:
    Config(const Arg &arg){
        srand(time(NULL));
        id = std::to_string(random()%233*(random()%10)+(random()));
        inputDirectoryToMount = "/root/repos/judger/output";
        outputDirectoryToMount = "/root/repos/judger/input";
        inputName = "1.in";
        outputName = id+".out";
        command = "./input/ordinary.out";
        uid_ = 114;
        gid_ = 514;
        cpuQuotaUs = 50000;
        MemLimitInByte = 1024*1024*1024;
        OutputLimitInByte = 1024*1024*4; // 4MB
        timeLimitInMs = 16000;
        if (arg.getval("uid")!=""){
            uid_ = atoi(arg.getval("uid").c_str());
        }
        if (arg.getval("gid")!=""){
            gid_ = atoi(arg.getval("gid").c_str());
        }
        if (arg.getval("cpu")!=""){
            cpuQuotaUs = atoi(arg.getval("cpu").c_str());
        }
        if (arg.getval("mem")!=""){
            MemLimitInByte = atoi(arg.getval("mem").c_str());
        }
        if (arg.getval("time")!=""){
            timeLimitInMs = atoi(arg.getval("time").c_str());
        }
        if (arg.getval("inDir")!=""){
            inputDirectoryToMount = atoi(arg.getval("inDir").c_str());
        }
        if (arg.getval("outDir")!=""){
            outputDirectoryToMount = atoi(arg.getval("outDir").c_str());
        }
        if (arg.getval("command")!=""){
            command = atoi(arg.getval("outDir").c_str());
        }   
    }
    size_t getOutputLimitInByte() const {
        return OutputLimitInByte;
    }
    uid_t getUid() const {
        return this->uid_;
    }
    gid_t getGid() const {
        return this->gid_;
    }
    size_t getCPU() const {
        return this->cpuQuotaUs;
    }
    // memory limitation in byte;
    size_t getMEM() const {
        return this->MemLimitInByte;
    }
    // time limitation in ms;
    size_t getTime() const {
        return this->timeLimitInMs;
    }
    rlimit getTimeRlimit() const {
        rlimit CPURlimit;
        CPURlimit.rlim_cur = ceil(1.0 * timeLimitInMs / 1000 * 2);
        CPURlimit.rlim_max = ceil(1.0 * timeLimitInMs / 1000 * 3);
        return CPURlimit;
    }

    rlimit getMemRlimit() const {
        rlimit MemRlimit;
        MemRlimit.rlim_cur = ceil(1.5 * MemLimitInByte);
        MemRlimit.rlim_cur = ceil(2 * MemLimitInByte);
        return MemRlimit;
    }

    ScmpConfig getScmpConfig() const {
        return scmpConfig;
    }

    std::string getInputDirectoryToMount() const {
        return this->inputDirectoryToMount;
    }
    std::string getOutputDirectoryToMount() const {
        return this->outputDirectoryToMount;
    }
    string getOutputName() const {
        return "/output/"+this->outputName;
    }
    string getInputName() const {
        return "/input/"+this->inputName;
    }
    string getId() const {
        return this->id;
    }
    string getCommand() const {
        return this->command;
    }
};