#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// open and read result file, return the sum of scores value
int getValue(char *fileName) {
    int fd, i=0;
    char num[5];

    if((fd = open(fileName, O_RDONLY)) < 0) {
	perror("Open"); // check error
	exit(1);
    }
    while(read(fd, &num[i++], 1) > 0 || i<5);

    return atoi(num);
}

int main(void) {
    int status, sum=0;
    pid_t pid1, pid2;


    switch (pid1 = fork()) {
	case -1 : // if fork failed
	    perror("fork");
	    exit(1);
	    break;
	case 0 : // if child process
	    if(execlp("./child", "./child", "data.txt", "1", "50", "result1.txt", (char*)NULL) == -1) {
		perror("execlp");
		exit(1);
	    }
	    exit(0);
	    break;
	default : // if parent process
	    while(wait(&status) != pid1);
	    break;
    }

    switch (pid2 = fork()) {
	case -1 : // if fork failed
            perror("fork");
            exit(1);
            break;
        case 0 : // if child process
            if(execlp("./child", "./child", "data.txt", "51", "100", "result2.txt", (char*)NULL) == -1) {
                perror("execlp");
                exit(1);
            }
            exit(0);
            break;
        default : // if parent process
            while(wait(&status) != pid2);
            break;
    }

    sum += getValue("result1.txt") + getValue("result2.txt");
    printf("total sum of score : %d\n", sum);
    printf("avg of all scores : %.2lf\n", sum/100.0);

    return 0;
}
