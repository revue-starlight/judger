#include "../src/utils/allutils.hpp"
#include <sys/types.h>
#include <unistd.h>
int main(){
  printFD(getpid());
  return 0;
}
