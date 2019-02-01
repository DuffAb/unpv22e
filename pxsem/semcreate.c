#include	"unpipc.h"
// 图10-9 创建一个有名信号量
int
main(int argc, char **argv)
{
	int		c, flags;
	sem_t	*sem;
	unsigned int	value;

	flags = O_RDWR | O_CREAT;
	value = 1;
	while ( (c = Getopt(argc, argv, "ei:")) != -1) 
	{
		switch (c) {
		case 'e':
			flags |= O_EXCL;
			break;

		case 'i':
			value = atoi(optarg);
			break;
		}
	}
	if (optind != argc - 1)
		err_quit("usage: semcreate [ -e ] [ -i initalvalue ] <name>");
	printf("%s\n", argv[optind]);
	sem = Sem_open(argv[optind], flags, FILE_MODE, value);
	// sem = sem_open(argv[optind], O_CREAT, 0666, 5);
	// if (sem == SEM_FAILED)
	// {
	// 	err_sys("sem_open error for %s", argv[optind]);
	// }
	Sem_close(sem);
	exit(0);
}
