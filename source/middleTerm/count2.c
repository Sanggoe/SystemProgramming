#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

// to show information
void informMessage() {
    printf("명령어 형식을 확인해주세요.\n");
    printf("count [file_name] : show file's bytes\n");
    printf("count [-s] [file_name] : show contents per 3 lines\n");
}

// to count file's bytes
int countFileBytes(char* fileName) {
    int fd, n;
    char byte;

    if((fd = open(fileName, O_RDONLY)) < 0) {
	perror("Open error"); // check error
        exit(1);
    }
    int len = 0;
    while((n = read(fd, &byte, 1)) > 0) {
        if(n < 0) {
	    perror("Read"); // check error
	    exit(1);
	}
	else {
	    len++;
	}
    }
    return len;
}

int main(int argc, char** argv) {
    int fd, len;
    char* buf;

    switch(argc) {
	case 1: 
	    printf("Error! You should enter one file name.\n");
	    break;
	case 2: 
	    len = countFileBytes(argv[1]);
	    printf("%s : %d\n", argv[1], len);
	    break;
	case 3:
	    if(argv[1][0]=='-' && argv[1][1] == 's') {
		if((fd = open(argv[2], O_RDONLY)) < 0) {
		    perror("Open error"); // check error
		    exit(1);
		}
		len = countFileBytes(argv[2]);
		buf = malloc(sizeof(char) * len); // memory allocate
		
		if(read(fd, buf, len) < 0) {
		    perror("Read"); // check error
		    exit(1);
		}

		int line = 0;
		for(int i=0; i<len; i++) {
		    if(buf[i] == '\n' && (++line) == 3) { // check every 3th line.
			if(getchar() == 'q') {
			    break;
			} else {
			    line = 0;
			}
		    } else {
			if(write(1, &buf[i], 1) < 0) {
			    perror("Write"); // check error
			    exit(1);
			}
		    }
		}
		free(buf); // memory free
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
