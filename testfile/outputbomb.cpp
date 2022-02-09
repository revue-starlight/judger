#include <iostream>
#include <fstream>
#include <ios>
using namespace std;
int main(){
    fstream outfile;
    try{
        outfile.open("/root/repos/judger/output/2.out");
        //outfile.open("/root/repos/judger/CON");
        outfile<<"hello world"<<endl;
    } catch (exception &e){
        cout<<e.what()<<endl;
    }
    cout<<outfile.good()<<endl;
    outfile.close();
    return 0;
}