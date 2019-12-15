/*********************************************************************************
 *      Copyright:  (C) 2019 Tang Zhiqiang<t_zhiqiang@163.com>
 *                  All rights reserved.
 *
 *       Filename:  socket_server_RPI.c
 *    Description:  This file is socket server RPI temperature
 *                 
 *        Version:  1.0.0(11/04/2019)
 *         Author:  Tang Zhiqiang <t_zhiqiang@163.com>
 *      ChangeLog:  1, Release initial version on "11/04/2019 08:15:46 PM"
 *                 
 ********************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <getopt.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h> 
#include <sys/resource.h>
#include <signal.h>
#include <syslog.h>
#include "server_init.c"

#define MAX_EVENT      512
#define ARRAY_SIZE(x)   (sizeof(x)/sizeof(x[0]))    //计算结构体数组大小

static inline void print_usage(char *programe);
//void set_socket_rlimit(void);
//int socket_server_init(char *listenip, int listen_port);

int     g_sigstop = 0;

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
    int     listenfd,connfd;
    int     serv_port = 0;
    int     daemon_run = 0;
    char    *progname = NULL;
    int     opt;
    int     rv;
    int     i;
    char    buf[1024];

    int     epollfd;
    struct  epoll_event event;
    struct  epoll_event event_array[MAX_EVENT];
    int     events;

    struct option long_options[]=
    {
        {"deamon", no_argument, NULL, 'b'},
        {"port", required_argument, NULL, 'p'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    progname = basename(argv[0]);

    //Parser the command line parameters 
    while((opt = getopt_long(argc, argv, "bp:h", long_options, NULL)) != -1)
    {
        switch (opt) 
        {
            case 'b':
                daemon_run = 1;
                break;
            case 'p':
                serv_port = atoi(optarg);
                break;
            case 'h':
                print_usage(progname);
                break;
            default:
                break;
        }
    }
    if(!serv_port) 
    {
        print_usage(progname);
        return -1;
    }
    
    if((listenfd=socket_server_init(NULL, serv_port)) < 0) 
    {
        printf("ERROR: %s server listen on port %d failure\n", argv[0], serv_port);
        return -2;
    }
    
    printf("server socket[%d] start to listen on port %d\n", listenfd, serv_port);
    signal(SIGTERM, signal_stop);

    //if(daemon_run)
    //{
    //    daemon(0, 0);
    //}

    /*  将加入 listenfd 到兴趣数组中 */
    if(epollfd=epoll_create(MAX_EVENT) < 0)
    {
        printf("%s create epoll farilure: %s\n", argv[0], strerror(errno));
        return -3;
    }
    /* 必须将文件描述符赋值给event.data.fd */
    event.events = EPOLLIN;
    event.data.fd = listenfd;
    
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event) < 0 )
    {
        printf("epoll add listen socket[%d] failure: %s\n", listenfd, strerror(errno));
        return -4;
    }
    
    while(!g_sigstop)
    {   
        //program will blocked here
        printf("programe will blocked here...\n");
        events=epoll_wait(epollfd, event_array, MAX_EVENT, -1);
        if(events < 0)
        {
            printf("epoll failure: %s\n", strerror(errno));break;
        }
        else if(events==0) 
        {
            printf("epoll timeout\n");
            continue;
        }
        //rv>0 is the active events count
        for(i = 0; i < events; i++)
        {   
            //for循环的events与结构体成员中的events不同
            //按位与求的是结果，逻辑与求的是真或假
            if((event_array[i].events&EPOLLERR) || (event_array[i].events&EPOLLHUP)) 
            {
                printf("epoll_wait get error on fd[%d]: %s\n",event_array[i].data.fd, strerror(errno));
                epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
                close(event_array[i].data.fd);
            }
            //listen socket get event means new client start connect now
            if(event_array[i].data.fd == listenfd) //已将listenfd加入到epoll感兴趣事件集合
            {
                if((connfd=accept(listenfd, (struct sockaddr *)NULL, NULL)) < 0) //不保存客户端信息（结构体指针为NULL）
                {
                    printf("accept new client failure: %s\n", strerror(errno));      
                    continue;
                }
                event.data.fd = connfd; 
                event.events = EPOLLIN;
                if(epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &event) < 0 )
                {
                   printf("epoll add client socket failure: %s\n", strerror(errno));
                   close(event_array[i].data.fd);
                    continue;
                }
                printf("accept new client[%d] OK\n", connfd);
                //read(event_array[i].data.fd, buf, sizeof(buf); 这个语句还能通过event_array[i].data.fd与客户端连接
                //read(connfd, buf, sizeof(buf);//读到数据之后需要处理并保存到数据库
                memset(buf, 0, sizeof(buf));
                if((rv=read(event_array[i].data.fd, buf, sizeof(buf))) <= 0 )  //同上
                {
                    printf("socket[%d] read data failure or disconnect and will be remove.\n", event_array[i].data.fd);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
                    close(event_array[i].data.fd);
                    continue;
                }
                printf("socket[%d] read data: %s\n", event_array[i].data.fd, buf);
            }
            else //data arriver from alrady connected client
            {
                if((rv=read(event_array[i].data.fd, buf, sizeof(buf))) <= 0 )  //同上
                {
                     printf("socket[%d] read data failure or disconnect and will be remove.\n", event_array[i].data.fd);
                     epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
                     close(event_array[i].data.fd);
                     continue;
                }
                else 
                {
                    //读到数据之后需要处理并保存到数据库
                    printf("socket[%d] read data: %s\n", event_array[i].data.fd, buf);
                }
            }
        }//events 
    }//while(g_sigstop)    
    close(listenfd);
    return 0;
}


static inline void print_usage(char *progname)
{
    printf("Usage: %s [OPTION]...\n", progname);
    printf(" %s is a socket server program, which used to verify client and echo back string from it\n",progname);
    printf("\nMandatory arguments to long options are mandatory for short options too:\n");
    printf(" -b[daemon ] set program running on background\n");
    printf(" -p[port ] Socket server port address\n");
    printf(" -h[help ] Display this help information\n");
    printf("\nExample: %s -b -p 8900\n", progname);
    return ;
}

