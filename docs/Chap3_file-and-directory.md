# 01 개요

<br/>

#### 유닉스에서의 파일

* 파일
  * 텍스트 파일, 실행파일 등 바이너리 형태의 데이터 저장 파일
  * 데이터 저장 뿐 아니라 전송 및 장치에 접근하기 위한 파일
  * 해당 디렉토리에 속한 파일을 관리하는 특별한 파일

<br/>

#### 파일의 구성

* 파일명 : 사용자가 파일에 접근할 때 사용
* inode : 외부적으로는 번호 표시, 파일에 관한 정보 및 주소들이 저장
* 데이터 블록 : 실제 데이터가 저장되는 하드디스크 공간

<br/>

#### 링크

* 기존 파일이나 디렉토리에 접근할 수 있는 새로운 이름을 의미
* 하드링크
  * 기존 파일과 동일한 inode 사용
* 심볼릭 링크
  * 기존 파일과 다른 inode 사용
  * 기존 파일의 경로를 저장

<br/>

<br/>

# 02 유닉스 파일의 특징

<br/>

## 1_ 파일의 종류

유닉스에서 파일은 크게 일반파일, 특수파일, 디렉토리로 구분할 수 있다.

<br/>

### 일반 파일

* 텍스트 파일, 실행 파일, 라이브러리, 이미지 등 유닉스에서 사용하는 대부분의 파일
* 편집기나 다른 응용 프로그램을 통해 생성

<br/>

### 특수 파일 (장치 파일)

* 다른 파일과 달리 장치 관련 특수 파일을 장치 파일이라고도 한다.

* 장치 파일은 데이터 블록을 사용하지 않는다.

* 대신 장치의 종류를 나타내는 장치 번호를 inode에 저장한다.

* 블록 장치 파일

  * 블록 단위로 데이터를 읽고 쓴다.
  * 기본 블록크기는 8KB이다.

* 문자 장치 파일

  * 하드디스크인 경우 섹터 단위로 읽고 쓴다.
  * 기본 섹터 크기는 512byte이다.
  * 로우 디바이스 라고도 한다.

  <br/>

#### 디렉토리

* 유닉스에서는 디렉토리도 파일로 취급한다.
* 디렉토리와 연관된 데이터 블록은 해당 디렉토리에 속한 파일의 목록과 inode를 저장한다.

* 관련 명령어
  * mkdir : 디렉토리 생성
  * rmdir : 디렉토리 삭제
    * rm -r : 디렉토리 삭제
    * cp -r : 디렉토리 복사

<br/>

#### 파일의 종류 구분

* ls -l 명령으로 첫 글자를 통해 파일의 종류를 확인 할 수 있다.

```
# ls -l
-rw-r--r-- 1 sanggoe sanggoe  285  4월 24 09:34 README.md
drwxr-xr-x 3 sanggoe sanggoe 4096  4월 13 06:48 chap1
```

* 첫 번째 글자인 -, d가 파일의 종류를 나타낸다.

<br/>

| 문자 | 파일의 종류         |
| ---- | ------------------- |
| -    | 일반 파일           |
| d    | 디렉토리            |
| b    | 블록 장치 특수 파일 |
| c    | 문자 장치 특수 파일 |
| l    | 심볼릭 링크         |

<br/>

## 2_ 파일의  구성요소

#### 파일명

* 사용자가 파일에 접근할 때 사용
* 파일명과 관련된 inode가 반드시 있어야 한다.
* 파일명은 최대 255자 까지 사용 가능
* 파일 및 디렉토리 이름 생성 규칙
  * 알파벳, 숫자, 하이픈(-), 언더바(_), 점(.) 만을 사용한다.
  * 공백, 위에 언급된 특수문자 이외의 특수문자들은 사용할 수 없다.
  * 알파벳은 대소문자를 구분한다
  * 점(.) 으로 시작하면 숨김 파일이다.

<br/>

**파일에 엑세스 하는 방법에는 절대경로로 엑세스와 inode 숫자 값으로 엑세스 하는 방법이 있다.**

**일반 사용자의 경우 절대경로가 편하겠지만, 운영체제의 경우 inode가 훨씬 이해하기 편할 것이다.**

<br/>

#### inode

* 외부적으로는 번호로 표현하며, 내부적으로는 두 부분으로 나누어 정보를 저장한다.
* 첫 번째 부분은 파일의 정보를 저장하는 부분이다.
  * 파일의 종류, 접근 권한, 하드 링크 수, 소유자, 그룹, 파일 크기, 파일 변경 시각, 파일명 등을 저장한다.
  * ls -l 명령을 통해 inode의 정보를 확인할 수 있다.
* 두 번째 부분은 파일의 실제 데이터를 저장하고 있는 데이터 블록의 위치를 나타내는 주소들을 저장한다.
  * 파일의 inode 번호는 ls -i 명령으로 확인할 수 있다.

<br/>

#### 데이터 블록

* 실제로 데이터가 저장되는 하드 디스크의 공간이다.
* 일반 파일, 디렉토리, 심볼릭 링크는 데이터 블록에 관련 내용을 직접 저장한다.
* 장치 파일은 데이터 블록을 사용하지 않고 장치에 관한 정보를 inode에 저장한다.

<br/>

<br/>

# 03 파일 정보 검색

파일의 정보를 검색하려면 ls 명령을 사용한다.

<br/>

### 파일명으로 파일 정보 검색 : stat(2)

```c
#include <fcntl.h>
#include <sys/type.h>
#include < sys/stat.h>

int stat(const char *restrict path, struct stat *buf);
```

* path : 정보를 알고자 하는 파일명
* buf : 검색한 파일 정보를 저장할 구조체 주소

#### stat 함수의 특징

* 시스템 호출 함수이다. (2)
* 파일 정보를 검색하는데 가장 많이 사용하는 함수이다.
* path에 지정한 파일 정보를 검색해 buf로 지정한 구조체에 저장한다.
* stat 함수 사용 시, 파일에 대한 권한이 반드시 있어야 하는 것은 아니다.
* 하지만 파일에 이르는 경로의 각 디렉토리에 대한 읽기 권한은 필요하다.

<br/>

#### stat 구조체

* stat 함수로 검색한 inode 정보는 stat 구조체에 저장된다.
* stat 구조체는 <sys/stat.h> 파일에 저장되어 있다.

```c
struct stat {
	dev_t		st_dev;
	ino_t		st_ino;
	mode_t		st_mode;
	nlink_t		stnlink;
	uid_t		st_uid;
	gid_t		st_gid;
	dev_t		st_rdev;
	off_t		st_size;
	time_t		st_atime;
	time_t		st_mtime;
	time_t		st_ctime;
	blksize_t	st_blksize;
	blkcnt_t	st_blocks;
//	char		st_fstype[_ST_FSTYPSZ];  // 오래된 내용이라, 주석처리 하자. 컴파일 오류 발생한다.
};
```

* st_dev : inode가 저장되어 있는 장치의 장치 번호를 저장한다.
* st_ino : 해당 파일의 inode 번호를 저장한다.
* st_mode : 파일의 형식과 접근 권한을 저장한다.
* st_nlink : 하드 링크의 개수를 저장한다.
* st_uid : 파일 소유자의 UID를 저장한다.
* st_gid : 파일 소유 그룹의 GID를 저장한다.
* st_rdev : 장치 파일이면 주 장치 번호와 부 장치 번호를 저장한다. (장치 파일이 아니면 무의미)
* st_size : 파일의 크기를 의미한다.
* st_atime : 마지막으로 파일을 읽거나, 실행한 시각을 저장한다. 이때 시각은 1970년 1월 1일을 기준으로 이후의 시간을 초 단위로 저장한다.
* st_mtime : 마지막으로 파일의 내용을 변경(쓰기)한 시각을 저장한다.
* st_ctime : 마지막으로 inode의 내용을 변경한 시각을 저장한다. inode의 내용은 소유자/그룹 변경, 파일 크기 변경, 링크 개수 변경 등을 수행할 때 변경된다.
* st_blksize : 파일의 내용을 입출력할 때 사용하는 버퍼의 크기를 저장한다.
* st_blocks : 파일을 512바이트씩 블록으로 나눈 개수를 저장한다.
* st_fstype[_ST_FSTYPSZ] : 파일시스템 종류 정보를 저장한다. // 오래된 내용으로 지금은 안쓰인다. 컴파일 오류 발생

<br/>

#### 예제 3-1) 파일명으로 inode 정보 검색하기

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(void) {
    struct stat buf;

    stat("unix.txt", &buf); // stat를 이용해 unix.txt 파일 정보 읽어오기

    printf("Inode = %d\n", (int)buf.st_ino);
    printf("Mode = %o\n", (unsigned int)buf.st_mode);
    printf("Nlink = %o\n", (unsigned int)buf.st_nlink);
    printf("UID = %d\n", (int)buf.st_uid);
    printf("GID = %d\n", (int)buf.st_gid);
    printf("SIZE = %d\n", (int)buf.st_size);
    printf("Atime = %d\n", (int)buf.st_atime);
    printf("Mtime = %d\n", (int)buf.st_mtime);
    printf("Ctime = %d\n", (int)buf.st_ctime);
    printf("Blksize = %d\n", (int)buf.st_blksize);
    printf("Blocks = %d\n", (int)buf.st_blocks);
//  printf("FStype = %s\n", (int)buf.st_fstype);

    return 0;
}
```

```c
* 결과
# ex3_1
Inode = 0
Mode = 4702353420
Nlink = 15111341250
UID = 32641
GID = 0
SIZE = 0
Atime = 652699232
Mtime = 15775231
Ctime = 403122333
Blksize = 1970169159
Blocks = 9
```

<br/>

### 파일 기술자로 파일 정보 검색  : fstat(2)

```c
#include <sys/types.h>
#include <sys/stat.h>

int fstat(int fd, struct stat *buf);
```

* fd : 열려 있는 파일의 파일 기술자
* buf : 검색한 파일 정보를 저장할 구조체 주소

#### fstat 함수의 특징

* 시스템 호출 함수이다. (2)
* 파일 경로 대신 현재 열려있는 파일의 파일 기술자를 인자로 받아 파일 정보를 검색한 후, buf로 지정한 구조체에 저장한다.

<br/>

#### 예제 3-2) fstat으로 파일 정보 검색하기

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int fd;
    struct stat buf;
    
    fd = open("unix.txt", O_RDONLY); // 파일 기술자를 리턴 받는다.
    if (fd ==-1) {
        perror("open: unix.txt");
        exit(1);
    }   
    
   fstat(fd, &buf); // 파일 기술자를 인자로 buf에 파일 정보의 내용을 저장한다.
   
   printf("Inode = %d\n", (int)buf.st_ino);
   printf("UID = %d\n", (int)buf.st_uid);
   close(fd);
   
   return 0;
}  

```

```c
* 결과
# ex3_2
Inode = 399302
UID = 1000
```

<br/>

<br/>

# 04 파일 접근 권한 제어

st_mode 항목에는 파일 종류 및 접근 권한 정보가 저장된다. 앞선 예제에서 처럼, st_mode 값은 숫자로 출력된다.

<br/>

## 1_ st_mode 값의 구조

* st_mode 항목의 데이터형인 mode_t는 unsigned int 형태로 정의되어 있다.
* 이중 16비트를 사용한다.
* 다음과 같은 구조로 되어있다.
  * <img src="/home/sanggoe/.config/Typora/typora-user-images/image-20200425090007363.png" alt="image-20200425090007363" style="zoom:80%;" />

* <sys/stat.h> 파일에 정의된 상수와 매크로는 위 구조로 저장된 값과 상수를 AND 연산해서 값을 추출하는 것이다.

<br/>

## 2_ 파일의 종류 검색

파일 종류를 검색하는데 상수와 매크로를 이용 가능하다

<br/>

#### 상수를 이용한 파일 종류 검색

<sys/stat.h>에 파일의 종류 검색과 관련해 정의된 상수는 다음 표와 같다.

| 상수명   | 상수값(16진수) | 기능                                              |
| -------- | -------------- | ------------------------------------------------- |
| S_IFMT   | 0xF000         | st_mode 값에서 파일의 종류를 정의한 부분을 가져옴 |
| S_IFIFO  | 0x1000         | FIFO 파일                                         |
| S_IFCHR  | 0x2000         | 문자 장치 특수 파일                               |
| S_IFDIR  | 0x4000         | 디렉토리                                          |
| S_IFBLK  | 0x6000         | 블록 장치 특수 파일                               |
| S_IFREG  | 0x8000         | 일반 파일                                         |
| S_IFLNK  | 0xA000         | 심볼릭 링크 파일                                  |
| S_IFSOCK | 0xC000         | 소켓 파일                                         |

* S_IFMT 상수의 값이 0xF000이므로, st_mode의 값과 AND 연산을 진행하면 파일 종류 부분만 남는다.
* 이 값을  다른 상수들과 비교하면 해당 파일의 종류를 알 수 있다.

<br/>

#### 예제 3-3) 상수를 이용해 파일 종류 검색하기

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(void) {
    struct stat buf;
    int kind;

    stat("unix.txt", &buf);

    printf("Mode = %o (16진수: %x)\n", (unsigned int)buf.st_mode,
            (unsigned int)buf.st_mode);

    kind = buf.st_mode & S_IFMT; // st_mode와 S_IFMT를 AND 연산 수행
    printf("Kind = %x\n", kind);

    switch (kind) {
        case S_IFIFO:
            printf("unix.txt : FIFO\n");
            break;
        case S_IFDIR :
            printf("unix.txt : Directory\n");
            break;
        case S_IFREG:
            printf("unix.txt : Regular File\n");
            break;
    }       
    
    return 0;
}   
```

```c
# ex3_3
Mode = 100644 (16진수: 81a4)
Kind = 8000
unix.txt : Regular File
```

<br/>

#### 매크로를 이용한 파일 종류 검색

* <sys/stat.h> 파일에는 상수 외에 매크로도 정의되어 있다.

