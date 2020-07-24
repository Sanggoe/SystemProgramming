#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>

int main(void) {
    int fd, sum = 0;
    char buf[30];
    char *ptr;

    if((fd = open("input.txt", O_RDONLY)) == -1) {
	perror("open");
	exit(1);
    }

    read(fd, buf, 30);

    ptr = strtok(buf, " ");
    while(ptr != NULL) {
	sum += atoi(ptr);
	ptr = strtok(NULL, " ");
    }

    printf("sum = %d\n", sum);
    return 0;
}
