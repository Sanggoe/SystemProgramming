#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int fd, fd2, n;
    char buf[100];

    if(argc < 2) {
	exit(1);
    }

    if(argc == 3 && argv[1][0] == '>') {
	fd = open(argv[2], O_RDWR | O_TRUNC, 0644);
        if(fd == -1) { 
            perror("open");
            exit(1);
        }

        while((n=read(0, buf, 100)) > 0) {
            if(write(fd, buf, n) != n) {
		perror("write");
	    }
        }
    } else {
	fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
	    perror("open");
	    exit(1);
	}

        while(n=read(fd, buf, 100) > 0) {
            write(1, buf, n);
        }
    }
}

