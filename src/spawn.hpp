#pragma once
#include <iostream>
#include <vector>
#include <filesystem>
#include <map>
namespace fs = std::filesystem;
namespace sp{

    class spawn{
        public:
        std::vector <fs::path> mntList = {
        {"/3/3"},{"f/s"}
        };
        int execFn();
        int mnt();
    };
}