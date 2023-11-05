
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include<unistd.h>
#include "project.h"



typedef long long int ll;
typedef double db;

#define f first
#define s second
#define all(v) (v).begin(),(v).end()
#define print(x) printf("%d \n", x);
#define printll(x) printf("%lld \n", x);
#define get(n) scanf("%d", &n);
#define N 10005
#define mod 1000000007	// 1e9 + 7



int main()
{
    //creatfile("root/a.txtaaaalfk");
    //return 0;
    //printf("%s\n", getcwd(CWD, 100));
    char q[N], ans[N] = "ok", inval[N] = "invalid command";
    scanf("%[^\n]", q);

    while(strcmp(q, "exit"))   
    {
       
        if(!query(q, ans))
        {
            for(int i = 0; i < strlen(inval); i++)
                ans[i] = inval[i];
        }
        
        printf("%s\n", ans);
        memset(ans, '\0', sizeof ans);
        ans[0] = 'o';
        ans[1] = 'k';
    
        char temp;
        scanf("%c",&temp);
        scanf("%[^\n]", q);
    }

    return 0;
}

