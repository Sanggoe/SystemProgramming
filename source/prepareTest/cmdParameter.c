#include <stdio.h>

void sOption() {
    
}


int main(int argc, char **argv) {
    
    if(argc < 2) {
        // T 명령어 수행
    } else if(argc == 2) {
	if(argv[1][0] == '-' && argv[1][1] == l) {
	    // 자세한 파일 정보 보기
	} else if(argv[1][0] == '-' && argv[1][1] == 'i') {
	    // i-node 값 보기
	} else if(argv[1][0] == 'a' && argv[1][1] == '*') {
	    // a로 시작하는 파일 정보만 보여주기
	}
    } else if(argc == 3) {
	if(argv[1][0] == '-' && argv[1][1] == 'i' &&
	    argv[2][0] == 'a' && argv[2][1] == '*') {
	    // a로 시작하는 파일의 정보를 자세히 보여주기
	}
    }



}
