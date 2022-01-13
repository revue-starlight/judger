#include <iostream>
#include <filesystem>
#include "utils/log.hpp"
#include "cgroup.hpp"
#include "spawn.hpp"
using namespace std;
int cgroupConfig();
int cmdRun();
int cgroupClean();
cg::Cgroup cgroup;

int main(){
    cgroupConfig();
    cmdRun();
    cgroupClean();
    return 0;
}

int cgroupConfig(){
    int q = 1;
    {// get Config (DEV)
        cgroup.setcpu(50000);
        cgroup.setmem(1024*1024*1024);
    }
    if (cgroup.createAll()==-1) return -1;
    if (cgroup.writeAll()==-1) return -1;
    return 1;
}

int cmdRun(){
    INFO("cmdRun");
    sp::spawn ss;
    string s = ss.mntList[0];
    INFO("%s",s.c_str());
    return 0;
}

int cgroupClean(){
    cgroup.getStatus();
    cgroup.clean();
    return 1;
}