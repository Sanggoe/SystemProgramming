#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

void copyfile(char *from, char *to) {
    char buf[30];
    strcat(buf, "cp ");
    strcat(buf, from);
    strcat(buf, " ");
    strcat(buf, to);
    printf("%s", buf);

    system(buf);
}

void copyfile2(char *from, char *to) {
    int fd1, fd2, n;
    char buf[100];


    fd1 = open(from, O_RDWR);
    if(fd1 == -1) {
	perror("open");
	exit(1);
    }

    fd2 = open(to, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(fd2 == -1) {
        perror("open");
        exit(1);
    }

    while((n = read(fd1, buf, 100)) > 0) {
	printf("%s",buf);
	write(fd2, buf, n);
    }

    close(fd1);
    close(fd2);
}

int main(int argc, char** argv) {
    char *from;
    char *to;

    if(argc == 3) {
	from = argv[1];
	to = argv[2];

	copyfile2(from, to);
    }
    return 0;
}
