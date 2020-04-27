#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    printf ("Before \n");
    if (execlp("ls", "ls", "-a", (char *)NULL) == -1) {
	perror("execlp error");
	exit(1);
    }
    printf("After \n");
    return 0;
}
