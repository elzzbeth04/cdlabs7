#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
char grammar[10][10];
int n;
char first[10][10];
char follow[10][10];
int isterminal(char c)
{
    return !(c>='A' && c<='Z');
}
void findfirst(char result[],char symbol)
{
    for(int i=0;i<n;i++)
    {
        if(grammar[i][0]==symbol)
        {
            char rhs=grammar[i][3];
            if(rhs==symbol)
            {
                continue;
            }
            if(isterminal(rhs))
            {
                if(strchr(result,rhs)==NULL)
                {
                    strncat(result,&rhs,1);
                }
            }
            else
            {
                findfirst(result,rhs);
            }
        }
    }
}
void findfollow(char result[],char symbol)
{
    if(symbol==grammar[0][0])
    {
        if(strchr(result,'$')==NULL)
        {
            strcat(result,"$");
        }
    }
    for(int i=0;i<n;i++)
    {
        for(int j=3;grammar[i][j]!='\0';j++)
        {
            if(grammar[i][j]==symbol)
            {
            char next=grammar[i][j+1];
            if(next!='\0' && isterminal(next))
            {
                if(strchr(result,next)==NULL)
                {
                    strncat(result,&next,1);
                }
            }
            else if(next!='\0' && !isterminal(next))
            {
                char temp[10]="";
                findfirst(temp,next);
                for(int k=0;temp[k]!='\0';k++)
                {
                    if(strchr(result,temp[k])==NULL)
                    {
                        strncat(result,&temp[k],1);
                    }
                }
            }
            else if(next=='\0' && grammar[i][0]!=symbol)
            {
                findfollow(result,grammar[i][0]);
            }
        }
    }
    }
}
int main()
{
    printf("enter no of rules:");
    scanf("%d",&n);
    printf("enter pdtions:");
    for(int i=0;i<n;i++)
    {
        scanf("%s",grammar[i]);
    }
    for(int i=0;i<n;i++)
    {
        char symbol=grammar[i][0];
        char temp[10]="";
        findfirst(temp,symbol);
        strcpy(first[i],temp);
    }
    for(int i=0;i<n;i++)
    {
        char symbol=grammar[i][0];
        char temp[10]="";
        findfollow(temp,symbol);
        strcpy(follow[i],temp);
    }
    for(int i=0;i<n;i++)
    {
        printf("first(%c)={%s}\n",grammar[i][0],first[i]);
    }
    for(int i=0;i<n;i++)
    {
        printf("follow(%c)={%s}\n",grammar[i][0],follow[i]);
    }
    return 0;
}