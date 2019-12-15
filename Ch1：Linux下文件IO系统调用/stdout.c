#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MSG_STR "Hello World\n"

int main(int main, char *argv[])
{
  printf("%s",MSG_STR);
  fputs(MSG_STR, stdout);
  write(STDOUT_FILENO, MSG_STR, strlen(MSG_STR));
 
  return 0;
}

