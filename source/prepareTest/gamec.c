#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char** argv) {
    int shmid, i, parent;
    char *shmaddr;
    key_t key;
    srand((unsigned)time(NULL));

    key = ftok("shmfile",1);
    shmid = shmget(key, 20, 0);
    if(shmid == -1) {
	perror("shmid");
	exit(1);
    }
    
    shmaddr = (char*)shmat(shmid, (char*)NULL, 0);
    shmaddr[0] = rand()%3;

    system("ipcs -m");
    shmdt((char*)shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
