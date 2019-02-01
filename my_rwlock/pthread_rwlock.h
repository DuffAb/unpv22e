/* include rwlockh */
#ifndef	__pthread_rwlock_h
#define	__pthread_rwlock_h

typedef struct {
  pthread_mutex_t	rw_mutex;		    /* basic lock on this struct. 互斥锁 */
  pthread_cond_t	rw_condreaders;	/* for reader threads waiting. 读条件变量 */
  pthread_cond_t	rw_condwriters;	/* for writer threads waiting. 写条件变量 */
  int				rw_magic;		    /* for error checking */
  int				rw_nwaitreaders;/* the number waiting */
  int				rw_nwaitwriters;/* the number waiting */
  int				rw_refcount;    /* 总是指示着本读写锁的当前状态 -1：写入锁   0：可用的   大于0：当前容纳着读出锁的数量 */
	/* 4-1 if writer has the lock, else # readers holding the lock */
} pthread_rwlock_t;

#define	RW_MAGIC	0x19283746

/* following must have same order as elements in struct above */
#define	PTHREAD_RWLOCK_INITIALIZER	{ PTHREAD_MUTEX_INITIALIZER, \
			PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, \
			RW_MAGIC, 0, 0, 0 }

typedef	int		pthread_rwlockattr_t;	/* dummy; not supported */

/* function prototypes */
int		pthread_rwlock_destroy(pthread_rwlock_t *);
int		pthread_rwlock_init(pthread_rwlock_t *, pthread_rwlockattr_t *);
int		pthread_rwlock_rdlock(pthread_rwlock_t *);
int		pthread_rwlock_tryrdlock(pthread_rwlock_t *);
int		pthread_rwlock_trywrlock(pthread_rwlock_t *);
int		pthread_rwlock_unlock(pthread_rwlock_t *);
int		pthread_rwlock_wrlock(pthread_rwlock_t *);

/* and our wrapper functions */
void	Pthread_rwlock_destroy(pthread_rwlock_t *);
void	Pthread_rwlock_init(pthread_rwlock_t *, pthread_rwlockattr_t *);
void	Pthread_rwlock_rdlock(pthread_rwlock_t *);
int		Pthread_rwlock_tryrdlock(pthread_rwlock_t *);
int		Pthread_rwlock_trywrlock(pthread_rwlock_t *);
void	Pthread_rwlock_unlock(pthread_rwlock_t *);
void	Pthread_rwlock_wrlock(pthread_rwlock_t *);

#endif	__pthread_rwlock_h
/* end rwlockh */
