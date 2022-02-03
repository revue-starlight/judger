#pragma once
#include <string>
#include <map>
using std::string;
using std::map;
class Arg{
    int argc_;
    char **argv_;
    map <string,string> arg_;
    public:
    Arg(int argc,char **argv):argc_(argc),argv_(argv){
      string opt = "";
      for (int i=0;i<argc;i++){
        if (argv[i][0]!='-' && opt.empty() == false){
          arg_[opt] = argv[i];
        }
        else if (argv_[i][0]=='-'){
          int st = 1;
          if (argv_[i][st]=='-') st++;
          opt = (argv[i]+st);
        }
      }
    }
    virtual ~Arg(){}; 
    string getval(string opt) const {
      if (arg_.find(opt)!=arg_.end()){
        return arg_[opt];
      } else return "";
    }
    void printAll() const;
};
