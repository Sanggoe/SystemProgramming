#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void) {
    int shmid, fd, n;
    char *shmaddr;
    char filename[20], buf[100];
    
    shmid = shmget(IPC_PRIVATE, 20, IPC_CREAT | 0644);
    if(shmid == -1) {
	perror("shmget");
	exit(1);
    }

    switch(fork()) {
	case -1:
	    perror("fork");
	    exit(1);
	    break;
        case 0:
	    sleep(4);
	    shmaddr = (char*)shmat(shmid, (char*)NULL, 0);

	    if((fd = open(shmaddr, O_RDONLY)) == -1) {
		perror("open");
		exit(1);
	    }

	    while((n = read(fd, buf, 100)) > 0) {
		if(write(1, buf, n) != n) {
		    perror("write");
		    exit(1);
		}
	    }
	    break;
	default:
	    shmaddr = (char*)shmat(shmid, (char*)NULL, 0);

	    scanf("%s", shmaddr);
	    break;
    }
    return 0;
}
