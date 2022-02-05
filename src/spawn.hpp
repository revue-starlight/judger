#pragma once
#include <iostream>
#include <vector>
#include <filesystem>
#include <map>
#include <string>

class spawn{
    public:
    std::string new_root = "/root/repos/judger/runtime";
    const std::vector <std::string> vec = {"bin", 
      "boot",  "dev" , "etc" , "home" , "lib"  ,
      "lib64" , "media" , "mnt"  ,"mycpp" , 
      "old_root","opt" , "proc"  ,"root" ,"run",
      "sbin" , "srv" , "sys" , "tmp" , "usr" ,"var"};
    int execFn();
    int pivot_root();
};
