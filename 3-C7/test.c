/*********************************************************************************
 *      Copyright:  (C) 2019 Tang Zhiqiang<t_zhiqiang@136.com>
 *                  All rights reserved.
 *
 *       Filename:  test.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(10/11/2019)
 *         Author:  Tang Zhiqiang <t_zhiqiang@136.com>
 *      ChangeLog:  1, Release initial version on "10/11/2019 07:59:03 PM"
 *                 
 ********************************************************************************/


#include <stdio.h>
#define TIMES 10

int main(int argc, char *argv[])
{
    int     i;
    for (i = 0; i < TIMES; ++i) 
    {
        printf("hello world\n");
    }
    return 0;
}
