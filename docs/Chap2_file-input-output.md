# 01 개요

<br/>

## 파일 

파일은 관련 있는 데이터의 집합을 말한다.

| 종류      | 용도                                                         |
| --------- | ------------------------------------------------------------ |
| 일반 파일 | 텍스트 바이너리 형태의 데이터를 저장하는 파일로, 외부 저장 장치의 데이터를 저장하는 단위 |
| 특수 파일 | 데이터 전송, 장치 접근에 사용하는 파일로, 모든 입출력 장치를 포함하는 의미 |

<br/>

#### 저수준(low-level) 파일 입출력 특징

* 유닉스 커널의 시스템 호출을 사용하여 파일 입출력을 수행한다.
* 파일에 좀 더 빠르게 접근할 수 있다.
* 바이트 단위로 파일의 내용을 다루므로 일반 파일과 특수파일 까지 읽고 쓸 수 있다.
* 여러 가지 추가 기능을 일일이 다 구현해야 하는 단점이 있다.
* 열린 파일을 참조하는데 파일 기술자(file descriptor)를 사용한다.
* 실제 데이터를 읽고 쓰는 함수는 read, write 뿐이다.

<br/>

#### 고수준(high-level) 파일 입출력 특징

* C언어의 표준 함수로 제공된다.
* 단위를 한정하지 않고 버퍼를 이용해 한꺼번에 읽기 쓰기를 수행한다.
* 입출력 라이브러리 라고도 한다.
* 열린 파일의 특성에 관한 정보를 저장하는 구조체(struct)를 가리키는 파일 포인터(file pointer)를 사용한다.

* 데이터를 읽고 쓰는 함수에 fread, fwrite, fgets, fputs, pfrintf, fscanf 등 다양한 함수가 있다.

<br/>

<br/>

# 02 저수준 파일 입출력 

<br/>

## 1_ 파일 기술자

#### 파일 기술자 특징

* 파일 구분 목적으로 유닉스가 붙여놓은 번호이다
* 정수(integer) 값이다.
* Open함수를 사용해 파일을 열었을 때 부여된다.
* 0번부터 가장 작은 값에 할당된다.
* 0번은 표준 입력(Standard input) - 키보드
* 1번은 표준 출력(Standard output) - 모니터
* 2번은 표준 오류 출력(Standard error) - 모니터
* 따라서 프로세스가 파일을 열면 3번부터 할당된다.
* 0, 1, 2번도 사용자에 의해 close 하면 할당될 수 있다.

<br/>

## 2_ 파일 생성과 열고 닫기

#### 파일 사용 순서

* 파일을 먼저 연다.
* 파일에 대한 작업을 한다.
* 파일을 닫는다.

<br/>

### 파일 열기 : open(2)

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *path, int oflag [, mode_t, mode]);
```

* path : 열려는 파일이 있는 경로
* mode : 접근 권한
* oflag : 파일 상태 플래그

#### open 함수의 특징

* 시스템 호출 함수이다. (2)
* 파일을 여는데 사용하는 함수이다. 
* path에 지정한 파일을 oflag에 지정한 상태 플래그 값에 따라 연다.
* 파일 열기에 성공하면 파일 기술자를 반환한다.
* 파일 열기에 실패하면 -1을 리턴하고, 외부 변수 errno에 실패한 이유 설명 오류코드를 저장한다.

<br/>

#### 주요 oflag

| 종류                | 기능                                                         |
| ------------------- | ------------------------------------------------------------ |
| O_RDONLY            | 파일 읽기 전용 열기                                          |
| O_WRONLY            | 파일 쓰기 전용 열기                                          |
| O_RDWR              | 파일 읽기 쓰기 가능하게 열기                                 |
| O_CREAT             | 파일이 없으면 파일 생성                                      |
| O_EXCL              | O_CREAT 옵션과 함께 사용 시 기존에 없는 파일이면 파일 생성, 이미 있으면 오류 메시지 출력. EXCLUSIVE 의미. |
| O_APPEND            | 기존에 있는 파일 열기, 파일의 맨 끝에 내용을 추가            |
| O_TRUNC             | 파일 생성 시 이미 있는 파일이고 쓰기 옵션으로 열었으면 내용을 모두 지우고 파일 길이를 0으로 변경 |
| O_NONBLOCK/O_NDELAY | 비블로킹(Non-blocking) 입출력                                |
| O_SYNC/O_DSYNC      | 저장장치에 쓰기가 끝나야 쓰기 동작을 완료                    |

* 플래그 사용 시, OR(|) 연산자로 연결해 지정할 수 있다.

<br/>

#### 사용 예

| ex)                                        | code)                          |
| ------------------------------------------ | ------------------------------ |
| 쓰기 전용으로 열 때, 이미 파일이 있는 경우 | O_WRONLY \| O_TRUNC            |
| 쓰기 전용으로 열 때, 파일이 없는 경우      | O_WRONLY \| O_CREAT \| O_TRUNC |
| 읽기/쓰기/추가용으로 열 때                 | O_RDWR \| O_APPEND             |

* mode는 파일 접근 권한을 설정하는 것으로, O_CREAT 플래그를 지정해 파일을 생성할 때만 사용한다. 

<br/>

#### 설정 가능한 mode 종류

| 플래그  | 모드 | 설명                            |
| ------- | ---- | ------------------------------- |
| S_IRWXU | 0700 | 소유자 읽기/쓰기/실행 권한      |
| S_IRUSR | 0400 | 소유자 읽기 권한                |
| S_IWUSR | 0200 | 소유자 쓰기 권한                |
| S_IXUSR | 0100 | 소유자 실행 권한                |
| S_IRWXG | 0070 | 그룹 읽기/쓰기/실행 권한        |
| S_IRGRP | 0040 | 그룹 읽기 권한                  |
| S_IWGRP | 0020 | 그룹 쓰기 권한                  |
| S_IXGRP | 0010 | 그룹 실행 권한                  |
| S_IRWXO | 0007 | 기타 사용자 읽기/쓰기/실행 권한 |
| S_IROTH | 0004 | 기타 사용자 읽기 권한           |
| S_IWOTH | 0002 | 기타 사용자 쓰기 권한           |
| S_IXOTH | 0001 | 기타 사용자 실행 권한           |

* 0644의 경우 mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

<br/>

### 파일 생성 : creat(2)

```c
#include <sys/stat.h>
#include <fcntl.h>

int creat(const char *path, mode_t mode);
```

* path : 파일 생성 경로
* mode : 접근 권한

#### creat 함수의 특징

* 시스템 호출 함수이다. (2)
* open 함수에 파일 생성 기능이 없었던 구버전 유닉스에서 사용하던 것이다.
* open 함수와 달리 옵션 지정 부분이 없다.
* create(path, mode) == open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
* 파일 생성 시 파일 기술자를 리턴한다.

<br/>

### 파일 닫기 : close(2)

```c
#include <unistd.h>

int close(int fildes);
```

* fildes : 파일 기술자

#### close 함수의 특징

* 시스템 호출 함수이다. (2)
* 한 프로세스가 열 수 있는 파일의 개수에 제한이 있기 때문에 파일을 닫아야 한다.
* 제대로 닫지 않으면 허용 개수를 초과해 파일을 더이상 못 열수도 있다.
* 따라서 파일을 닫을 때 사용한다.
* 성공적으로 닫으면 0을 리턴한다.
* 닫기에 실패하면 -1을 리턴하고, 오류코드를 외부 변수 errno에 저장한다.
* 물론, 개발자가 파일을 닫아주지 않아도 운영체제가 닫아주긴 한다.
* 하지만 닫는 습관을 들이자.

<br/>

#### 예제 2-1) 새 파일 생성하기

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {
    int fd;
    mode_t mode;

    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // 파일 접근권한 지정

    fd = open("unix.txt", O_CREAT, mode);	// 0644 권한으로 생성
    if (fd == -1) {
	perror("Creat");	// 오류 발생 시
	exit(1);
    }
    close(fd);

    return 0;
}
```

```c
* 결과
# ls unix.txt
unix.txt: file cannot found
# gcc -o ex2_1 ex2_1.c
# ./ex2_1
# ls -l unix.txt
-rw-r--r-- 1 sanggoe sanggoe 0  4월 23 23:45 unix.txt
```

<br/>

#### 예제 2-2) O_EXCL 플래그 사용하기

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int fd;

    fd = open("unix.txt", O_CREAT | O_EXCL, 0644); // 0644 지정
    if (fd == -1) {
	perror("Excl");
	exit(1);
    }
    close(fd);
    printf("unix.txt : fd = %d\n", fd);

    return 0;
}
```

```c
* 결과
# ls unix.txt
unix.txt
# ./ex2_2
Excl: File exists

# rm unix.txt
# ./ex2_2
# ls unix.txt
unix.txt
```

<br/>

#### 예제 2-3) 파일 기술자 할당하기

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int fd;

    close(0); // 0번 표준 입력(키보드) 파일을 닫는다.

    fd = open("unix.txt", O_RDWR); // 가장 작은 번호인 0이 할당된다.
    if (fd == -1) {
	perror("Excl");
	exit(1);
    }

    printf("unix.txt : fd = %d\n", fd);
    close(fd);

    return 0;
}

```

```c
* 결과
# ex2_3
unix.txt : fd = 0
```

<br/>

## 3_ 파일 읽기와 쓰기

<br/>

### 파일 읽기 : read(2)

```c
#include <unistd.h>

ssize_t read(int fildes, void *buf, size_t nbytes);
```

* fildes : 파일 기술자
* buf : 바이트를 저장할 메모리 영역의 시작 주소
* nbytes : 읽어올 바이트 수

#### read 함수의 특징

* 시스템 호출 함수이다. (2)
* 파일 기술자가 가리키는 파일에서 nbytes로 지정한 크기만큼 바이트를 읽어온다.
* 읽어온 bytes를 buf로 지정한 메모리 영역에 저장한다.
* 실제로 읽어온 bytes 수를 리턴한다.
* 오류가 발생하면 -1을 리턴한다.
* 함수를 실행할 때 마다 읽어온 크기만큼 오프셋이 이동한다.
* 무조건 byte 단위로 읽어온다.

<br/>

#### 예제 2-4) 파일 읽기

```c
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int fd, n;
    char buf[10];

    fd = open("unix.txt", O_RDONLY); // 읽기 전용으로 연다
    if (fd == -1) {
	perror("Open");
	exit(1);
    }

    n = read(fd, buf, 6); // 6bytes를 읽어 buf에 저정, read의 리턴값 저장
    if(n == -1) {
	perror("Read");
	exit(1);
    }

    buf[n] = '\0'; // 읽어온 문자열 끝에 널문자를 추가
    printf("n=%d, buf=%s\n", n, buf);
    close(fd);

    return 0;
}
```

```c
* 결과
# cat unix.txt
Unix System Programming
# ex2_4
n=6, buf=Unix S
```

<br/>

### 파일 쓰기 : write(2)

```c
#include <unistd.h>

ssize_t write(int fildes, const void *buf, size_t nbytes);
```

* fildes : 파일 기술자
* buf : 파일에 기록할 데이터를 저장한 메모리 영역
* nbytes : buf의 크기 (기록할 데이터의 크기)

#### write 함수의 특징

* 시스템 호출 함수이다. (2)
* 파일 기술자는 쓰기를 수행할 파일을 가리킨다.
* buf는 파일에 기록할 데이터를 저장하고 있는 메모리 영역을 가리킨다.
* buf가 가리키는 메모리 영역에서 nbytes로 지정한 크기만큼 읽어 파일에 쓰기를 수핸한다.
* 실제 쓰기를 수행한 bytes 수를 리턴한다.
* 오류가 발생하면 -1을 리턴한다. 

<br/>

#### 예제 2-5) 파일 읽고 쓰기

```c
int main(void) {
    int rfd, wfd, n;
    char buf[10];

    rfd = open("unix.txt", O_RDONLY);
    if (rfd == -1) {
	perror("Open unix.txt");
	exit(1);
    }

    wfd = open("unix.bak", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (wfd == -1) {
	perror("Open unix.bak");
	exit(1);
    }

    while ((n = read(rfd, buf, 6)) > 0) {
	if (write(wfd, buf, n) != n)
		perror("Write");
    }

    if (n ==-1)
	perror("Read");

    close(rfd);
    close(wfd);

    return 0;
}
```

```c
* 결과
# ls unix.bak
unix.bak: file cannot found
# ex2_5
# cat unix.bak
Unix System Programming
```

<br/>

## 4_ 파일 오프셋 지정

<br/>

#### 오프셋 특징 (offset)

* 파일 내용을 읽거나 쓸 때, 현재 읽을 위치나 쓸 위치를 알려주는 역할
* 파일의 시작 지점에서 현재 위치까지의 바이트 수를 말한다.
* 파일을 열면 오프셋 초기값은 0이다.
* read / write 함수를 수행할 때 마다 수행되는 bytes 수만큼 오프셋이 이동한다.
* 한 파일에 파일 오프셋은 오직 하나만 존재한다.
* 읽기 쓰기 모드에서도 별도로 있는게 아니다. 주의!

<br/>

### 파일 오프셋 위치 지정 : lseek(2)

```c
#include <sys/types.h>
#include <unistd.h>

off_t lseek(int fildes, off_t offset, int whence);
```

* fildes : 파일 기술자
* offset : 이동할 오프셋의 위치
* whence : 오프셋의 기준 위치

<br/>

#### lseek 함수 특징

* 시스템 호출 함수이다. (2)

* 파일 기술자가 가리키는 파일에서 offset으로 지정한 크기만큼 오프셋을 이동시킨다.

* offset의 값은 whence의 값을 기준으로 해석한다.

* byte 단위 기준으로 수행된다.

* whence 부분에 사용 가능한 값은 아래 표와 같다.

  * | 값       | 설명             |
    | -------- | ---------------- |
    | SEEK_SET | 파일의 시작 기준 |
    | SEEK_CUR | 현재 위치 기준   |
    | SEEK_END | 파일의 끝 기준   |

    * ex)  lseek(fd, 5, SEEK_SET);   :arrow_right: 파일의 시작에서 5 번째 위치로 이동
    * ex)  lseek(fd, 0, SEEK_END);  :arrow_right: 파일의 끝에서 0번째 즉, 파일의 끝으로 이동
    * ex)  lseek(fd, 0, SEEK_CUR);  :arrow_right: 파일의 현재 위치에서 0번째 즉, 현재 위치를 반환

* 성공하면 새로운 오프셋을 리턴한다.

* 실패하면 -1을 리턴한다.

<br/>

#### 예제 2-6) 파일 오프셋 사용하기

```c
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int fd, n;
    off_t start, cur;
    char buf[256];

    fd = open("unix.txt", O_RDONLY); // 읽기 전용 열기
    if (fd == -1) {
	perror("Open unix.txt");
	exit(1);
    }

    start = lseek(fd, 0, SEEK_CUR); // 현재 위치 (0)
    n = read(fd, buf, 255);
    buf[n] = '\0';
    printf("OFFset start=%d, Read Str=%s, n=%d\n", (int)start, buf, n);
    cur = lseek(fd, 0, SEEK_CUR); // 읽은 후 오프셋의 현재 위치
    printf("Offset cur=%d\n", (int)cur);

    start = lseek(fd, 5, SEEK_SET); // 시작 기준 오프셋이 5인 위치 이동
    n = read(fd, buf, 255);
    buf[n] = '\0';
    printf("Offset start=%d, Read Str=%s", (int)start, buf);

    close(fd);

    return 0;
}
```

```c
* 결과
# ex2_6
Offset start=0, Read Str=Unix System Programming
, n=24
Offset cur=24
Offset start=5, Read Str=System Programming
```

<br/>

## 5_ 파일 기술자 복사

* 파일을 열면 파일 기술자가 할당된다.
* 파일 기술자를 복사해 같은 파일을 가리키는 두 번째 파일 기술자를 생성할 수 있다./>

### 파일 기술자 복사하기 : dup(2)

```c
#include <unistd.h>

int dup(int fildes);
```

* fildes : 파일 기술자

#### dup 함수의 특징

* 시스템 호출 함수이다. (2)
* 기존 파일 기술자를 인자로 받아 새로운 파일 기술자를 리턴한다.
* 파일 기술자 중 할당 가능한 가장 작은 값으로 자동 할당 된다.
* 입출력 방향 전환에서 많이 사용한다.

<br/>

#### 예제 2-7) 파일 기술자 복사하기

```c
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int fd, fd1;

    fd = open("tmp.aaa", O_CREAT | O_WRONLY | O_TRUNC, 0644); // 0644 권한
    if (fd == -1) {
	perror("Create tmp.aaa");
	exit(1);
    }

    close(1); // 1번 파일기술자인 표준 출력(모니터)을 닫는다.

    fd1 = dup(fd); // 파일 기술자를 새로 할당한다. 가능한 제일 작은 값은 1이다.

    printf("DUP FD=%d\n", fd1);
    printf("Standard Output Redirection\n");
    close(fd);

    return 0;
}
```

```
* 결과
# ex2_7
# cat tmp.aaa
DUP FD=1
Standard Output Redirection
```

<br/>

### 파일 기술자 복사 : dup2(3)

```c
#include <unistd.h>

int dup2(int fildes, int fildes2);
```

* fildes : 파일 기술자
* fildes : 파일 기술자를 복사할 곳

#### dup2 함수의 특징

* C 라이브러리 함수이다. (3)
* 새로운 파일 기술자를 지정할 수 있게 해준다.
* 파일 기술자 fildes를 fildes2로 복사한다.

<br/>

#### 예제 2-8) dup2 함수 사용하기

```c
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int fd;

    fd = open("tmp.bbb", O_CREAT | O_WRONLY | O_TRUNC, 0644); // 0644 권한
    if (fd == -1) {
	perror("Create tmp.bbb");
	exit(1);
    }

    dup(fd, 1); // 기존 파일 기술자를 표준 출력(1) 파일 기술자에 새로 할당한다.
    
    printf("DUP2 : Standard Output Redirection\n");
    close(fd);

    return 0;
}
```

```c
* 결과
# ex2_8
# cat tmp.bbb
DUP2 : Standard Output Redirection
```

<br/>

## 6_ 파일 기술자 제어

<br/>

### 파일 기술자 제어 : fcntl(2)

```c
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int fcntl(int fildes, int cmd, /* arg */ ...);
```

* fildes : 파일 기술자
* cmd : 명령
* arg : cmd에 따라 필요시 지정하는 인자들

#### fcntl 함수의 특징

* 파일에 대한 파일 기술자의 속성을 확인하고 제어할 수 있다.

* 파일을 열 때 설정한 플래그들을 조정하는 역할을 수행한다.

* 파일 기술자가 가리키는 파일에 cmd로 지정한 명령을 수행한다.

* cmd의 종류에 따라 인자를 지정해야 할 수도 있다.

* 보통은 아래 두 가지를 많이 사용한다.

  * | 명령어  | 의미                                                         |
    | ------- | ------------------------------------------------------------ |
    | F_GETFL | 상태 플래그 정보를 읽어온다.                                 |
    | F_SETFL | 상태 플래그 정보를 설정한다.<br />설정할 수 있는 플래그는 대부분 Open 함수에서 지정하는 플래그다. |

#### 예제 2-9) fcntl 함수로 파일 기술자 제어하기

```c
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int fd, flags;

    fd = open("unix.txt", O_RDWR); // 읽기 쓰기 권한으로 열기
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    if ((flags = fcntl(fd, F_GETFL)) == -1) { // 플래그 정보 읽어오기
        perror("fcntl");
        exit(1);
    }

    flags |= O_APPEND; // 파일의 맨 끝에 내용을 추가하는 flag를 flags에 추가

    if (fcntl(fd, F_SETFL, flags) == -1) { // 플래그 정보 설정하기
        perror("fcntl");
        exit(1);
    }

    if (write(fd, "Hanbit Media", 12) != 12) perror("write");
    close(fd);

    return 0;
}
```

```
# cat unix.txt
Unix System Programming
# ex2_9.out
# cat unix.txt
Unix System Programming
Hanbit Media
```

<br/>

## 7_ 파일 삭제

<br/>

### 파일 삭제 : unlink(2)

```c
#include <unistd.h>

int unlink(const char *path);
```

* path : 파일의 경로

#### unlink 함수의 특징

* 시스템 호출 함수이다. (2)

* path에 지정한 파일의 inode에서 링크 수를 감소시킨다.

* 링크 수가 0이 되면 path에 지정한 파일이 삭제된다.

* 파일이 위치한 디렉터리에 쓰기 권한이 있어야 함수를 사용할 수 있다.

* 파일 뿐 아니라 디렉토리도 삭제한다.

* 디렉토리가 비어있지 않아도 삭제된다.

  * 비슷한 기능으로 라이브러리 함수 remove(3)가 있다.

  * ```c
    #include <stdio.h>
    int remove(const char *path);
    ```

    * path : 경로
    * 해당 디렉토리가 비어있을 때만 삭제한다.

<br/>

#### 예제 2-10) unlink 함수로 파일 삭제하기

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int cnt;

    cnt = unlink("tmp.aaa");
    if (cnt == -1) {
        perror("Unlink tmp.aaa");
        exit(1);
    }
    
    printf("Unlink tmp.aaa success!!!\n");

    return 0;
}
```

```
* 결과
# ls -l tmp*
-rw-r--r-- 1 sanggoe sanggoe 5  4월 24 01:35 tmp.aaa
-rw-r--r-- 1 sanggoe sanggoe 4  4월 24 01:35 tmp.bbb
# ex2_10
Unlink tmp.aaa success!!!
# ls -l tmp*
-rw-r--r-- 1 sanggoe sanggoe 4  4월 24 01:35 tmp.bbb
```

<br/>

## 8_ 파일과 디스크 동기화 함수

<br/>

### 동기화 함수 : fsync(3)

```c
#include <unistd.h>

int fsync(int fildes);
```

* fildes : 파일 기술자

#### fsync 함수의 특징

* 메모리에 위치하고 있는 파일의 내용을 디스크로 보내 메모리와 디스크의 내용을 동기화 한다.
* 메모리의 내용이 디스크로 모두 기록되기 전에는 리턴하지 않는다.

<br/>

<br/>

# 03 고수준 파일 입출력

이 부분은 기본 C언어 라이브러리 함수이므로, 생략한다.