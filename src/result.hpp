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
    try{
      std::filesystem::space_info devi = std::filesystem::space("/root/repos/judger/output");
      if (devi.capacity-devi.free > config->getOutputLimitInByte()){
        return 1;
      }
      return 0;
    } catch (exception e){
      ERROR("%s",e.what());
    }
  }
  public:
  size_t time,mem;
  uint returnValue;
  std::string exceedInfo;
  /**
   * check whether resource usage exceed the control
   * */
  void checkValid(Config *config){
    if (returnValue != 0){
      if (returnValue == 25){
        exceedInfo = "OLE";
      }
      else exceedInfo = "RE";
    }
    if (config->getTime() < time){
      exceedInfo = "TLE";
    }
    if (config->getMEM() < mem){
      INFO("config limit:%d, used:%d",config->getMEM(),mem);
      exceedInfo = "MLE";
    }
  }

  void printResult(){
    printf("cpu usage %dms \n memory usage %d byte \n  error:%s\n",time,mem,exceedInfo.c_str());
    printf("return value=%d\n",returnValue);
  }
  
};
