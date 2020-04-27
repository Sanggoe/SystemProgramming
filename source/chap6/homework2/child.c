#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int fd, n;

    fd = open(argv[2], O_RDONLY);
    if(fd < 0) {
	perror("Open");
	exit(1);
    }


//    while(read(fd, 

    printf("this is child program\nI'll give you information\n");
    printf("%s, %s, %d, %d\n", argv[0], argv[1], atoi(argv[2]), atoi(argv[3]));
    
    sleep(1);
    
    return 0;
}
