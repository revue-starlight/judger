#include "spawn.hpp"
#include "utils/log.hpp"
#include "settings.hpp"
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/mman.h>
#include <iostream>
#include <string.h>
#include <vector>

int spawn::execFn(){
        INFO("execFn");
        return 1;
    }

int spawn::pivot_root(){
    const std::filesystem::path put_old = "oldrootfs";
    if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL) == -1)
        ERROR("mount-MS_PRIVATE");

    if (mount(new_root.c_str(), new_root.c_str(), NULL, MS_BIND, NULL) == -1)
        ERROR("mount-MS_BIND");



    INFO("mounting...");
    /* mount ordinary file */
    for (const auto &place:vec){
        std::filesystem::path path = (new_root+"/"+place).c_str();
        if (!std::filesystem::is_directory(path)){
            std::filesystem::create_directory(path);
        }
        mount(("/"+place).c_str(),(new_root+"/"+place).c_str(),
	    NULL,MS_BIND | MS_RDONLY,0);
    }
    std::filesystem::path path = new_root / put_old;
    if (mkdir(path.c_str(),0777) == -1){
        INFO(path.c_str());
        ERROR("mkdir");
    }
    /* mount output directory */
    mount((settings::rootpath + "/output").c_str(),(new_root + "/output").c_str(),
    NULL,MS_BIND | MS_RDONLY,0);

    if (syscall(SYS_pivot_root, new_root.c_str(), path.c_str()) == -1){
        ERROR("chdir");
    }

    if (chdir("/")==-1){
        ERROR("chdir");
    }


    if (umount2(put_old.c_str(), MNT_DETACH) == -1){
        ERROR("umount2 path:%s, ,error:%s",path.c_str(),strerror(errno));
    }

    if (rmdir(put_old.c_str()) == -1)
        perror("rmdir");

    return 1;
}