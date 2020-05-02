#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char buf[BUFSIZ];
    char *argvName = argv[1];

    while(1) {
	if(read(0, &buf, BUFSIZ) > 0) {
	    printf("%s", buf);
	    exit(1);
	}
	getcwd(buf, BUFSIZ);
	printf("%s $ ", buf);
	switch(argc) {
	    case 1:
		if (chdir(argv[1])) {
                perror("error"); // if chdir failed
                exit(1);
                }
	    break;
	}
        getcwd(buf, 255);
        printf("현재 작업 디렉토리 : %s\n", buf);
        exit(0);
	
    }
    return 0;
}
