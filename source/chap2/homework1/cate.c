#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int wfd, n;
    char buf[256];

    if (argc != 2) {
	perror("no name");
	exit(1);
    }

    wfd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (wfd == -1) {
	perror("Open");
    }

    while ((n = read(0, buf, 256)) > 0)
	if (write(wfd, buf, n) != n)
	    perror("Write");

    if (n == -1)
	perror("Read");

    close(wfd);

    return 0;
}
