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
#include <signal.h>
#include <syslog.h>
//#include <sqlite3.h>
//#include "server_init.c"

#define MAX_EVENTS      512
#define ARRAY_SIZE(x)   (sizeof(x)/sizeof(x[0]))    //前一个是整个数组大小，后一个是第一个元素的大小，得到数组元素个数

static inline void msleep(unsigned long ms);
static inline void print_usage(char *programe);
//void set_socket_rlimit(void);
int socket_server_init(char *listenip, int listen_port);

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
    int     log_fd;
    int     i;
    char    buf[1024];

    int     fds_array[1024];
    fd_set  rdset;
    int     maxfd=0;
    int     found = -1;
    //sqlite3  *db=NULL;
    //char    *zErrMsg = NULL;
    //int     rc;
    //int     len;
    //char    *sql=NULL;

    struct option long_options[]=
    {
        {"daemon", no_argument, NULL, 'd'},
        {"port", required_argument, NULL, 'p'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    progname = basename(argv[0]);

    //Parser the command line parameters 
    while((opt = getopt_long(argc, argv, "dp:h", long_options, NULL)) != -1)
    {
        switch (opt) 
        {
            case 'd':
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
    
    signal(SIGTERM, signal_stop);   //15号信号（SIGTERM）可被捕捉

    if(daemon_run)
    {
        printf("program %s running at the backgroud now\n", argv[0]);
        
        //建立日志系统
        log_fd=open("receive_temper.log", O_CREAT|O_RDWR, 0666);
        if(log_fd < 0) 
        {
            printf("Open the logfile failure: %s\n", strerror(errno));
            return 0;
        }
        //标准输出、标准出错重定向
        dup2(log_fd, STDOUT_FILENO);
        dup2(log_fd, STDERR_FILENO);

        //程序后台运行
        if(daemon(1, 1) <0 )
        {
            printf("Deamon failure: %s", strerror(errno));
            return 0;
        }
    }
    
    if((listenfd=socket_server_init(NULL, serv_port)) < 0)
    {
        printf("ERROR: %s server listen on port %d failure\n", argv[0], serv_port);
        return -2;
    }
    printf("server socket[%d] start to listen on port %d\n", listenfd, serv_port);
    
//     /*   Open database  若没有则创建*/
//    len = sqlite3_open("temper.db", &db);
//    if(len != SQLITE_OK)
//    {
//        sqlite3_close(db);
//        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//        exit(1);
//    }
//    printf("Opened database successfully\n");
//    
//     /*   Create SQL statement */
//    sql ="create table if not exists temperature(ID char(10), datetime char(50), temperature  char(10))";
//    /*   Execute SQL statement */
//    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
//    if( rc != SQLITE_OK )
//    {
//        sqlite3_close(db);
//        fprintf(stderr, "Create table error: %s\n", zErrMsg);
//        return -1;
//    }
//    printf("Table created successfully\n");
    
    /* 清空数组*/
    for (i = 0; i < ARRAY_SIZE(fds_array); i++) 
    {
        fds_array[i]=-1;
    }
    fds_array[0]=listenfd;  //将listenfd作为第一个文件描述符

    while(!g_sigstop)
    {   
        //printf("programe will blocked here...\n");
        FD_ZERO(&rdset);    //清空集合
        for(i=0;i<ARRAY_SIZE(fds_array); i++)
        {   
            if(fds_array[i] < 0)    
            continue;                   //不断寻找已存在的文件描述符

            maxfd = fds_array[i] > maxfd ? fds_array[i]:maxfd;  //找到就改变最大值
            FD_SET(fds_array[i], &rdset);                       //加入到监控的集合中
        }
        printf("programe will blocked here...\n");
        rv=select(maxfd+1, &rdset, NULL, NULL, NULL);
        if(rv<0)
        {
            printf("select failure: %s\n", strerror(errno));
        }
        else 
        if(rv == 0)
        {
            printf("select get timeout\n");
            continue;
        }

        /* listen socket get event means new client start connect now */
        if(FD_ISSET(listenfd, &rdset)) 
        {
            if((connfd=accept(listenfd, (struct sockaddr *)NULL, NULL)) < 0) //不保存客户端信息（结构体指针为NULL）
            {
                printf("accept new client failure: %s\n", strerror(errno));      
            }
            printf("accept new client[%d] OK\n", connfd);
            found = 0;  //未加入到文件描述符集合
            for (i = 0; i < ARRAY_SIZE(fds_array); i++) 
            {
               if(fds_array[i] < 0) 
               {
                    printf("accept new client[%d] and add it into array\n", connfd); //找到空的文件描述符，并赋值给它 
                    fds_array[i] = connfd;
                    found = 1; break;   //客户端文件描述符加入到集合
               } 
            }
            if(!found) 
            {
                printf("accept new client[%d] but full, so refuse it\n", connfd);   
                close(connfd);
            }
        }  
        else    /* data arrive from already connected client */ 
        {
            for (i = 0; i < ARRAY_SIZE(fds_array); i++;) 
            {
                if((fds_array[i] < 0) || !FD_ISSET(fds_array[i], &rdset)) 
                continue;   //不断寻找非零或在描述符集合中的 文件描述符
                

                while 
                memset(buf, 0, sizeof(buf));
                if((rv=read(fds_array[i], buf, sizeof(buf))) <= 0 )
                {
                    printf("socket[%d] read failure or get disconncet and will be remove.\n", fds_array[i]); 
                    close(fds_array[i]);
                    fds_array[i] = -1;
                }
                else //成功读取到客户端传来的数据，进行处理保存到数据库
                {
                              


    
                }
            }

        }
    }//while(!g_sigstop)

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

int socket_server_init(char *listenip, int listen_port)
{
        int                 listenfd;
        int                 on = 1;
        int                 rv = 0;
        struct sockaddr_in  serv_addr;


        if((listenfd=socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        {
            printf("create socket failure: %s\n", strerror(errno));
            return -1;
        }
        //Set socket port reuseable, fix 'Address already in use' bug when socket server restart
        //setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(listen_port);
        if( !listenip)
        {
            /*  监听所有的客户端的IP地址 */
            serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }
        else
        { 
            if(inet_pton(AF_INET,listenip, &serv_addr.sin_addr) <= 0)
            {
                printf("inet_pton() set listen IP failure: %s\n", strerror(errno));
                rv = -2;
                goto Cleanup;
            }
    
        }
        //Set socket port reuseable, fix 'Address already in use' bug when socket server restart
        if((setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void*)&on, sizeof(on))) < 0 )
        {
            printf("setsockopt failure: %s\n", strerror(errno));
            return -2;
        }

        if(bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            printf("bind socket[%d] failure: %s\n", listenfd, strerror(errno));
            rv = -3;
            goto Cleanup;
        }
        if(listen(listenfd, 64) < 0)
        {
            printf("listen() socket[%d] failure: %s\n", listenfd, strerror(errno));
            rv = -4;
            goto Cleanup;
        }
        Cleanup:
        if(rv < 0) 
        {
            close(listenfd);   
        }
        else 
            rv = listenfd;
        
        return rv;
}

