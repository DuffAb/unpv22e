#include	"unpipc.h"
// 图10-11 取得并输出一个信号量的值
int
main(int argc, char **argv)
{
	sem_t	*sem;
	int		val;

	if (argc != 2)
		err_quit("usage: semgetvalue <name>");

	sem = Sem_open(argv[1], 0);
	Sem_getvalue(sem, &val);
	if (val == 0)
	{
		printf("该信号量当前已上锁\n");
	}
	else if (val < 0)
	{
		printf("等待该信号量解锁的线程数(%d)\n", abs(val));
	}
	printf("value = %d\n", val);

	exit(0);
}
