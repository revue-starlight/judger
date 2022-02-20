#include <seccomp.h>
#include <vector>
#include <string>
typedef std::vector <std::string> banlist_t;
class Seccomp{
    banlist_t syscallBanList;
    public:
    const banlist_t& getBanList() const {
        return syscallBanList;
    }
    bool indexOfBanList(std::string toCheck) const{
        int p = 0;
        for (const auto &item:syscallBanList){
            if (toCheck == item){
                return p;
            }
            p++;
        }
        return -1;
    }
    bool setBanList(banlist_t list) {
        this->syscallBanList = list;
    }

    int enable(const std::string &item) const {
        
    }
    int enableAll(){
        for (const auto &item:syscallBanList){

        }
    }
    

};