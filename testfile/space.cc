#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;
int main()
{
    fs::space_info devi = fs::space("/root/repos/judger/output");
 
    std::cout << ".        Capacity       Free      Available\n"
              << "/dev:   " << devi.capacity/1024/1024 << "   "
              << devi.free/1024/1024 << "   " << devi.available/1024/1024  << '\n';

}