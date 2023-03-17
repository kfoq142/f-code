#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define EL printf("\n")
#define MOD 1000000007LL
#define INF 1e9
#define LINF 1e18
#define ld long double
#define ll long long
#define ull unsigned long long
#define ROR(x, y) ((unsigned)(x) >> (y) | (unsigned)(x) << (32 - (y)))

//Calculate the hash of the API used in the PEB code

int main() {
    //freopen("text.inp", "r", stdin);
    // freopen("text.out", "w", stdout);
    // char str[30];
    // gets(str);
    char str[] = "ReadFile";
    unsigned int hash = 0;
    for (int i = 0; i < strlen(str); ++i) {
        hash = ROR(hash, 0xd);
        hash += str[i];
    }
    printf("%xh", hash);
    return 0;
}