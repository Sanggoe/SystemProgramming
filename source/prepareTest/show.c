#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char **argv) {
    int fd, n, sum = 0;
    char buf[100], *ptr;

    if(argc < 2) {
	printf("input filename\n");
	exit(1);
    }

    fd = open(argv[1], O_RDONLY);
    if(fd == -1) {
	perror("open");
	exit(1);
    }

    if((n = read(fd, buf, 100)) == -1) {
	perror("read");
	exit(1);
    }

    printf("문자 개수 %d개\n", n);
    ptr = strtok(buf, " ");

    while(ptr != NULL) {
	sum += atoi(ptr);
	ptr = strtok(NULL, " ");
    }

    printf("%d\n", sum);
/*
    while((n = read(fd, buf, 100)) > 0) {
	if(write(1, buf, n) != n) {
	    perror("write");
	    exit(1);
	}
    }
    */
    return 0;
}
