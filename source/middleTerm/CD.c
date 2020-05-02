#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
    char *s = "/home/sanggoe/class/SystemProgramming/source/";

    chdir(s);
    /*
    switch(argc) {
	case 1:
	    chdir(getenv("HOME") );
	    break;
	case 2:
	    getcwd(s, BUFSIZ)
	    if(chdir(argv[1])) {
		printf("No directory\n");
		getcwd(s, BUFSIZ);
		
	    }
	    break;
	default:
	    printf("USAGE: cd [dir]\n");
	    break;
    }
    */
    return 0;
}
