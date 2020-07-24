#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int main(int argc, char **argv) {
    int fd, n;
    char buf[100];

    if(argc != 2) {
	printf("input filename!\n");
	exit(1);
    }

    if(argc == 2) {
	if((fd = open(argv[1], O_RDONLY)) == -1) {
	    perror("open");
	    exit(1);
	}

	while((n = read(fd, buf, 100)) > 0) {
	    if(write(1, buf, n) != n) {
		perror("write");
	    }
	}    
    }

    return 0;
}
