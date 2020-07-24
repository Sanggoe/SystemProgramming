#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(void) {
    int shmid, i;
    char* shmaddr;
    char buf[10];
    int n, sum = 0;

    shmid = shmget(IPC_PRIVATE, 20, IPC_CREAT | 0644);
    if(shmid == -1) {
	perror("shmget");
	exit(1);
    }

    switch(fork()) {
	case 0:
	    shmaddr = (char*)shmat(shmid, (char*)NULL, 0);
	    
	    for(int i=0; i<3; i++) {
		sleep(2);
		strcpy(buf, shmaddr);
		sum += atoi(buf);
	    }

	    printf("%d\n", sum);
	    break;
	default:
	    shmaddr = (char*)shmat(shmid, (char*)NULL, 0);

	    for(int i=0; i<3; i++) {
		n=read(0, buf, 9);
		buf[n]='\0';
		strcpy(shmaddr, buf);
	    }

	    shmdt((char*)shmaddr);
	    shmctl(shmid, IPC_RMID, (struct shmid_ds*)NULL);
	    break;
    }

    return 0;
}
