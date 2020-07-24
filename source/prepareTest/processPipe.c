#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>

int main(void) {
    int fd[2], fd1, n;
    pid_t pid;
    char filename[20], buf[20];
    int status;

    if(pipe(fd) == -1) {
	perror("pipe");
	exit(1);
    }

    switch(pid = fork()) {
        case 0:
	    close(fd[1]);
	    if((n = read(fd[0], filename, 20)) == -1) {
		perror("read");
		exit(1);
	    }

	    if((fd1 = open(filename, O_RDONLY)) == -1) {
		perror("open");
		exit(1);
	    }

	    while((n = read(fd1, buf, 20)) > 0) {
		if(write(1, buf, n) != n ) {
		    perror("write");
		    exit(1);
		}
	    }

	    close(fd[0]);
	    break;
	default:
	    close(fd[0]);
	    scanf("%s", filename);
	    n = strlen(filename)+1;
	    if(write(fd[1], filename, n) != n) {
		perror("write");
		exit(1);
	    };

	    close(fd[1]);
	    waitpid(pid, &status, 0);
	    break;
	    
    }
    return 0;
}
