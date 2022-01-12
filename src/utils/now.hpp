#pragma once
#include <chrono>
namespace NOW{
    std::chrono::time_point<std::chrono::system_clock> now();
}