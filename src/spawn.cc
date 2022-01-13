#include "spawn.hpp"
#include "utils/log.hpp"
#include <iostream>
#include <vector>
namespace sp{
    
    int spawn::execFn(){
        INFO("execFn");
        return 1;
    }

    int mnt(){
        INFO("mnt");
        return 1;
    }

}