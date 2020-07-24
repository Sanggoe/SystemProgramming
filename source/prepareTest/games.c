#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

void handler(int dummy) { ; }

int main(void) {
    int shmid, i, parent;
    char *shmaddr;
    key_t key;
    sigset_t mask;
    srand((unsigned)time(NULL));

    key = ftok("shmfile",1);
    shmid = shmget(key, 20, IPC_CREAT | 0666);
    if(shmid == -1) {
	perror("shmid");
	exit(1);
    }
    
    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1);
    signal(SIGUSR1, handler);
    sigsuspend(&mask);

    shmaddr = (char*)shmat(shmid, (char*)NULL, 0);
    parent = rand()%3;
    printf("child : %d, parent : %d\n", shmaddr[0], parent);

    switch(shmaddr[0]) {
	case 0:
	    if(parent == 1)
		printf("부모가 이겼습니다\n");
	    else if(parent == 2)
		printf("자식이 이겼습니다\n");
	    else
		printf("비겼습니다\n");
	    break;
	case 1:
	    if(parent == 2)
                    printf("부모가 이겼습니다\n");              
                else if(parent == 0)
                    printf("자식이 이겼습니다\n");
                else
                    printf("비겼습니다\n");
	    break;
	case 2:
	    if(parent == 0)
                    printf("부모가 이겼습니다\n");
                else if(parent == 1)
                    printf("자식이 이겼습니다\n");
                else
                    printf("비겼습니다\n");
	    break;
	default:
	    printf("잘못된 값 Error\n");
	    exit(1);
	    break;
    }

    shmdt((char*)shmaddr);

    return 0;
}
