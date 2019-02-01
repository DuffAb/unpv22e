#include	"unpipc.h"
// 图10-10 删除一个
int
main(int argc, char **argv)
{
	if (argc != 2)
		err_quit("usage: semunlink <name>");

	Sem_unlink(argv[1]);

	exit(0);
}
