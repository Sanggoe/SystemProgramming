#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int fd;
    int turn = 0;
    char buf[5];
    pid_t pid;
    caddr_t addr;
    struct stat statbuf;

    if (argc != 2) { // check right commend
	fprintf(stderr, "Usage : %s filename\n", argv[0]);
	exit(1);
    }

    if (stat(argv[1], &statbuf) == -1) { // read stat inform
	perror("stat");
	exit(1);
    }

    if ((fd = open(argv[1], O_RDWR)) == -1) { // file open
	perror("open");
	exit(1);
    }
    
    addr = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE,
	    MAP_SHARED, fd, (off_t)0); // mapping
    if (addr == MAP_FAILED) {
	perror("mmap");
	exit(1);
    }
    close(fd);

    printf("parent\tvalue\tchild\n");
    switch (pid = fork()) {
	case -1: /* fork failed */
	    perror("fork");
	    exit(1);
	    break;
	case 0: /* child process */
	    while(1) {
		if (addr[0] == '1') { // if child turn
		    int value, num = rand()%3 + 1;
		    buf[0] = addr[2];
		    buf[1] = addr[3];
		    buf[2] = '\0';

		    value = atoi(buf) + num;
		    printf("\t %2d %6d\t(c)\n", value, num);
		    if(value > 20) {
			printf("child process loss!\n");
			addr[0] = '2';
			break;
		    }

		    addr[2] = value/10 + '0';
                    addr[3] = value%10 + '0';
		    sleep(1);
		    addr[0] = '0';
		} else if (addr[0] == '2') {
		    printf("child process win!\n");
		    break;
		}
	    }
	    break;
	default: /* parent process */
	    while(1) {
		if (addr[0] == '0') { // if parent turn
		    int value, num = rand()%3 + 1;
                    buf[0] = addr[2];
                    buf[1] = addr[3];
                    buf[2] = '\0';

                    value = atoi(buf) + num;
                    printf("%4d %6d \t\t(p)\n", num, value);
		    if(value > 20) {
                        printf("parent process loss!\n");
                        addr[0] = '2';
                        break;
                    }
        
                    addr[2] = value/10 + '0';
                    addr[3] = value%10 + '0';
                    sleep(1);
		    addr[0] = '1';
		} else if (addr[0] == '2') {
		    printf("parent process win!\n");
		    break;
		}
	    }
	    break;
    }

    return 0;
}
	
