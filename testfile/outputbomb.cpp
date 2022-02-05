#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include "../src/utils/allutils.hpp"
using namespace std;
int main(){
    printFD(getpid());
    int pid = getpid();
    std::string s = "/proc/" + std::to_string(pid)+"/fd";

}