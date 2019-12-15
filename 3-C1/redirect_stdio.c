#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv)
{
  int  fd = -1;
  fd = open("std2.txt", O_RDWR|O_CREAT|O_TRUNC, 0666);
  if(fd < 0)
  {
    printf("Open file failure:%s\n",strerror(errno));
    return ;
  }
 dup2(fd, STDIN_FILENO);  		//标准输入重定向到std.txt文件中
// dup2(fd, STDOUT_FILENO);               //标准输出重定向到std.txt文件中
 dup2(fd, STDERR_FILENO);               //标准出错重定向到std.txt文件中

 printf("fd=%d\n",fd);
 
 close(fd);
}

