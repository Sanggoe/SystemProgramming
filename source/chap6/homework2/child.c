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
	
    // open data file to read
    if((fd = open(argv[1], O_RDONLY)) < 0) {
	perror("Open"); // check error
	exit(1);
    }

    // read data file
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

    // sum of scores from start to end
    for(i = start-1; i<end; i++)
	sum += scores[i];
    sprintf(bufNum, "%d", sum); // int to string

    close(fd);
  
    // open result file to write
    if((fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
	perror("Open"); // check error
	exit(1);
    }

    // write result file
    for(i=0; bufNum[i] != '\0'; i++) {
	if(write(fd, &bufNum[i], 1) == -1 ) {
	    perror("write"); // check error
	    exit(1);
	}
    }

    close(fd);
    
    return 0;
}
