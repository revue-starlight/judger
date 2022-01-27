#include <filesystem>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <iostream>
using namespace std;
int main(){
  int k = getpid();  
  string s;
  s = "/proc/"+to_string(k)+"/fd";
  std::filesystem::path p = s;
  for (const auto &entry: filesystem::directory_iterator(p)){
    std::cout<<entry.path();
    if ( 1 == entry.is_symlink()){
      const std::filesystem::path &p = filesystem::read_symlink(entry.path());
      cout<<"->"<<p.c_str()<<endl;
    }
  }
}
