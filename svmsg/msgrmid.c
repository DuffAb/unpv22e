#include	"unpipc.h"
// 图6-7 删除一个System V消息队列
int
main(int argc, char **argv)
{
	int		mqid;

	if (argc != 2)
		err_quit("usage: msgrmid <pathname>");

	mqid = Msgget(Ftok(argv[1], 0), 0);
	Msgctl(mqid, IPC_RMID, NULL);

	exit(0);
}
