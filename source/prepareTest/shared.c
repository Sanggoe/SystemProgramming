#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

void process1(char *shmaddr, int *count) {
    int i, dummy = 0;

    shmaddr[*count] = rand()%100;
    (*count)++;

    for(int i=0; i<3000; i++) {
	dummy++;
    }
}

int process2(char *shmaddr, int *count) {
    int num, dummy = 0;

    num = shmaddr[*count];
    (*count)++;

    for(int i=0; i<3000; i++) {
        dummy++;
    }

    return num;
}


int main(void) {
    int shmid, count = 0, sum = 0;
    char *shmaddr;
    srand((unsigned)time(NULL));

    shmid = shmget(IPC_PRIVATE, sizeof(int)*10, IPC_CREAT | 0644);
    if(shmid == -1) {
	perror("shmget");
    	exit(1);
    }

    switch(fork()) {
	case 0:
	    shmaddr = (char*)shmat(shmid, (char*)NULL, 0);
	    
	    for(int i=0; i<10; i++) {
		sum += process2(shmaddr, &count);
	    }

	    printf("sum : %d\n", sum);
	    shmdt((char*)shmaddr);
	    exit(0);
	    break;
	default:
	    shmaddr = (char*)shmat(shmid, (char*)NULL, 0);
	
	    for(int i=0; i<10; i++) {
		process1(shmaddr, &count);
	    }
	    wait(0);
	    break;
    }

    for(int i=0; i<10; i++) {
	printf("%d ", shmaddr[i]);
    }
    puts("");

    shmdt((char*)shmaddr);
    shmctl(shmid, IPC_RMID, (struct shmid_ds*)NULL);

    return 0;
}
