/*********************************************************************************
 *      Copyright:  (C) 2019 Tang Zhiqiang<t_zhiqiang@163.com>
 *                  All rights reserved.
 *
 *       Filename:  strtok.cpp
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(11/30/2019)
 *         Author:  Tang Zhiqiang <t_zhiqiang@163.com>
 *      ChangeLog:  1, Release initial version on "11/30/2019 04:57:21 PM"
 *                 
 ********************************************************************************/

#include<stdio.h>
#include<string.h>
#include<iostream>
using namespace std;

int main()
{
    void convertxx(char *str,char *dates,char *times);
    char dates[25];
    char times[25];
    memset(dates,0,sizeof(dates));
    memset(times,0,sizeof(times));
   
    char str[25]="2012-07-25 02:34:23";
    convertxx(str,dates,times);
    
    return 0;
}

void convertxx(char *str,char *dates,char *times)
{
    char *p=str;
    while(*p)
    {
        if(*p=='-'||*p==':')
        {
            *p=' ';
            p++;
        }
        p++;
    }
    p=str;   //str change 2012 07 25 02 34 23
    char *token;
    token=strtok(p," ");
    int i=0;

    while(token)
    {
        if(i<3)
        {
            strcat(dates,token);
            printf("%s\n",dates);
            i++;
        }
        if(i==3)
        {
            token=strtok(NULL," ");
            i++;
        }
        if(i>3)
        {
            strcat(times,token);
            printf("%s\n",times);
            i++;
        }
        token=strtok(NULL," ");
    }
}

