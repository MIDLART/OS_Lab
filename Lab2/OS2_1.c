#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main () {
	pid_t pid, ppid, pgrp;
	uid_t uid, euid;
	uid_t gid, egid;
	
	pid = getpid();
	ppid = getppid();
	pgrp = getpgrp();
	uid = getuid();
	gid = getgid();
	euid = geteuid();
	egid = getegid();
	
	printf("Идентификатор текущего процесса: %d\n", pid);
	printf("Идентификатор родительского процесса: %d\n", ppid);
	printf("Идентификатор группы вызывающего процесса: %d\n", pgrp);
	printf("Реальный идентификатор владельца: %d\n", uid);
	printf("Реальный идентификатор группы владельца: %d\n", gid);
	printf("Эффективные идентификаторы владельца: %d\n", euid);
	printf("Эффективные идентификаторы группы владельца: %d\n", egid);

	return 0;
}
