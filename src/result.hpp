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
    if (config->getTime() >= time){
      exceed = "TLE";
    }
    if (config->getMEM() >= mem){
      exceed = "MLE";
    }
    if (returnValue != 0){
      exceed = "RE";
    }
    if (ifOLE(config)){
      exceed = "OLE";
    }
  }
  
};
