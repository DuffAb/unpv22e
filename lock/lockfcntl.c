#include	"unpipc.h"

void
my_lock(int fd)
{
	struct flock	lock;

	// 锁住整个文件的第一种方式
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;				/* write lock entire file */

	Fcntl(fd, F_SETLKW, &lock);// F_SETLKW：如果锁不可得，我们希望阻塞到它变为可得为止
}

void
my_unlock(int fd)
{
	struct flock	lock;

	lock.l_type = F_UNLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;				/* unlock entire file */

	Fcntl(fd, F_SETLK, &lock);// F_SETLK：释放锁，因为 lock.l_type = F_UNLCK
}
