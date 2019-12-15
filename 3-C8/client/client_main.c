/*********************************************************************************
 *      Copyright:  (C) 2019 Tang Zhiqiang<t_zhiqiang@163.com>
 *                  All rights reserved.
 *
 *       Filename:  socket_client_RPI.c
 *    Description:  This file socket client monitor RPI temperature.
 *                 
 *        Version:  1.0.0(10/22/2019)
 *         Author:  Tang Zhiqiang <t_zhiqiang@163.com>
 *      ChangeLog:  1, Release initial version on "10/22/2019 04:32:59 PM"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <getopt.h>
#include <syslog.h>
#include <signal.h>
#include "header.h"

#define     id  "t_zhiqang"  

int     g_sigstop = 0;

//void print_usage(char *progname);

void signal_stop(int signum)
{
    if(SIGTERM == signum)
    {
        printf("SIGTERM signal detected\n");
        g_sigstop = 1;
    }
}

int main(int argc, char *argv[])
{
    int                 sock_fd = -1;
    int                 rv = -1;
    struct sockaddr_in  serv_addr;
    char                *serv_ip = NULL;
    int                 serv_port = 0;
    char                buf[1024];
    int                 ch;
    char                da_time[50];                
    int                 interv_time = 0;   
    float               *temper;

    static struct option long_options[] = {
            {"ipaddr", required_argument, NULL, 'i'},
            {"port", required_argument, NULL, 'p'},
            {"help", no_argument, NULL, 'h'},
            {"interv_time", required_argument, NULL, 't'},
            {0, 0, 0, 0}
        };

        while((ch=getopt_long(argc, argv, "i:p:t:h", long_options, NULL)) != -1)
        {
            switch (ch) 
            {
                case 'i': 
                    serv_ip=optarg;
                    break;
                case 'p': 
                    serv_port=atoi(optarg); //optarg返回的是字符串，atoi()将char转换为int型
                    break;
                case 'h': 
                    print_usage(argv[0]);
                    break;
                case 't':
                    interv_time=atoi(optarg);
                    break;
                default:
                    print_usage(argv[0]);
                    exit(0);
                    break;
            }
            
        }
        if(!serv_ip || !serv_port || !interv_time)
        {
            print_usage(argv[0]);
            return -1;
        }

        //openlog("socket_client_RPI", LOG_CONS | LOG_PID, 0);    //可选
        //printf("Program %s running failure: %s\n",argv[0], strerror(errno)); //将错误或调试信息写入系统自带的syslog日志
        
        signal(SIGTERM, signal_stop);   //安装signal信号

        /*if(daemon(0, 0) < 0)   //后台运行该进程
        {      
            printf("Program deamon() fialure: %s\n", strerror(errno));
            return -2;
        }
        */
        while (!g_sigstop) 
        {   
            /*采样SN、时间、温度...*/  
            get_time(da_time);
            get_temper(temper);
            
            memset(buf, 0, sizeof(buf));
            snprintf(buf, sizeof(buf), "%s/%s/%.1f%c", id, da_time, temper, 'C');

            //sock_fd=client_init(serv_port, serv_ip);    //client initial 
            if(sock_fd < 0)
            {
                if((sock_fd=client_init(serv_port, serv_ip)) < 0)    //client initial 
                {
                    printf("write data to server fialure: %s\n", strerror(errno));
                    continue;   //连接服务端失败，重新连接
                }
            }
            /* connect server OK！*/
            if(sock_fd >= 0)
            {
                if(write(sock_fd, buf, sizeof(buf)) < 0)
                {
                    printf("write data to server fialure: %s\n", strerror(errno));
                    close(sock_fd);
                    sock_fd = -1;
                    return -3;
                }
            }
                
            sleep(interv_time); //命令行参数设定上传数据间隔时间   
        }

        //closelog();
        close(sock_fd);


        return 0;
}


void print_usage(char *progname)
{
    printf("%s usage.\n", progname);     
    printf("%s is a socket client progname, which used to verify server and echo back string from it\n", progname);
    printf("\nMandatory arguments to long options are mandatory for short option too:\n");
    //printf("-b[darmon] set program running on backgroud\n");
    printf("-t[interv_time] RPI temperature interval time\n");
    printf("-p[port] Socket client port address\n");
    printf("-i[ip] Socket client ip address\n");
    printf("-h[help] Display this help information\n");
    printf("\nExample: %s -t 30 -p 8088 -i 192.168.174.5\n", progname);
    return;
}


