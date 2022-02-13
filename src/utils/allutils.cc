#include <iostream>
#include <filesystem>
#include <string>
#include <string.h>
#include "allutils.hpp"
namespace fs = std::filesystem;
void printFD(const int pid){
  std::string s = "/proc/" + std::to_string(pid)+"/fd";
  fs::path p = s;
  if (!std::filesystem::is_directory(p)){
    printf("failed FD CHECK");
  }
  for (const auto &entry: fs::directory_iterator(p)){
    printf("%s->%s\n",entry.path().c_str(),fs::read_symlink(entry.path()).c_str());
  }
}

int nanoToMilli(std::string nano){
  if (nano.length()<=6){
    return 0;
  }
  for (int i=1;i<=6;i++){
    nano.pop_back();
  }
  return std::stoi(nano);
}
