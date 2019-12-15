#include <sys/types.h>
#include <sys/stat.h>  
#include <unistd.h>

int main (int argc, char **argv)
{
//   struct stat {
//        dev_t      st_dev;		/* ID of device containing file */ 
//        ino_t      st_ino;		/* inode number */
//	mode_t     st_mode; 		/* protection */
//	nlink_t    st_nlink; 		/* number of hard links */
//	uid_t      st_uid;		/*user ID of owner */
//	gid_t      st_gid;		/* group ID of owner */
//	dev_t      st_rdev;		/* device ID (if special file) */
//	off_t      st_size;		/* total size, in bytes */
//	blksize_t  st_blksize;	 	/* blocksize for filesystem I/O */
//	blkcnt_t   st_blocks; 		/* number of 512B blocks allocated */
//	time_t     st_atime; 		/* time of last access */
//	time_t     st_mtime;		/* time of last modification */
//	time_t     st_ctime;		/* time of last status change */
//}stbuf;

  struct stat stbuf;			//只用该结构体的变量
  
  stat("stat.c", &stbuf);
  printf("File Mode: %o Real Size: %luB, Space Size: %luB\n",stbuf.st_mode, stbuf.st_size, stbuf.st_blksize);
  
  return 0; 
}

