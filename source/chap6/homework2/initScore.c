#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void) {
    FILE *rfp, *wfp;
    int i;
    srand((unsigned)time(NULL));

    if ((rfp = fopen("data.txt", "r")) == NULL) {
	perror("fopen: data.txt");
	exit(1);
    }

    if ((wfp = fopen("data.txt", "w")) == NULL) {
	perror("fopen: data.txt");
	exit(1);
    }

    for (i=0; i<100; i++) {
        fprintf(wfp, "%d %d\n", 2020301001+i, rand()%101);
    }
    
    fclose(rfp);
    fclose(wfp);

    return 0;
}
