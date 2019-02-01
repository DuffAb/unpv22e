/* include main */
#include	"unpipc.h"

#define	MAXNITEMS 		1000000
#define	MAXNTHREADS			100

int		nitems;			/* read-only by producer and consumer. 线程间共享的全局变量 */
struct {
  pthread_mutex_t	mutex;
  int	buff[MAXNITEMS];
  int	nput;
  int	nval;
} shared = { PTHREAD_MUTEX_INITIALIZER };

void	*produce(void *), *consume(void *);

int
main(int argc, char **argv)
{
	int			i, nthreads, count[MAXNTHREADS];
	pthread_t	tid_produce[MAXNTHREADS], tid_consume;

	// 命令行参数
	if (argc != 3)
		err_quit("usage: prodcons2 <#items> <#threads>");
	nitems = min(atoi(argv[1]), MAXNITEMS);    // 指定生产这存放的条目数
	nthreads = min(atoi(argv[2]), MAXNTHREADS);// 指定待创建生产者线程的数目

	// 设置希望并发多少线程
	Set_concurrency(nthreads);
	/* start all the producer threads。 创建生产者线程 */
	for (i = 0; i < nthreads; i++) {
		count[i] = 0;
		Pthread_create(&tid_produce[i], NULL, produce, &count[i]);
	}

	/* wait for all the producer threads. 等待生产者线程停止 */
	for (i = 0; i < nthreads; i++) {
		Pthread_join(tid_produce[i], NULL);
		printf("count[%d] = %d\n", i, count[i]);	
	}

	/* start, then wait for the consumer thread. 启动消费者线程 */
	Pthread_create(&tid_consume, NULL, consume, NULL);
	Pthread_join(tid_consume, NULL);

	exit(0);
}
/* end main */

/* include producer */
void *
produce(void *arg)
{
	for ( ; ; ) 
	{
		Pthread_mutex_lock(&shared.mutex);
		if (shared.nput >= nitems) 
		{
			Pthread_mutex_unlock(&shared.mutex);
			return(NULL);		/* array is full, we're done */
		}
		shared.buff[shared.nput] = shared.nval;
		shared.nput++;
		shared.nval++;
		Pthread_mutex_unlock(&shared.mutex);
		*((int *) arg) += 1;
	}
}

void *
consume(void *arg)
{
	int		i;

	for (i = 0; i < nitems; i++)
	{
		if (shared.buff[i] != i)
			printf("buff[%d] = %d\n", i, shared.buff[i]);
	}
	return(NULL);
}
/* end producer */
