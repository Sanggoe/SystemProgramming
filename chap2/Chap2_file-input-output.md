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

* 파일을 여는데 사용하는 함수이다. 
* 시스템 호출 함수이다. (2)
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

* open 함수에 파일 생성 기능이 없었던 구버전 유닉스에서 사용하던 것이다.
* open 함수와 달리 옵션 지정 부분이 없다.
* 시스템 호출 함수이다. (2)
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

```
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

```
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

```
* 결과
# ex2_3
unix.txt : fd = 0
```

<br/>

### 파일 읽기 : read(2)

```c
#include <unistd.h>

ssize_t read(int fildes, void *buf, size_t nbytes);
```

* fildes : 파일 기술자
* buf : 파일에 기록할 데이터를 저장한 메모리 영역
* nbytes : buf의 크기 (기록할 데이터의 크기)



### 파일 쓰기 : write(2)

```
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

