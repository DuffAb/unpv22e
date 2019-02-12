/* include main */
#include	"unpipc.h"
// 图10-21使用基于内存信号量的生产者-消费者程序(多个生产者线程)
#define	NBUFF	 	 10
#define	MAXNTHREADS	100

/* read-only by producer and consumer */
int	nitems;		// 所有生产者生产的总条目数
int nproducers;	// 生产者线程的总数目

/* data shared by producers and consumer */
struct {	
  int	buff[NBUFF];
  int	nput;	// 下一个待存入值的缓冲区项的下标(按NBUFF)求模
  int	nputval;// 下一个待存入缓冲区的值
  sem_t	mutex, nempty, nstored;		/* semaphores, not pointers */
} shared;

void	*produce(void *), *consume(void *);

int
main(int argc, char **argv)
{
	int		i, count[MAXNTHREADS];
	pthread_t	tid_produce[MAXNTHREADS], tid_consume;

	if (argc != 3)
		err_quit("usage: prodcons3 <#items> <#producers>");
	nitems = atoi(argv[1]);
	nproducers = min(atoi(argv[2]), MAXNTHREADS);

	/* initialize three semaphores */
	Sem_init(&shared.mutex, 0, 1);
	Sem_init(&shared.nempty, 0, NBUFF);
	Sem_init(&shared.nstored, 0, 0);

	/* create all producers and one consumer */
	Set_concurrency(nproducers + 1);
	for (i = 0; i < nproducers; i++) 
	{
		count[i] = 0;
		Pthread_create(&tid_produce[i], NULL, produce, &count[i]);
	}
	Pthread_create(&tid_consume, NULL, consume, NULL);

	/* wait for all producers and the consumer. 等待所有线程结束 */
	for (i = 0; i < nproducers; i++) 
	{
		Pthread_join(tid_produce[i], NULL);
		printf("count[%d] = %d\n", i, count[i]);	
	}
	Pthread_join(tid_consume, NULL);

	Sem_destroy(&shared.mutex);
	Sem_destroy(&shared.nempty);
	Sem_destroy(&shared.nstored);
	exit(0);
}
/* end main */

/* include produce */
void *produce(void *arg)
{
	for ( ; ; ) 
	{
		Sem_wait(&shared.nempty);	/* wait for at least 1 empty slot */
		Sem_wait(&shared.mutex);

		if (shared.nput >= nitems) 
		{
			Sem_post(&shared.nempty);
			Sem_post(&shared.mutex);
			return(NULL);			/* all done */
		}

		shared.buff[shared.nput % NBUFF] = shared.nputval;
		shared.nput++;
		shared.nputval++;

		Sem_post(&shared.mutex);
		Sem_post(&shared.nstored);	/* 1 more stored item */
		*((int *) arg) += 1;
	}
}
/* end produce */

/* include consume */
void *consume(void *arg)
{
	int		i;

	for (i = 0; i < nitems; i++) 
	{
		Sem_wait(&shared.nstored);		/* wait for at least 1 stored item */
		Sem_wait(&shared.mutex);

		if (shared.buff[i % NBUFF] != i)
			printf("error: buff[%d] = %d\n", i, shared.buff[i % NBUFF]);

		Sem_post(&shared.mutex);
		Sem_post(&shared.nempty);		/* 1 more empty slot */
	}
	return(NULL);
}
/* end consume */
