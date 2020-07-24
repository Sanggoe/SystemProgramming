#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    int fd[2];
    pid_t pid;
    char buf[10];
    int n, status, sum = 0;

    if (pipe(fd) == -1) {
	perror("pipe");
	exit(1);
    }

    switch(pid = fork()) {
	case 0:
	    close(fd[1]);
	    for(int i=0; i<3; i++) {
		read(fd[0], buf, 10);
		sum += atoi(buf);
	    }

	    printf("%d\n", sum);
	    break;
	default:
	    close(fd[0]);
	    for(int i=0; i<3; i++) {
		n=read(0, buf, 9);
		buf[n]='\0';
		write(fd[1], buf, n+1);
	    }
	    break;
    }

    return 0;
}
