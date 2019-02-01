#include	"unpipc.h"

#define	MAXMSG	(8192 + sizeof(long))
// 图6-6 从一个System V消息队列中读出一个消息
int
main(int argc, char **argv)
{
	int		c, flag, mqid;
	long	type;
	ssize_t	n;
	struct msgbuf	*buff;

	type = flag = 0;
	while ( (c = Getopt(argc, argv, "nt:")) != -1) {
		switch (c) {
		case 'n':
			flag |= IPC_NOWAIT;// 指定非阻塞
			break;

		case 't':
			type = atol(optarg);
			break;
		}
	}
	if (optind != argc - 1)
		err_quit("usage: msgrcv [ -n ] [ -t type ] <pathname>");

	mqid = Msgget(Ftok(argv[optind], 0), MSG_R);

	buff = Malloc(MAXMSG);

	n = Msgrcv(mqid, buff, MAXMSG, type, flag);
	printf("read %d bytes, type = %ld\n", n, buff->mtype);

	exit(0);
}
