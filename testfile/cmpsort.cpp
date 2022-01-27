#include <cstddef>
#include <cstring>
#include <iostream>
using namespace std;
int main(){
  int a[20];
  memset(a,0,sizeof(a));
  srand(time(NULL));
  for (int i=0;i<20;++i){
    a[i]=rand()%100;
  }
  for (int i=0;i<20;i++) cout<<a[i]<<" ";cout<<endl;
  for (int i=0;i<20;i++){
    for (int j=0;j<20-i-1;j++){
      if (a[j]>a[j+1]) swap(a[j],a[j+1]);
    }
  }
  for (int i=0;i<20;i++) cout<<a[i]<<" ";cout<<endl;
  return 0;
}
