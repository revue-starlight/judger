#include <iostream>
#include <sstream>
#include <unistd.h>
#include <iomanip>
#include <cstring>
#include <chrono>
#include <algorithm>
using namespace std;

string str = "100"; // a variable of string data type
int num;
int a[3000001];
int main() {
    cout<<"pid="<<getpid()<<endl;
    int n;
    cout<<"start?"<<endl;
    cin>>n;
    chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
    memset(a,0,sizeof(a));
    srand(time(NULL));
    for (int i=0;i<=3000000;i++){
        a[i] = rand()%114514;
    }
    sort(a+1,a+3000000);
    chrono::time_point<std::chrono::system_clock> t2 = std::chrono::system_clock::now();
    unsigned long long p = (t2-t1).count();
    cout<<p<<endl;
    return 0;
}