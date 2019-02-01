#include	"svmsg.h"
// 图6-10 使用消息队列的服务器程序main函数
void	server(int, int);

int
main(int argc, char **argv)
{
	int		readid, writeid;

	readid = Msgget(MQ_KEY1, SVMSG_MODE | IPC_CREAT);
	writeid = Msgget(MQ_KEY2, SVMSG_MODE | IPC_CREAT);

	server(readid, writeid);

	exit(0);
}
