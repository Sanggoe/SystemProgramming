#include <fcntl.h>
#include <unistd.h>

// String Exit Return 1, Not Exist Return 0
int find_str(char* src, int src_len, char* cmp, int cmp_len) {
    int x = 0;
    int y = 0;
    for (; x < src_len; x++) {
        if (src[x] == cmp[y]) {
            y++;
            if (y == cmp_len) { // Cmp String Find
                return 1;
            }
        } else {
            y=0;
        }
    }
    return 0;
}

void print_str(char* src) {
    int x = 0;
    for(; x < src[x]!='\0'; x++) {
	write(1, &src[x], 1);
    }
    write(1, "\n", 1);   
}

int main(int argc, char *argv[]) {
    int fd;
    int len=0;
    int prints[50] = {0,}; // Buffer Initialized
    char buf[2500] = {0,};
    char *cmp;

    // Read file with name of parameter
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        print_str("Can Not Found File : ");
	print_str(argv[1]);
        return 1;
    }

    // Argument's length
    cmp = argv[2];
    while(cmp[len]!='\0') {
        len++;
    }

    // Read each lines to buf
    int i = 0; //
    while (read(fd, &buf[i], 1) > 0) {
        if(buf[i] == '\n') {
            buf[i] = '\0';
            if (find_str(buf, i, cmp, len) > 0) {
		print_str(buf);
            }
            i = -1;
        }
        i++;
    }

    // Last Line Check
    buf[i] = '\0';
    if (find_str(buf, i, cmp, len) > 0) {
        print_str(buf);
    }

    close(fd);
    return 0;
}
