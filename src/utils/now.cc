#include <chrono>
#include "now.hpp"
namespace NOW {
    std::chrono::time_point<std::chrono::system_clock> now(){
        return std::chrono::system_clock::now();
    }
}