#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
int main()
{
    try {
        fs::path p = "/root/repos/judger/output";
        std::cout << "File size = " << fs::file_size(p) << '\n';
    } catch(fs::filesystem_error& e) {
        std::cout << e.what() << '\n';
    }        
}