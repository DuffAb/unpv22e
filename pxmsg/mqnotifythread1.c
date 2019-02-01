#include	"unpipc.h"

mqd_t	mqd;
struct mq_attr	attr;
struct sigevent	sigev;

static void	notify_thread(union sigval);		/* our thread function */

int
main(int argc, char **argv)
{
	if (argc != 2)
		err_quit("usage: mqnotifythread1 <name>");

	mqd = Mq_open(argv[1], O_RDONLY | O_NONBLOCK);
	Mq_getattr(mqd, &attr);

	sigev.sigev_notify = SIGEV_THREAD; // 创建一个新的线程
	sigev.sigev_value.sival_ptr = NULL;// 新线程所用的参数
	sigev.sigev_notify_function = notify_thread; // 新线程调用的函数
	sigev.sigev_notify_attributes = NULL;// 新线程的属性，这里使用系统默认属性
	Mq_notify(mqd, &sigev);

	for ( ; ; )
		pause();		/* each new thread does everything */

	exit(0);
}

static void
notify_thread(union sigval arg)
{
	ssize_t	n;
	void	*buff;

	printf("notify_thread started\n");
	buff = Malloc(attr.mq_msgsize);
	Mq_notify(mqd, &sigev);			/* reregister */

	while ( (n = mq_receive(mqd, buff, attr.mq_msgsize, NULL)) >= 0) {
		printf("read %ld bytes\n", (long) n);
	}
	if (errno != EAGAIN)
		err_sys("mq_receive error");

	free(buff);
	pthread_exit(NULL);
}
