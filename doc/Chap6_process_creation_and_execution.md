# 01 개요

##### 유닉스에서 프로세스 생성

* 사용자가 명령행에서 직접 프로그램을 실행해 생성
* 프로그램 안에서 다른 프로그램을 실행해 생성
* system, fork, vfork 함수 사용

<br/>

##### 유닉스에서 프로세스 종료

* 해당 프로세스가 어떻게 종료되었는지 상태 정보를 저장
* 자식 프로세스는 부모 프로세스에게 자신이 어떻게 종료되었는지 상태값을 리턴 가능
* 종료 상태값이 0이면 정상 종료
* 종료 상태값이 0이 아니면 오류 발생
* exit, atexit, _exit 함수 사용

<br/>

##### 'exec'로 시작하는 함수군

* 인자로 받은 다른 프로그램을 자신을 호출한 프로세스의 메모리에 덮어 쓴다.
* 기존 프로그램은 중지, 새로 덮어쓴 프로그램 실행
* fork 함수와 연결, 생성한 자식 프로세스가 새로운 프로그램 실행 시에도 사용

<br/>

##### 프로세스 동기화

* fork 함수로 자식 프로세스 생성 시, 부모와 자식 프로세스는 순서 관계 없이 실행
* 먼저 실행 마친 프로세스는 종료
* 종비 프로세스 등의 불안정 상태 프로세스 발생
* wait, waitpid 함수 등으로 동기화 필요

<br/>

<br/>

# 02 프로세스 생성

* 프로세스 : 실행 가능한 프로그램이 현재 실행중인 상태

* 프로세스를 생성하는 경우
  * 사용자가 명령행에서 직접 프로그램을 실행해 생성하는 경우
  * 프로그램 안에서 다른 프로그램을 실행해 생성하는 경우
    * ex) 업무용 소프트웨어 보고서 작성을 위해 출력 프로그램을 실행
    * ex) 인터넷을 통해 서비스 제공을 위해 다른 서비스 프로그램을 실행

<br/>

## 1_ 간단한 방법

* system 함수를 사용하는 방법
* 단, 명령 실행을 위해 쉘까지 동작시키므로 비효율적
* system 함수 남용은 바람직하지 않음

<br/>

### 프로그램 실행 : system(3)

```c
#include <stdlib.h>

int system(const char *string);
```

* string : 실행할 명령이나 실행 파일명

#### system 함수의 특징

* 기존 명령이나 실행 파일명을 인자로 받아 쉘에 전달한다.
* 쉘은 내부적으로 새 프로세스를 생성해 인자로 받은 명령을 실행한다.
* 해당 명령 실행이 끝날 때까지 기다렸다가 종료 상태를 리턴한다.

<br/>

#### 예제 6-1) system 함수 사용하기

```c
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int a;
    a = system("ps -ef | grep han > han.txt");
    // 명령은 현재 실행중인 프로세스들에서 'han'을 포함한 내용을 찾아 han.txt 파일에 저장
    printf("Return Value : %d\n", a);
    
    return 0;
}
```

```c
* 결과
# ex6_1
Return Value : 0

# cat han.txt
sanggoe    3271   3269  0 15:23 pts/1    00:00:00 grep han
sanggoe    3269   3268  0 15:23 pts/1    00:00:00 sh -c ps -ef | grep han > han.txt
// 본 쉘에서 -c 옵션은 문자열에서 명령을 읽어오라는 의미
```

<br/>

## 2_ 프로세스 생성

* 대표적인 방법은 fork 함수를 사용하는 것
* fork 함수는 새로운 프로세스를 생성한다.
* fork 함수가 생성한 프로세스를 자식 프로세스라고 한다. 
* fork 함수를 호출한 프로세스를 부모 프로세스라고 한다.
* fork 함수 리턴 시, 부모 프로세스와 자식 프로세스가 동시에 동작한다.
* 어느 프로세스가 먼저 실행 될지는 알 수 없다.
* 운영체제의 스케줄링에 따라 처리 순서가 달라진다.

![image-20200427153758796](/home/sanggoe/.config/Typora/typora-user-images/image-20200427153758796.png)

<br/>

#### fork 함수의 수행 과정

1. fork 함수를 호출하면
2. 새로운 자식 프로세스를 생성한다.
3. fork 함수로 생성한 자식 프로세스의 메모리 공간은 부모 프로세스의 메모리공간을 그대로 복사해 만든다.
4. fork 함수는 부모 프로세스에는 자식 프로세스의 PID를 리턴, 자식 프로세스에는 0을 리턴한다.

<br/>

* 자식 프로세스가 부모 프로세스에게 상속 받는 속성
  * 실제 사용자 ID, 유효 사용자 ID, 실제 그룹 ID, 유효 그룹ID
  * 환경 변수
  * 열린 파일 기술자
  * 시그널 처리 설정
  * setuid, setgid 설정
  * 현재 작업 디렉토리
  * umask 설정값
  * 사용 가능한 자원 제한

<br/>

* 자식 프로세스가 부모 프로세스와 다른 점
  * 유일한 프로세스 ID를 갖는다.
  * 자식 프로세스는 부모 프로세스의 PPID와 다른 자신만의 PPID를 갖는다. 즉, 자식 프로세스의 PPID는 자식을 호출한 부모 프로세스이다.
  * 자식 프로세스는 부모 프로세스가 연 파일 기술자 사본을 갖고 있다. 즉, 부모와 자식 프로세스는 같은 파일의 오프셋을 공유하고 있는 상태이므로 읽고 쓸 때 주의해야 한다.
  * 프로세스 잠금, 파일 잠금, 기타 메모리 잠금 등은 상속하지 않는다.
  * tms 구조체 값은 0으로 초기화 된다. 즉, 프로세스 실행 시간을 측정하는 기준값이 새로 설정된다.

<br/>

### 프로세스 생성 : fork(2)

```c
#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);
```

#### fork 함수의 특징

* 인자를 받지 않는다.
* 수행 성공 시, 부모 프로세스에는 자식 프로세스 ID를, 자식 프로세스에는 0을 리턴한다.
* 실패하면 -1을 리턴한다.

<br/>

#### 예제 6-2 fork 함수 사용하기

```c
int main(void) {
    pid_t pid;

    switch (pid = fork()) { // fork 함수 호출
        case -1 : /* fork failed*/
            perror("fork");
            exit(1);
            break;
        case 0 : /* child process */
            printf("Child process - My PID:%d, My Parent's PID:%d\n",
                    (int)getpid(), (int)getppid());
            break;
        default : /* parent process */
            printf("Parent process - My PID:%d, My Parent's PID:%d, "
                    "My Child's PID:%d\n", (int)getpid(), (int)getppid(), (int)pid);
            break;
    }

    printf("End of fork\n");

    return 0;
}
```

```c
* 결과
# ex6_2
Parent process - My PID:3452, My Parent's PID:3140, My Child's PID:3453
End of fork
Child process - My PID:3453, My Parent's PID:3452
End of fork
```

<br/>

### 프로세스 생성 : vfork(2)

```c
#include <unistd.h>

pid_t vfork(void);
```

#### vfork 함수의 특징

* fork 함수처럼 새로운 프로세스를 생성한다.
* 부모 프로세스의 메모리 공간을 모두 복사하지는 않는다.
* 이 함수로 생성한 자식 프로세스는 exec 함수군을 사용해 새로운 작업을 수행할 때 효율적이다.
* 표준이 아니므로 시스템에서 지원하는지 확인 후 사용해야 한다.

<br/>

<br/>

# 03 프로세스 종료

* 유닉스는 프로세스가 종료되면 어떻게 종료되었는지 상태(exit status)를 저장한다.
* 부모 프로세스는 종료 상태 정보를 사용해 자식 프로세스가 어떻게 종료되었는지 알 수 있다.
* 자식 프로세스는 종료 상태값을 리턴할 수 있다.
* 종료 상태값이 0이면 정상 종료를 의미한다.
  * exit 함수를 호출했거나 main 함수에서 0을 리턴
* 종료 상태값이 0이 아니면 오류가 발생했음을 의미한다.
  * 보통 오류 발생 시 1을 리턴

<br/>

## 1_ 프로세스 종료 함수

<br/>

### 프로그램 종료 : exit(2)

```c
#include <stdlib.h>

void exit(int status);
```

* status : 종료 상태값

#### exit 함수의 특징

* 프로세스를 종료시키고 부모 프로세스에 종료 상태값을 전달한다.
* 이때 atexit 함수로 예약한 함수들을 지정된 순서와 역순으로 모두 실행한다.
* 예약한 함수가 실행 도중 문제가 생겨 리턴하지 못할 경우, exit 함수의 나머지 과정도 수행되지 않는다.
* 프로세스가 사용중이던 모든 표준 입출력 스트림에 데이터가 남아있으면 이를 기록하고, 열려있는 스트림을 모두 닫는다.
* tmpfile 함수로 생성한 임시 파일을 모두 삭제한다.
* 시스템 호출인 _exit 함수를 호출한다.
* exit 함수는 C 표준 함수라서 시스템에 독립적인 기능만 수행한다.

<br/>

### 프로그램 종료 시 수행할 작업 예약 : atexit(3)

```c
#include <stdlib.h>

int atexit(void (*func)(void));
```

* func : 종료 시 수행할 작업을 지정한 함수명

#### atexit 함수의 특징

* 프로세스가 종료할 때 수행할 기능을 예약한다.
* 인자로 함수의 포인터를 받는다.
* 지정되는 함수는 인자와 리턴값이 없는 함수여야 한다.
* 예약할 수 있는 함수의 개수는 여유 메모리에 따라 달라진다.
* sysconf 함수의 _SC_ATEXIT_MAX 항목으로 검색할 수 있다.

<br/>

#### 예제 6-3) exit와 atexit 함수 사용하기

```c
#include <stdlib.h>
#include <stdio.h>

void cleanup1(void) {
    printf("Cleannup 1 is called.\n"); // 종료 시 반드시 수행해야할 작업을 지정해야 한다.
}   

void cleanup2(void) {
    printf("Cleannup 2 is called.\n"); // 로그 저장, 관리자에게 메시지 전송, 데이터 저장 등
}

int main(void) {
    atexit(cleanup1);
    atexit(cleanup2);

    exit(0); // 리턴값이 0이면 정상 종료
}
```

```c
* 결과
# ex6_3
Cleannup 2 is called.
Cleannup 1 is called. // 예약된 순서와 반대로 호출되었음을 알 수 있다.
```

<br/>

### 프로그램 종료 : _exit(2)

```c
#include <unistd.h>

void _exit(int status);
```

* status : 종료 상태값

#### _exit 함수의 특징

* 일반적으로 프로그램에서 직접 사용하지 않는다.
* exit 함수에서 내부적으로 호출한다.
* 시스템 호출 함수이다.
* 프로세스를 종료할 때 다음 과정을 통해 시스템 관련 자원을 정리한다.
  1. 모든 파일 기술자를 닫는다.
  2. 부모 프로세스에 종료 상태를 알린다.
  3. 자식 프로세스들에 SIGHUB 시그널을 보낸다.
  4. 부모 프로세스에 SIGHLD 시그널을 보낸다.
  5. 프로세스 간 통신에 사용한 자원을 반납한다.

<br/>

<br/>

# 04 exec 함수군 활용

* 함수 이름이 exec로 시작하는 함수들을 exec 함수군이라 한다.
* 명령이나 실행 파일을 실행할 수 있다.
* 인자로 받은 다른 프로그램을 자신을 호출한 프로세스의 메모리에 덮어쓴다.
* 프로세스가 수행 중이던 기존 프로그램은 중지되어 없어지고 새로 덮어쓴 프로그램이 실행된다.
* exec 함수군을 호출한 프로세스 자체가 바뀌므로, exec 함수 호출에 성공하면 리턴값이 없다.
* fork 함수로 생성한 자식 프로세스가 새로운 프로그램을 실행하도록 할 때 유용하다.

<br/>

## 1_ exec 함수군의 함수 형태

함수군은 다음 6가지 형태가 있다. 형태에 따른 지정 인자가 다르므로 주의해야 한다.

```c
#include < unistd.h>

int execl(const char *path, const char *arg0, ..., const char * argn, (char *)0);

int execv(const char *path, char *const argv[]);

int execle(const char *path, const char *arg0, ..., const char * argn, (char *)0, char *const envp[]);

int execve(const char *path, char *const argv[], char *const envp[]);

int execlp(const char *file, const char *arg0, ..., const char * argn, (char *)0);

int execvp(const char *file, char *const argv[]);
```

* exec 함수군은 path나 file에 지정한 명령이나 실행 파일을 실행한다.
* arg나 envp로 시작하는 인자를 path나 file에 지정한 파일의 main 함수에 전달한다.
* **자식이 부모와 다른 일을 수행했으면 해서 별도의 실행파일로 기능을 만들어 path로 전달 **
* exec**l***의 경우, 인자를 나열해서 전달
* exec**v***의 경우, 인자를 배열로 전달
* exec***e**의 경우, 인자에 환경 변수까지 전달
* exec***p**의 경우, 인자에 경로 대신 파일명으로 전달

<br/>

#### 각 함수별 기능

* execl
  * path에 지정한 경로명의 파일을 실행
  * arg0 ~ argn 을 나열해서 인자로 전달
  * 관례적으로 arg0에는 실행 파일명을 지정
  * 마지막 인자로 NULL 포인터((char *)0)를 지정해야 함
  * path에 지정하는 경로명은 절대 경로, 상대 경로 모두 사용 가능
* execv
  * path에 지정한 경로명의 파일을 실행
  * argv를 포인터 배열 인자로 전달
  * 배열 마지막에는 NULL 문자열을 저장해야 함
* execle
  * path에 지정한 경로명의 파일을 실행
  * arg0 ~ argn을 나열한것 + envp(환경변수)를 인자로 전달
  * 나열한 마지막 인자로 NULL 포인터((char *)0)를 지정해야 함
  * envp는 포인터 배열이며, 배열 마지막에는 NULL 문자열을 저장해야 함
* execve
  * path에 지정한 경로명의 파일을 실행
  * argv, envp를 포인터 배열 인자로 전달
  * 각 배열 마지막에는 NULL 문자열을 저장해야 함
* execlp
  * file에 지정한 파일을 실행
  * arg0 ~ argn 을 나열하여 인자로 전달
  * 파일은 프로세스와 동일한 경로에서 찾는다.
  * 나열한 마지막 인자로 NULL 포인터((char *)0)를 지정해야 함
* execvp
  * file에 지정한 파일을 실행
  * argv를 포인터 배열 인자로 전달
  * 배열 마지막에는 NULL 문자열을 저장해야 함

<br/>

## 2_ exec 함수군의 활용

<br/>

#### 예제 6-4) execlp 함수 사용하기

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    printf ("Before \n");
    if (execlp("ls", "ls", "-a", (char *)NULL) == -1) { 
        // exec 계열의 함수를 만나 프로세스 이미지가 ls 명령의 이미지로 바뀌어 이후 코드는 수행되지 않음
        perror("execlp error");
        exit(1);
    }   
    printf("After \n");
    return 0;
}
```

```c
* 결과
# ex6_4
Before 
.   .ex6_4.c.swp  ex6_1    ex6_2    ex6_3    ex6_4    fp    han.txt
..  .fp.c.swp	  ex6_1.c  ex6_2.c  ex6_3.c  ex6_4.c  fp.c  homework1
```

<br/>

#### 예제 6-5) execv 함수 사용하기

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    char *argv[3]; // 포인터 배열 선언

    printf("Before exec function\n");

    argv[0] = "ls";
    argv[1] = "-a";
    argv[2] = NULL;
    if (execv("/bin/ls", argv) == -1) { // execv의 인자로 포인터 배열 ergv를 지정
        perror("execv");
        exit(1);
    }   
    
    printf("After exec function\n");
    
    return 0;
} 
```

```c
* 결과
# ex6_5
Before exec function  // ls -a 명령 결과를 수행
.   .ex6_5.c.swp  ex6_1    ex6_2    ex6_3    ex6_4    ex6_5    fp    han.txt
..  .fp.c.swp	  ex6_1.c  ex6_2.c  ex6_3.c  ex6_4.c  ex6_5.c  fp.c  homework1
```

<br/>

#### 예제 6-6) (1) execve 함수 사용하기

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    char *argv[3];
    char *envp[2];
    
    printf("Before exec function\n");

    argv[0] = "arg.out";
    argv[1] = "100";
    argv[2] = NULL;

    envp[0] = "MYENV=hanbit";
    envp[1] = NULL;
    
    if (execve("./arg.out", argv, envp) == -1) {
	perror("execve");
	exit(1);
    }

    printf("After exec function\n");

    return 0;
}
```

#### 예제 6-6) (2) ex6_6_arg.c 파일

```c
#include <stdio.h>

int main(int argc, char **argv, char **envp) {
   int n;
   char **env;

   printf("\n--> In ex6_6_arg.c Main\n");
   printf("argc = %d\n", argc);
   for (n = 0; n < argc; n++) 
       printf("argv[%d] = %s\n", n, argv[n]);

   env = envp;
   while (*env) {
       printf("%s\n", *env);
       env++;
   }
   
   return 0;
} 
```

```c
* 결과
# gcc -o arg.out ex6_6_arg.c
# gcc -o ex6_6 ex6_6.c
# ex6_6 
Before exec function

--> In ex6_6_arg.c Main
argc = 2
argv[0] = arg.out
argv[1] = 100
MYENV=hanbit
```

<br/>

## 3_ exec 함수군과 fork 함수

* 앞선 fork 함수 예제에서는, 자식 프로세스에서도 부모 프로세스와 같은 코드를 수행했다.

* 물론, case로 구분하긴 했지만 기본적으로 같은 프로그램을 수행한다.
* exec 함수군을 호출하면 자식은 부모 프로세스를 복사한 프로그램과 다른 명령이나 프로그램을 실행 할 수 있다.
* 부모와 자식 프로세스가 각자 다른 작업을 수행해야 할 때 fork와 exec 함수를 함께 사용해야 한다.

<br/>

#### 예제 6-7) fork와 exec 함수 사용하기

```c
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    pid_t pid;
    
    switch (pid = fork()) {
        case -1 : /* fork failed */ 
            perror("fork");
            exit(1);
            break;
        case 0 : /* child process */
            printf("--> Child Process\n");
            if (execlp("ls", "ls", "-a", (char*)NULL) == -1) {
                perror("execlp");
                exit(1);
            }
            exit(0);
            break;
        default : /* parent process */
            printf("--> Parent process - My PID:%d\n", (int)getpid());
            break;
    }       
    
    return 0;
}
```

```c
* 결과
# ex6_7 
--> Parent process - My PID:4674
--> Child Process
 .	      .fp.c.swp  ex6_1.c  ex6_3    ex6_4.c  ex6_6	 ex6_7	  fp.c
..	      arg.out	 ex6_2	  ex6_3.c  ex6_5    ex6_6.c	 ex6_7.c  han.txt
.ex6_7.c.swp  ex6_1	 ex6_2.c  ex6_4    ex6_5.c  ex6_6_arg.c  fp	  homework1
```

<br/>

<br/>

# 05 프로세스 동기화

<br/>

* 동기화는, 일의 우선 순위를 정하는 것이라고도 표현할 수 있다.

* bash(shell) 가 운영체제와 사용자 간의 인터페이스 역할을 해주는 부모 프로세스이다.
* 명령어를 입력하면 부모 프로세스인 쉘이 명령어에 해당하는 자식 프로세스를 만들어 실행시켜주는것

<br/>

#### 프로세스의 종료 절차

* 자식 프로세스가 종료를 위해 부모 프로세스에 종료 상태 정보를 보낸다.
* 부모 프로세스는 이 정보로 프로세스 테이블에서 자식 프로세스를 삭제한다.
* 그러나 부모와 자식 프로세스는 순서 관계 없이 실행되고, 먼저 실행을 마치면 종료한다.
* 부모와 자식 프로세스 사이에 종료 절차가 제대로 진행되지 않으면 좀비 프로세스가 발생한다.

<br/>

## 1_ 좀비 프로세스

* 실행 종료 후 자원을 반납한 자식 프로세스의 종료 상태 정보를 부모 프로세스가 가져가지 않을 때 발생
* 좀비 프로세스는 프로세스 테이블에만 존재
* 일반적인 제거 방법은 없음
* 좀비 프로세스 방지를 위해 부모와 자식 프로세스를 동기화 해야함

#### 고아 프로세스

* 자식 프로세스보다 부모 프로세스가 먼저 종료할 경우 자식 프로세스들은 고아 프로세스가 됨
* 고아 프로세스는 init(PID 1) 프로세스의 자식 프로세스로 등록

<br/>

## 2_ 프로세스 동기화

부모와 자식 프로세스 동기화를 위해서는, 자식 프로세스가 종료 할 때까지 부모 프로세스가 기다리는 방법이 있다.

<br/>

### 프로세스 동기화 : wait(3)

```c
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *stat_loc);
```

* stat_loc : 상태 정보를 저장할 주소

#### wait 함수의 특징

* 자식 프로세스가 종료할 때까지 부모 프로세스를 기다리게 한다.
* 자식 프로세스 종료 상태는 stat_loc 에 지정한 주소에 저장된다.
* stat_loc에 NULL도 가능하다.
* 부모 프로세스가 wait 함수 호출 전에 자식이 종료하면 wait 함수는 즉시 리턴한다.
* wait 함수의 리턴값은 자식의 ID 이다.
* 리턴값이 -1이면 살아있는 자식이 하나도 없다는 의미이다.
* 단, 자식 프로세스가 여러 개일 경우 아무 자식 프로세스나 종료하면 리턴한다.

<br/>

#### 예제 6-8) wait 함수 사용하기

```c
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int status;
    pid_t pid;

    switch (pid = fork()) {
        case -1 : /* fork failed */
            perror("fork");
            exit(1);
            break;
        case 0 :  /* child process */
            printf("--> Child Process\n");
            exit(2);
            break;
        default : /* parent process */
            while(wait(&status) != pid) // wait() 만 동작시켜도 상관 없다
                continue;
            printf("--> parent process\n");
            printf("status: %d, %x\n", status, status);
            printf("child process Exit Status:%d\n", status >> 8);
            break;
    }
    
    return 0;
}
```

```c
* 결과
# ex6_8 
--> Child Process
--> parent process
status: 512, 200
child process Exit Status:2
```

<br/>

### 특정 자식 프로세스와 동기화 : waitpid(3)

```c
#include <sys/types.h>
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int * stat_loc, int options);
```

* pid : 종료를 기다리는 PID
* stat_loc : 종료 상태값을 저장할 주소
* options : waitpid 함수의 리턴 조건

#### waitpid 함수의 특징

* wait 함수와 다르게, 특정 PID로 지정한 자식 프로세스가 종료하기를 기다린다.
* 자식 프로세스의 종료 상태값을 stat_loc에 저장하고, options의 조건에 따라 리턴한다.
* 첫 번째 인자인 pid에 지정할 수 있는 값과 의미
  * pid < -1 : pid의 절댓값과 같은 프로세스 그룹 ID에 속한 자식 프로세스 중 임의의 프로세스의 상태값을 요청한다.
  * pid == -1 : wait 함수처럼 임의의 자식 프로세스의 상태값을 요청한다.
  * pid == 0 : 함수를 호출한 프로세스와 같은 프로세스 그룹에 속한 임의의 프로세스의 상태값을 요청한다.
  * pid > 0 : 지정한 pid에 해당하는 프로세스의 상태값을 요청한다.
* 세 번째 인자인 options에 지정 가능한 값은 <sys/wait.h>에 정의되어 있고, OR 연산으로 연결해 지정 가능하다.
  * WCONTINUED : 수행중인 자식 프로세스의 상태값이 리턴된다.
  * WNOHANG : pid로 지정한 자식 프로세스의 상태값을 즉시 리턴받을 수 없어도 이를 호출한 프로세스의 실행을 블록하지 않고 다른 작업을 수행하게 한다.
  * WNOWAIT : 상태값을 리턴한 프로세스가 대기 상태로 머물 수 있도록 한다.
  * WUNTRACED : 실행을 중단한 자식 프로세스의 상태값을 리턴한다. 실행이 중단되었으므로 더이상 상태값 리턴하지 않는다.

<br/>

#### 예제 6-9) waitpid 함수 사용하기

```c
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    int status;
    pid_t pid;

    if ((pid = fork()) < 0) { /* fork failed */
        perror("fork");
        exit(1);
    }

    if (pid == 0) { /* child process */
        printf("--> Child process\n");
        sleep(3);
        exit(3);
    }

    printf("--> Parent process\n");

    while (waitpid(pid, &status, WNOHANG) == 0) {
        // WNOHANG 옵션. waitpid 함수 블록되지 않고 아래 문장들 지속 수행
        printf("Parent still wait...\n");
        sleep(1);
    }

    printf("Child Exit Status : %d\n", status>>8);

    return 0;
}
```

```c
* 결과
# ex6_9
--> Parent process
Parent still wait...
--> Child process
Parent still wait...
Parent still wait...
Child Exit Status : 3
```

