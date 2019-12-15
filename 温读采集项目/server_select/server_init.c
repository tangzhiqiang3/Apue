/*********************************************************************************
 *      Copyright:  (C) 2019 Tang Zhiqiang<t_zhiqiang@163.com>
 *                  All rights reserved.
 *
 *       Filename:  server_init.c
 *    Description:  This file is socket server initial 
 *                 
 *        Version:  1.0.0(11/10/2019)
 *         Author:  Tang Zhiqiang <t_zhiqiang@163.com>
 *      ChangeLog:  1, Release initial version on "11/10/2019 09:07:00 PM"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <getopt.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <syslog.h>
#include <sys/resource.h>
#include <time.h>
#include <pthread.h>

int socket_server_init(char *listenip, int listen_port);

int socket_server_init(char *listenip, int listen_port)
{
    int                 sockfd;
    int                 on = 1;
    struct sockaddr_in  serv_addr;


    if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        printf("create socket failure: %s\n", strerror(errno));
        return -1;
    }
    //Set socket port reuseable, fix 'Address already in use' bug when socket server restart
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(listen_port);
    
    if( !listenip)
    {
        /* 监听所有的客户端的IP地址 */
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        /* 监听指定的客户端的IP地址 */
        if(inet_pton(AF_INET,listenip, &serv_addr.sin_addr) < 0)
        printf("inet_pton() set listen IP failure: %s\n", strerror(errno));
        return -2;
        goto CleanUp;
    }

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("bind socket[%d] failure: %s\n", sockfd, strerror(errno));
        return -3;
        goto CleanUp;
    }

    if(listen(sockfd, 64) < 0)
    {
        printf("listen() socket[%d] failure: %s\n", sockfd, strerror(errno));
        return -4;
        goto CleanUp;
    }

CleanUp:
    close(sockfd);

    return sockfd;
}

