#include <iostream>
#include "args.hpp"
using std::cout;
using std::endl;
void Arg::printAll(){
  for (const auto iter: arg_){
    cout<<iter.first<<" "<<iter.second<<endl;
  } 
}
