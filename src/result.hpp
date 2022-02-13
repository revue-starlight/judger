#pragma once
#include "config.hpp"
#include "cgroup.hpp"
#include "settings.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include <filesystem>
class Result{
  private:
  int ifOLE(Config *config){
    std::filesystem::space_info devi = std::filesystem::space("/root/repos/judger/output");
    if (devi.capacity-devi.free > config->getOutputLimitInByte()){
      return 1;
    }
    return 0;
  }
  public:
  // time in ms, mem in bytes
  size_t time,mem;
  uint returnValue;
  std::string exceed;
  /**
   * check whether resource usage exceed the control
   * */
  void checkValid(Config *config){
    if (returnValue != 0){
      exceed = "RE";
    }
    if (config->getTime() < time){
      exceed = "TLE";
    }
    if (config->getMEM() < mem){
      INFO("config limit:%d, used:%d",config->getMEM(),mem);
      exceed = "MLE";
    }
    if (ifOLE(config)){
      exceed = "OLE";
    }
  }

  void printResult(){
    printf("cpu usage %dms \n memory usage %d byte \n  error:%s\n",time,mem,exceed.c_str());
    printf("return value=%d\n",returnValue);
  }
  
};
