#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#define TEST_DIR  "dir"
int main(int argc, char **argv)
{
  int     rv;
  int	  fd1;
  int	  fd2;
  DIR	  *dirp;
  struct dirent *direntp;

/* 创建文件夹dir，并设置文件夹权限为755 */
if(mkdir(TEST_DIR, 0755) < 0)
{
  printf("creat directory '%s' fialure: %s\n",TEST_DIR, strerror(errno));
  return -1;
}

/* 更改当前工作路径到文件夹dir下去 */
if(chdir(TEST_DIR) < 0)
{
  printf("Change directory to '%s' failure: %s\n",TEST_DIR, strerror(errno));
  rv = -2;
  goto cleanup;
}

/* 在dir文件夹下，创建普通文本文件file1.txt并设置权限位为644 */
if((fd1=creat("file1.txt",0644)) < 0)
{
  printf("Create file1.txt failure: %s\n", strerror(errno));
  rv = -3;
  goto cleanup;
}
/* 在dir文件夹下，创建普通文本文件file2.txt并设置权限位为644 */
if((fd2=creat("file2.txt",0644)) < 0)
{
  printf("Creat file2.txt failure: %s\n", strerror(errno));   
  rv = -4;
  goto cleanup; 
}

/* 更改当前工作路径到父目录中（上一级） */
if(chdir("../") < 0)
{
  printf("Change directory to '%s' fialure:%s\n",TEST_DIR,strerror(errno));
  rv = -5;
  goto cleanup;
}

/* 打开dir文件夹 */
if((dirp=opendir(TEST_DIR)) == NULL)
{
  rv = -6;
  printf("opendir %s error: %s\n",TEST_DIR, strerror(errno));
  goto cleanup;
}

/* 列出dir里面的所有文件和文件夹 */
while((direntp = readdir(dirp)) != NULL)
{
  printf("Find file: %s\n",direntp->d_name);
}

/* 关闭所有打开的文件夹 */
close(dirp);
cleanup:
if(fd1 >= 0) close(fd1);
if(fd2 >= 0) close(fd2);

}

