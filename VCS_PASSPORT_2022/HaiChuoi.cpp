#include <bits/stdc++.h>
using namespace std;
#define faster ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
const int maxx = 1e5 + 10;

int main(){
    faster;
    string a, b;
    cin >> a >> b;
    a = "$" + a + "$";
    b = "$" + b + "$";
    int lt[maxx]={}, rt[maxx]={};
    int n = a.size();
    int m = b.size();
    int j = 0;
    for (int i = 0; i < n; i++){
        if (j < m && a[i] == b[j])
            j++;
        lt[i] = j;
    }
    j = 0;
    for (int i = n - 1; i >= 0; i--){
        if (j < m && a[i] == b[m - j - 1])
            j++;
        rt[i] = j;
    }
    int l = 0, r = 0;
    for (int i = 0; i < n; i++){
        if (l + r < lt[i] + rt[i + 1]){
            l = lt[i];
            r = rt[i + 1];
        }
    }
    for (int i = 1; i < m - 1; i++){
        if (i < l || m - i - 1 < r)
            cout << b[i];
    }
    if (l + r <= 2) cout << '-';
    cout << endl;
    return 0;
}