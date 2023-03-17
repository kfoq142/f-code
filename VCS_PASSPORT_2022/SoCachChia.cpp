#include <bits/stdc++.h>
using namespace std;
#define faster ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)

int main(){
    faster;
    long long n;
    cin >> n;
    long long a[100+5];
    long long sum=0;
    for (long long i = 0; i < n; i++){
        cin >> a[i];
        sum += a[i];
    }
    long long cnt[100] = {};       
    if (sum % 3 != 0)
        cout << 0 << endl;
    else{
        sum /= 3;
        long long spara = 0;
        for (long long i = n-1; i >= 0; i--){
            spara += a[i];
            if (spara == sum)
                cnt[i] = 1;
        }
        for (long long i = n-2 ; i >= 0; i--)
            cnt[i] += cnt[i+1];
        long long count = 0;
        spara = 0;
        for (long long i = 0 ; i < n-2; i++){
            spara += a[i];
            if (spara == sum)
                count += cnt[i+2];
        }
        cout << count << endl;
    }
    return 0;
}