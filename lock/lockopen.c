#include	"unpipc.h"
// 图9-12 使用指定O_CREAT和O_EXCL标志的open实现的锁函数
#define	LOCKFILE	"/tmp/seqno.lock"

void
my_lock(int fd)
{
	int		tempfd;

	while ( (tempfd = open(LOCKFILE, O_RDWR|O_CREAT|O_EXCL, FILE_MODE)) < 0) 
	{
		if (errno != EEXIST)
			err_sys("open error for lock file");
		/* someone else has the lock, loop around and try again */
	}
	Close(tempfd);			/* opened the file, we have the lock */
}

void
my_unlock(int fd)
{
	Unlink(LOCKFILE);		/* release lock by removing file */
}
