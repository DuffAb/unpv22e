/* include mq_unlink */
#include	"unpipc.h"
#include	"mqueue.h"

int
mymq_unlink(const char *pathname)
{
	// 删除与我们定义的某个消息队列相关联的名字
	if (unlink(pathname) == -1)
		return(-1);
	return(0);
}
/* end mq_unlink */

void
Mymq_unlink(const char *pathname)
{
	if (mymq_unlink(pathname) == -1)
		err_sys("mymq_unlink error");
}
