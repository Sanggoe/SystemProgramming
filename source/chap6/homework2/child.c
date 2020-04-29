#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int fd;
    int start, end;
    int scores[100];
    int i=0, sum = 0, n;
    char buf, bufNum[5];

    start = atoi(argv[2]);
    end = atoi(argv[3]);
	
    printf("%s 오픈할꺼야 다음 안뜨면 오류남\n", argv[1]);
    if((fd = open(argv[1], O_RDONLY)) < 0) {
	perror("Open"); // check error
	exit(1);
    }
    printf("%s 만들 자식 %s 파일 오픈 했음\n", argv[4], argv[2] );

    while((n = read(fd, &buf, 1)) != 0) {
	if(n == -1) { // check error
	    perror("Read");
	    exit(1);
	}
	if(buf == ' ' ) { // if space simbol
	    int j=0;
	    while((n = read(fd, &buf, 1)) != 0 && buf != '\n') { // before buf is '\n' or nothing
		if(n == -1) { // check error
		    perror("Read");
		    exit(1);
		  }

		bufNum[j++] = buf;
	    }
	    bufNum[j] = '\0'; // last character should be NULL
	    scores[i++] = atoi(bufNum);
	}
    }
    for(int i = start-1; i<end; i++) // sum [start ~ end] scores
	sum += scores[i];

    close(fd);
  
    if((fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
	perror("Open"); // check error
	exit(1);
    }
    printf("%s 만들라고 파일 오픈 했음\n", argv[4]);

    sprintf(bufNum, "%d", sum);
    printf("%s\n", bufNum);
    
    for(i=0; bufNum[i] != '\0'; i++) {
	if(write(fd, &bufNum[i], 1) == -1 ) {
	    perror("write"); // check error
	    exit(1);
	}
    }
    sleep(1);
    return 0;
}
