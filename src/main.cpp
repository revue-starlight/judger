#include <iostream>
#include <filesystem>
#include "utils/log.hpp"
#include "cgroup.hpp"
using namespace std;
int cgroupConfig();
int cmdRun();
int cgroupClean();
cg::Cgroup cgroup;

int main(){
    cgroupConfig();
    cmdRun();
    sleep(10);
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
    int q;
    cin>>q;
    cgroup.bind(q,cg::CPU);
    cgroup.bind(q,cg::MEM);
    return 0;
}

int cgroupClean(){
    cgroup.getStatus();
    cgroup.clean();
    return 1;
}