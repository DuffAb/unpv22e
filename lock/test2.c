#include	"unpipc.h"
// 确定在有一个写入锁待处理期间是否允许有另一个读出锁
int
main(int argc, char **argv)
{
	int		fd;

	fd = Open("test1.data", O_RDWR | O_CREAT, FILE_MODE);

	Read_lock(fd, 0, SEEK_SET, 0);		/* parent read locks entire file */
	printf("%s: parent has read lock\n", Gf_time());

	if (Fork() == 0) {
		/* first child */
		sleep(1);
		printf("%s: first child tries to obtain write lock\n", Gf_time());
		Writew_lock(fd, 0, SEEK_SET, 0);	/* this should block */
		printf("%s: first child obtains write lock\n", Gf_time());
		sleep(2);
		Un_lock(fd, 0, SEEK_SET, 0);
		printf("%s: first child releases write lock\n", Gf_time());
		exit(0);
	}

	if (Fork() == 0) {
		/* second child */
		sleep(3);
		printf("%s: second child tries to obtain read lock\n", Gf_time());
		Readw_lock(fd, 0, SEEK_SET, 0);
		printf("%s: second child obtains read lock\n", Gf_time());
		sleep(4);
		Un_lock(fd, 0, SEEK_SET, 0);
		printf("%s: second child releases read lock\n", Gf_time());
		exit(0);
	}

	/* parent */
	sleep(5);
	Un_lock(fd, 0, SEEK_SET, 0);
	printf("%s: parent releases read lock\n", Gf_time());
	exit(0);
}
