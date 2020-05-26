#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    int fd1[2], fd2[2];
    int fd;
    pid_t pid;
    char buf[257];
    char token[5];
    int count[3] = {0};
    int len, status;

    if (pipe(fd1) == -1) {
	perror("pipe");
	exit(1);
    }

    if (pipe(fd2) == -1) {
	perror("pipe");
	exit(1);
    }

    switch (pid = fork()) {
	case -1 :
	    perror("fork");
	    exit(1);
	    break;
	case 0 : /* child */
	    close(fd1[1]);
	    close(fd2[0]);

	    // receive from parent
	    while((len = read(fd1[0], buf, 100)) > 0) {

		/* ascii code => '0':48, '9':57, 'A':65, 'Z':90, 'a':97, 'z':122 */
    		for(int i=0; i<len; i++) { // check each characters
		    if(buf[i] == 65 || buf[i] == 69 || buf[i] == 73 || buf[i] == 79 || buf[i] == 85 ||
			buf[i] == 97 || buf[i] == 101 || buf[i] == 105 || buf[i] == 111 || buf[i] == 117) {
			count[0]++; // upper, lower vowels (a,e,i,o,u,A,E,I,O,U)
		    } else if ((buf[i] >= 65 && buf[i] <= 90) || (buf[i] >= 97 && buf[i] <= 122)) {
			count[1]++; // upper, lower cosonants (all alphabets)
		    } else if (buf[i] >= 48 && buf[i] <= 57) {
			count[2]++; // numbers (0 ~ 9)
		   }
		}

		if(len < 100) {
		    break;
		}
	    }

	    sprintf(buf, "%d\n%d\n%d\n", count[0], count[1], count[2]); // int to char
	    write(fd2[1], buf, strlen(buf));
	    break;
	default :
	    close(fd1[0]);
	    close(fd2[1]);

	    if ((fd = open("data.txt", O_RDONLY)) == -1) {
		perror("open");
		exit(1);
	    }

	    // read from file and send to child
	    while((len = read(fd, buf, 100)) > 0) {
		write(fd1[1], buf, len);
	    }
    	    
	    // receive from child
	    len = read(fd2[0], buf, 100);
	    for(int i=0, j=0, k=0; i<len; i++) {
		if(buf[i] == '\n') { // divide by token
		    token[j] = '\0';
		    count[k] = atoi(token);
		    k++;
		    j=0;
		} else {
		    token[j] = buf[i];
		    j++;
		}
	    }

	    printf("모음: %d개, 자음: %d개, 숫자: %d개\n", count[0], count[1], count[2]);
	    waitpid(pid, &status, 0);
	    break;
    }
    
    return 0;
}
