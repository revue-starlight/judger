#include <cstddef>
#include <cstring>
#include <iostream>
#include <algorithm>      
using namespace std;
const int N = 1000;
const int MAX = 1e8;
int a[N];
int main(){
  memset(a,0,sizeof(a));
  srand(time(NULL));
  for (int i=0;i<N;++i){
    a[i]=rand()%MAX;
  }
  sort(a,a+N);
  printf("bye\n");
  return 0;
}
