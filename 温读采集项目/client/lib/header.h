/********************************************************************************
 *      Copyright:  (C) 2019 Tang Zhiqiang<t_zhiqiang@163.com>
 *                  All rights reserved.
 *
 *       Filename:  header.h
 *    Description:  This head file is temperature project head file  
 *
 *        Version:  1.0.0(11/12/2019)
 *         Author:  Tang Zhiqiang <t_zhiqiang@163.com>
 *      ChangeLog:  1, Release initial version on "11/12/2019 07:29:02 PM"
 *                 
 ********************************************************************************/

void print_usage(char *progname);
void get_time(char *data_time);
int get_temper(float *temper);
int client_init(int port, char * serv_ip);
//void print_usage(char *progname);
