#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

int main(void) {
    int shmid, i;
    char *shmaddr;
    key_t key;

    key = ftok("shmfile",1);
    shmid = shmget(key, 20, IPC_CREAT | 0666);
    if(shmid == -1) {
	perror("shmid");
	exit(1);
    }
    
    shmaddr = (char*)shmat(shmid, (char*)NULL, 0);
    
    while(1) {
	sprintf((char *)shmaddr, "%d", rand()%100);
	sleep(1);
    }

    shmdt((char*)shmaddr);

    return 0;
}
