#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int main(void) {
    int shmid, i, parent;
    char *shmaddr;
    srand((unsigned)time(NULL));

    shmid = shmget(IPC_PRIVATE, 20, IPC_CREAT | 0644);
    if(shmid == -1) {
	perror("shmid");
	exit(1);
    }
    
    switch(fork()) {
	case 0:
	    
	    shmaddr = (char*)shmat(shmid, (char*)NULL, 0);
	    shmaddr[0] = rand()%3;

	    shmdt((char*)shmaddr);
	    exit(0);
	    break;
	default:
	    wait(0);
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
    }
    return 0;
}
