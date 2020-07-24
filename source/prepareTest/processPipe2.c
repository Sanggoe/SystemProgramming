#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>

int main(void) {
    int fd1[2], fd2[2], n;
    pid_t pid;
    char buf[100];
    int status;

    if(pipe(fd1) == -1) {
	perror("pipe");
	exit(1);
    }

    if(pipe(fd2) == -1) {
	perror("pipe");
	exit(1);
    }


    switch(pid = fork()) {
        case 0:
	    close(fd1[1]);
	    close(fd2[0]);

	    if((n = read(fd1[0], buf, 100)) == -1) {
		perror("read");
		exit(1);
	    }

	    strcat(buf, getcwd(NULL,BUFSIZ));
	    n = strlen(buf) + 1;
	
	    if(write(fd2[1], buf, n) != n ) {
		    perror("write");
		    exit(1);
	
	    }

	    close(fd1[0]);
	    close(fd2[1]);

	    exit(0);
	    break;
	default:
	    close(fd1[0]);
	    close(fd2[1]);
	    scanf("%s", buf);
	    n = strlen(buf) + 1;

	    if(write(fd1[1], buf, n) != n) {
		perror("write");
		exit(1);
	    };

	    if((n = read(fd2[0], buf, 100)) == -1) {
		perror("read");
		exit(1);
	    }

	    close(fd1[1]);
	    close(fd2[0]);
	    waitpid(pid, &status, 0);

	    printf("%s\n", buf);
	    break;
	    
    }
    return 0;
}
