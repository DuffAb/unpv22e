/* include rdlock */
#include	"unpipc.h"
#include	"pthread_rwlock.h"

int
pthread_rwlock_rdlock(pthread_rwlock_t *rw)
{
	int		result;

	if (rw->rw_magic != RW_MAGIC)
		return(EINVAL);

	// 无论何时操作pthread_rwlock_t类型的结构，都必须给其中的rw_mutex成员上锁
	if ( (result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return(result);

	/* give preference to waiting writers */
	/* 总是指示着本读写锁的当前状态 -1：写入锁   0：可用的   大于0：当前容纳着读出锁的数量 */
	while (rw->rw_refcount < 0 || rw->rw_nwaitwriters > 0) 
	{
		rw->rw_nwaitreaders++;
		result = pthread_cond_wait(&rw->rw_condreaders, &rw->rw_mutex);
		rw->rw_nwaitreaders--;
		if (result != 0)
			break;
	}

	// 取得读出锁后把rw_refcount加1
	if (result == 0)
		rw->rw_refcount++;		/* another reader has a read lock */

	// 互斥锁释放
	pthread_mutex_unlock(&rw->rw_mutex);
	return (result);
}
/* end rdlock */

void
Pthread_rwlock_rdlock(pthread_rwlock_t *rw)
{
	int		n;

	if ( (n = pthread_rwlock_rdlock(rw)) == 0)
		return;
	errno = n;
	err_sys("pthread_rwlock_rdlock error");
}
