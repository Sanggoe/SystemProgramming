#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    DIR *dp;
    struct dirent *dent;

    if ((dp = opendir("hanbit")) == NULL ) {
	perror("opendir: hanbit");
	exit(1);
    }

    printf("d_reclen    d_name   d_inode\n");
    while ((dent = readdir(dp))) {
	printf("    %-7d", dent->d_reclen);
	printf("%-10s", dent->d_name);
	printf("%-10d\n", (int)dent->d_ino);
    }

    closedir(dp);

    return 0;
}
