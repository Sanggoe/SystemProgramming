#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// copy to buf from addr
void addrToBuf(caddr_t addr, char* buf){
    int i=2;

    while (addr[i] != '\n' && addr[i] != '\0') {
	buf[i-2] = addr[i];
	i++;
    }
    buf[i-2] = '\0'; // set the last index to '\0'
}

// copy to addr from buf
void bufToAddr(caddr_t addr, char* buf, int value) {
    int i = 0;

    // int to char before value become 0
    while (value != 0) {
	buf[i++] = value % 10 + '0';
	value /= 10;
    }
    i--;

    // buf to addr
    for(int j=2; i>=0; i--, j++) {
	addr[j] = buf[i];
    }
}


int main(int argc, char* argv[]) {
    int fd;
    int value, randNum;
    char buf[5];
    pid_t pid;
    caddr_t addr;
    struct stat statbuf;
    srand((unsigned)time(NULL));

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

    printf("parent\tvalue\tchild\n\t %2d\n", (int)(addr[2]-'0'));

    switch (pid = fork()) {
	case -1: /* fork failed */
	    perror("fork");
	    exit(1);
	    break;
	case 0: /* child process */
	    while(1) {
		if (addr[0] == '1') { // if child turn
		    addrToBuf(addr, buf);

		    randNum = rand() % 3 + 1;
		    value = atoi(buf) + randNum; // char to int
		    printf("\t %2d %6d\t(c)\n", value, randNum);

		    if(value > 20) {
			printf("child process loss!\n");
			addr[0] = '2';
			break;
		    }

		    bufToAddr(addr, buf, value);
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
		    addrToBuf(addr, buf);
		    
		    randNum = rand() % 3 + 1;
                    value = atoi(buf) + randNum;
                    printf("%4d %6d \t\t(p)\n", randNum, value);

		    if(value > 20) {
                        printf("parent process loss!\n");
                       	addr[0] = '2';
                        break;
                    }
        
		    bufToAddr(addr, buf, value);
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
