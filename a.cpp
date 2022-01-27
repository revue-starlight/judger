#include <algorithm>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
const int N = 201;
int a[N],b[N];
void solve(){
  int n;
  cin>>n;
  for (int i=1;i<=n;i++) cin>>a[i];
  for (int i=1;i<=n;i++) cin>>b[i];
  for (int i=1;i<=n;i++){
    if (a[i]<b[i]) swap(a[i],b[i]);
  }
  int aa = -1;
  int bb = -1;
  for (int i=1;i<=n;i++){ aa = max(a[i],aa); bb = max(b[i],bb); }
  cout<<aa*bb<<endl;
}
int main(){
  ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);
  int T;
  cin>>T;
  while (T--) solve();
  
}
