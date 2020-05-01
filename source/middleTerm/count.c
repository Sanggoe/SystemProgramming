#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

void informMessage() {
    printf("명령어 형식을 확인해주세요.\n");
    printf("count [file_name] : show file's bytes\n");
    printf("count [-s] [file_name] : show contents per 3 lines\n");
}

int main(int argc, char** argv) {
    int fd;
    char byte;

    switch(argc) {
	case 0: printf("??");
	    break;
	case 1: 
	    printf("Error! You should enter one file name.\n");
	    break;
	case 2: 
	    if((fd = open(argv[1], O_RDONLY)) < 0) {
		perror("Open error");
		exit(1);
	    }
	    int len = 0;
	    while(read(fd, &byte, 1) > 0) {
		len++;
	    }
	    printf("%s : %d\n", argv[1], --len);
	    break;
	case 3:
	    if(argv[1][0]=='-' && argv[1][1] == 's') {
		if((fd = open(argv[2], O_RDONLY)) < 0) {
		    perror("Open error");
		    exit(1);
		}
		int line = 0;
		while(read(fd, &byte, 1) > 0) {
		    if(byte == '\n' && (++line) == 3) {
			if(getchar() == 'q') {
			    break;
			}
			else {
			    line = 0;
			}
		    }
		    else {
			write(1, &byte, 1);
		    }
		}

	    } else {
		informMessage();
	    }
	    break;
	default:
	    informMessage();
	    break;
    }

    return 0;
}
