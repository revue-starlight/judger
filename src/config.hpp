#pragma once
#include "utils/log.hpp"
#include <unistd.h>
#include <filesystem>
class Config{
    uid_t uid;
    gid_t gid;
    size_t rcpu,rmem;
    unsigned long cloneFlag;
};