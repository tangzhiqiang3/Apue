/*********************************************************************************
 *      Copyright:  (C) 2019 Tang Zhiqiang<t_zhiqiang@163.com>
 *                  All rights reserved.
 *
 *       Filename:  get_temper.c
 *    Description:  This file is get RPI temperature in file.
 *                 
 *        Version:  1.0.0(11/09/2019)
 *         Author:  Tang Zhiqiang <t_zhiqiang@163.com>
 *      ChangeLog:  1, Release initial version on "11/09/2019 06:00:53 PM"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>


//#define filepath /sys/bus/w1/devices/28-041731f7c0ff/

int get_temper(float *temper);

int get_temper(float *temper)
{
    char 	    filepath[120]="/sys/bus/w1/devices/";
    char	    f_name[50];
    char            data_array[1024];
    char            *data_p=NULL;
    struct dirent   *file=NULL;
    DIR             *dir=NULL;
    int             data_fd;
    int             found = -1;
    //float           *temper;

    if((dir = opendir(filepath)) < 0)
    {
        printf("opendir file failure: %s\n",strerror(errno));
        return -1;
    }

    while((file = readdir(dir)) != NULL)
    {
        //if((strcmp(file->d_name, ".", 1) == 0) || (strcmp(file->d_name, ".", 1) == 0))
        //continue;               //ignore '.' and '..' file
        if(strstr(file->d_name, "28-")) 
        {   //memset(f_name, 0, sizeof(f_name));
            strncpy(f_name, file->d_name, sizeof(f_name));       //locate reserve temperature data file path
            found = 1;
            printf("reserve temperature data file path: %s\n",f_name);
        }
        //closedir(dir);
    }
    closedir(dir);
    /* found == 0; 未找到目的文件夹 */
    if(!found) 
    {
        printf("Can not find the folder\n");   
        return 0;
    }
    
    /* 找到相应文件夹后，切换至该文件夹下以获取温度数据 */
    strncat(filepath, f_name, sizeof(filepath)-strlen(filepath));       //将文件夹名连接到filepath路径后
    strncat(filepath, "/w1_slave", sizeof(filepath)-strlen(filepath));  //将设备文件夹下存放温度的文件连接到filepath路径后

    data_fd=open(f_name, O_RDONLY);
    if(data_fd < 0) 
    {
        printf("open file failure: %s\n", strerror(errno));
        return -2;
    }
    memset(data_array, 0, sizeof(data_array));
    if(read(data_fd, data_array, sizeof(data_array)) < 0)
    {
        printf("read file failure: %s\n", strerror(errno));
        return -3;
    }
    /* data_p指针后移两个字符单位，其后即为温度数据 */
    data_p=strstr(data_array, "t=");
    data_p=data_p+2;        //local temperature data 
    *temper=atof(data_p)/1000;    //"()" priority super "/"
    
    close(data_fd);

    return 0;
}

