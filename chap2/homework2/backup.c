#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int fd, test;
    int len=0;
    int check;
    int prints[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int rows[10];
    char buf[1024];
    char *cmp;
    char ch;

    // Read file with name of parameter
    fd = open(argv[1], O_RDONLY);

    // Argument's length
    cmp = argv[2];
    while(cmp[len]!='\0')
	len++;

    // Read each lines to buf
    rows[0] = 0; 
    for(int i=0, j=0; read(fd, &buf[i], 1) > 0; i++) {
	printf("%c", buf[i]);
        if(buf[i] == '\n') {
            rows[++j]=i;	    
	    printf("rows[%d]=%d\n",j-1,rows[j-1]);
        }
	test = i;
    }
    printf("\n[%c] [%c] [%c]\n",buf[test-2], buf[test-1], buf[test]);

    // Print each lines from buf
  for(int i=0, j=0; buf[i]!='\0'; i++) {
	printf("[%d]:%c\n", i, buf[i]);
	if(buf[i] == '\n') {
	    printf("enter\n");
	}
	if(buf[i+1] == '\0')
	    printf("lastIndex=[%d], lastChar=[%c]\n", i+1, buf[i+1]);
    }

    // compare is it right
    for(int i=0, j=0, k=0; buf[i]!='\0'; i++) {
	
	if(buf[i] == '\n') { // increase enters[] index
	    printf("before, rows[%d]=%d  ", j, rows[j]);
	    printf("\\n so j++, now this line rows[%d]=%d\n", ++j, rows[j]);
	}

	if(buf[i] == cmp[0]) { // if same first char,
	    for(check=1; check<len; ) { // compare cmp, buf
		if(cmp[check] == buf[++i])
		    check++;
	        else
		    break;
	    }

	    if(check == len) { // if same all
		printf("this is the same. %d\n",i);
		if(k==0) {
		    prints[k++]=rows[j];
		    printf("k == 0 | prints[%d]=%d\n",k-1, prints[k-1]);
		} else if(prints[k-1]!=rows[j]) {
		    prints[k++]=rows[j];
		    printf("k > 0  | prints[%d]=%d\n",k-1, prints[k-1]);
		}
		// put in this row to prints
	    }
	}
    }

    for(int i=0; i<10; i++)
        printf("%d\n",prints[i]);
    printf("start print!! \n");
    for(int i=0; prints[i] != -1; i++) {
	for(int j=prints[i]; buf[j+1]!='\n'; j++) {
    	    printf("%c",buf[j]);
	    if(buf[j+1] == '\n')
	        printf("\n");
	}
    }
    printf("end print!! \n");


    close(fd);
    return 0;
}
