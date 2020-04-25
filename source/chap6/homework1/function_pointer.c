#include <stdio.h>

int add(int a, int b) { // return add two parameters function
    return a+b;
}
int sub(int a, int b) { // return sub two parameters function
    return a-b;
}
int mul(int a, int b) { // return mul two parameters function
    return a*b;
}
int div(int a, int b) { // return div two parameters function
    return a/b;
}

int main() {
    char func_Op;
    int num1, num2;
    
    // integer type return value, inteager type two parameters function pointer
    int (*fp)(int, int) = NULL;

    while(1) {
    	printf("intput the operation : ");
	scanf("%d %c %d", &num1, &func_Op, &num2);

    	switch (func_Op) {
	    case '+': fp = add;
	    	break;
	    case '-': fp = sub;
	    	break;
	    case '*': fp = mul;
	    	break;
	    case '/': fp = div;
	    	break;
	}

	printf("%d%c%d=%d\n", num1, func_Op, num2, fp(num1, num2));
    }
    return 0;
}

