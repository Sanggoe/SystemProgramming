#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int status;
    pid_t pid;

    switch (pid = fork()) {
	case -1 : /* forrk failed */
	    perror("fork");
	    exit(1);
	    break;
	case 0 :  /* child process */
	    printf("--> Child Process\n");
	    exit(2);
	    break;
	default : /* parent process */
	    while(wait(&status) != pid) // wait() 만 동작시켜도 상관은 없다
		continue;
	    printf("--> parent process\n");
	    printf("status: %d, %x\n", status, status);
	    printf("child process Exit Status:%d\n", status >> 8);
	    break;
    }

    return 0;
}
