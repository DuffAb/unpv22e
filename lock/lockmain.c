#include	"unpipc.h"
// 图9-2 文件上锁例子的main函数
#define	SEQFILE	"seqno"		/* filename */

void	my_lock(int), my_unlock(int);

int
main(int argc, char **argv)
{
	int		fd;
	long	i, seqno;
	pid_t	pid;
	ssize_t	n;
	char	line[MAXLINE + 1];

	pid = getpid();
	fd = Open(SEQFILE, O_RDWR, FILE_MODE);

	for (i = 0; i < 20; i++) {
		my_lock(fd);				/* lock the file */

		// (1) 读出文件序列号
		Lseek(fd, 0L, SEEK_SET);	/* rewind before read */
		n = Read(fd, line, MAXLINE);
		line[n] = '\0';				/* null terminate for sscanf */

		// (2) 使用其中的序列号
		n = sscanf(line, "%ld\n", &seqno);
		printf("%s: pid = %ld, seq# = %ld\n", argv[0], (long) pid, seqno);

		// (3) 给序列号+1并写回文件中
		seqno++;					/* increment sequence number */
		snprintf(line, sizeof(line), "%ld\n", seqno);
		Lseek(fd, 0L, SEEK_SET);	/* rewind before write */
		Write(fd, line, strlen(line));

		my_unlock(fd);				/* unlock the file */
	}
	exit(0);
}
