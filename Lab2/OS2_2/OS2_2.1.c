#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main () {
	int pid;
	pid = fork();
	printf("%d\n", pid);
	
	/*fork() дублирует текущий процесс. Весь код после fork() выполняется дважды. 
	На печать выводится 2 числа:
	целое положительное число (PID потомка) - возвращает родительский процесс
	0 - возвращает процесс-потомок
	*/
	
	return 0;
}
