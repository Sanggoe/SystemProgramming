#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int getValue(char *fileName) {
    int fd, i=0;
    char num[10];

    fd = open(fileName, O_RDONLY);
    if (fd < 0) {
	printf("Can Not Found File : ");
	printf("%s\n", fileName);
	exit(1);
    }

    while(read(fd, &num[i++], 1) > 0 || i<10);
	printf("%s\n", num);

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
	    printf("자식1 불렀어!\n");
	    if(execlp("./child", "./child", "data.txt", "1", "50", "result1.txt", (char*)NULL) == -1) {
		perror("execlp");
		exit(1);
	    }
	    exit(0);
	    break;
	default : // if parent process
	    while(wait(&status) != pid1);
	    printf("wait() 끝남. 자식1 종료\n");
	    break;
    }

    switch (pid2 = fork()) {
	case -1 : // if fork failed
            perror("fork");
            exit(1);
            break;
        case 0 : // if child process
	    printf("자식2 불렀어!\n");
            if(execlp("./child", "./child", "data.txt", "51", "100", "result2.txt", (char*)NULL) == -1) {
                perror("execlp");
                exit(1);
            }
            exit(0);
            break;
        default : // if parent process
            while(wait(&status) != pid2);
	    printf("wait() 끝남. 자식2 종료\n");
            break;
    }

    sum += getValue("result1.txt") + getValue("result2.txt");
    printf("total sum of score is : %d\n", sum);

    return 0;
}
