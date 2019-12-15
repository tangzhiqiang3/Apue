/*********************************************************************************
 *      Copyright:  (C) 2019 Tang Zhiqiang<t_zhiqiang@163.com>
 *                  All rights reserved.
 *
 *       Filename:  client_init.c
 *    Description:  This file is cliet initial function.
 *                 
 *        Version:  1.0.0(11/09/2019)
 *         Author:  Tang Zhiqiang <t_zhiqiang@163.com>
 *      ChangeLog:  1, Release initial version on "11/09/2019 05:57:49 PM"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <netinet/in.h>




int client_init(int port, char *serv_ip);

int client_init(int port, char *serv_ip)
{
    int                 con_fd = -1;
    int 		        rv = -1;
    struct sockaddr_in  serv_addr;

    con_fd=socket(AF_INET, SOCK_STREAM, 0);

    if(con_fd < 0)
    { 
        printf("Create socket failure: %s\n", strerror(errno));
        return -1;
    }
    printf("Create socket[%d] sucessfully!\n", con_fd); 
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(port);
    inet_aton(serv_ip, &serv_addr.sin_addr);
    
    if(con_fd >= 0)
    {
        rv=connect(con_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if(rv < 0)
        {
            printf("Connect to server[%s:%d] failure: %s\n", serv_ip, port, strerror(errno));
            return -2;
        }
        printf("Connect to server[%s:%d] sucessfully!\n", serv_ip, port);
    }

    return con_fd;
}

