/*********************************************************************************
 *      Copyright:  (C) 2019 Tang Zhiqiang<t_zhiqiang@163.com>
 *                  All rights reserved.
 *
 *       Filename:  get_time.c
 *    Description:  This file is get current systrm time.
 *                 
 *        Version:  1.0.0(11/09/2019)
 *         Author:  Tang Zhiqiang <t_zhiqiang@163.com>
 *      ChangeLog:  1, Release initial version on "11/09/2019 05:59:44 PM"
 *                 
 ********************************************************************************/

#include <time.h>
#include <stdio.h>
//#define ip  TZQ

void get_time(char *data_time);

void get_time(char *data_time)
{
    struct tm   *p;
    time_t      timep;
    //char        datime[50];

    time(&timep);       // 获取当前的系统时间（CUT时间）              
    p=gmtime(&timep);   //gmtime()函数参数timep 所指的time_t 结构中的信息转换成真实世界所使用的时间日期表示方法，然后将结果由结构strcut tm*返回
    snprintf(data_time, 50, "%d-%d-%d  %d:%d:%d", (1900+p->tm_year),  (1+p->tm_mon), p->tm_mday, (p->tm_hour+8), p->tm_min, p->tm_sec);
    //小时+8是因为我们是+8区，比CUT多8个小时
    return ;
}

