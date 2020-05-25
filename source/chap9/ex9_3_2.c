#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int fd[2];
    pid_t pid;
    char buf[257];
    int get[10];
    int i, value, len, status;

    if (pipe(fd) == -1) {
	perror("pipe");
	exit(1);
    }

    switch (pid = fork()) {
	case -1:
	    perror("fork");
	    exit(1);
	    break;
	case 0: /* child */
	    close(fd[1]);
	    write(1, "Child Process: ", 15);
	    len = read(fd[0], buf, 256);
	    printf("Childe got len from fd[0]\n");
	    write(1, buf, len);
	    printf("Child print it\n");
	    value = atoi(buf);
	    for (i=0; i<value; i++) {
		len = read(fd[0], buf, 256);
		printf("Child got len from fd[0]\n");
		buf[len] = '\0';
		write(1, buf, len);
		printf("Child print it\n");
	    }
	    close(fd[0]);
	    break;
	default:
	    close(fd[0]);
	    buf[0] = '\0';
	    printf("parent will generate 5 random numbers\n");
	    write(fd[1], "5", 2);
	    sleep(4);
	    for(i=0; i<5; i++) {
		value = rand() % 1000;
		printf("Parent %d : %d \n", i, value);
		sprintf(buf, "%d\n", value);
		write(fd[1], buf, strlen(buf)+1);
		sleep(4);
	    }

	    close(fd[1]);
	    waitpid(pid, &status, 0);
	    break;
    }

    return 0;
}
