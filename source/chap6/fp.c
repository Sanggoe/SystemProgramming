#include <stdio.h>

void print() {
    printf("Hello workld, print() excuse\n");
}

void print1() {
    printf("another function excute\n");
}

int print2(char *str) {
    printf("Second : %s\n", str);
    return 0;
}

int main() {
    void (*p)() ; // 함수 포인터 변수 선언
    int (*p1) (char *) ;
    int arr[10] = {1,2,3,4,5,6};
    char str[5][20] = {"Hello", "World!! Good", "Morning"};
    char * ptr;

    ptr = str[0];
    ptr = str[2];

    printf("int array : %p\n, string array : %p - %p - %p\n", arr, str, str[0], &str[0][0]);

    //print();
    p = print; // print 함수의 시작주소를 저장
    p();
    p = print1; // print1 함수의 시작주소를 저
    p();
    
    p1 = print2;
    p1(str[0]);

    return 0;
}
