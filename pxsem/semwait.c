#include	"unpipc.h"
// 图10-12 等待一个信号量并输出它的值
int
main(int argc, char **argv)
{
	sem_t	*sem;
	int		val;

	if (argc != 2)
		err_quit("usage: semwait <name>");

	sem = Sem_open(argv[1], 0);
	Sem_wait(sem);
	Sem_getvalue(sem, &val);
	if (val == 0)
	{
		printf("该信号量当前已上锁\n");
	}
	else if (val < 0)
	{
		printf("等待该信号量解锁的线程数(%d)\n", abs(val));
	}
	printf("pid %ld has semaphore, value = %d\n", (long) getpid(), val);

	pause();	/* blocks until killed */
	exit(0);
}
