#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void main(void) {
    pid_t pid;

    switch(pid = fork()) {
        case 0:
	    if(execlp("ls", "ls", "-l", (char *)NULL) == -1) {
		perror("execlp");
		exit(1);
	    }
	    break;
	default:
	    system("cat > a.txt");
	    wait(0);
	    printf("Thanks you for executing me\n");
	    break;
    }
}
