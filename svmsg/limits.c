#include	"unpipc.h"

#define	MAX_DATA	64*1024
#define	MAX_NMESG	4096
#define	MAX_NIDS	4096
int		max_mesg;

struct mymesg {
  long	type;
  char	data[MAX_DATA];
} mesg;

int
main(int argc, char **argv)
{
	int		i, j, msqid, qid[MAX_NIDS];

	/* first try and determine maximum amount of data we can send */
	msqid = Msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT);
	mesg.type = 1;
	// 为确定最大消息大小，我们尝试发送一个含有65536字节数据的消息，
	// 如果失败了就尝试含有65408字节数据的消息，直到成功为止
	for (i = MAX_DATA; i > 0; i -= 128)
	{
		if (msgsnd(msqid, &mesg, i, 0) == 0) 
		{
			printf("maximum amount of data per message = %d\n", i);
			max_mesg = i;
			break;
		}
		if (errno != EINVAL)
			err_sys("msgsnd error for length %d", i);
	}
	if (i == 0)
		err_quit("i == 0");
	Msgctl(msqid, IPC_RMID, NULL);

	/* see how many messages of varying size can be put onto a queue */
	// 确定一个队列中可以放置多少不同大小的消息
	mesg.type = 1;
	for (i = 8; i <= max_mesg; i *= 2) 
	{
		msqid = Msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT);
		for (j = 0; j < MAX_NMESG; j++) 
		{
			if (msgsnd(msqid, &mesg, i, IPC_NOWAIT) != 0) 
			{
				if (errno == EAGAIN)
					break;
				err_sys("msgsnd error, i = %d, j = %d", i, j);
				break;
			}
		}
		printf("%d %d-byte messages were placed onto queue,", j, i);
		printf(" %d bytes total\n", i*j);
		Msgctl(msqid, IPC_RMID, NULL);
	}

	/* see how many identifiers we can "open" */
	// 确定同时可打开多少标识符
	mesg.type = 1;
	for (i = 0; i <= MAX_NIDS; i++) 
	{
		if ( (qid[i] = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT)) == -1) 
		{
			printf("%d identifiers open at once\n", i);
			break;
		}
	}
	for (j = 0; j < i; j++)
		Msgctl(qid[j], IPC_RMID, NULL);

	exit(0);
}
