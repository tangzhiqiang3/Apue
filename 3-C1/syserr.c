#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  char *file_name = "/test.txt";
  int fd = -1;

  fd = open(file_name, O_RDONLY, 066);  
  if(fd < 0)
  {
    perror("Open file failure");
    printf("Open file %s failure: %s\n", file_name, strerror(errno));
  }
  close(fd);
}

