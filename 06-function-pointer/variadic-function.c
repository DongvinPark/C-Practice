#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int sum(int args_cnt, ...){
    int result = 0;

    va_list args;

    va_start(args, args_cnt);

    for(int i=0; i<args_cnt; i++){
        int num = va_arg(args, int);
        result += num;
    }

    va_end(args);

    return result;
}

int main(){

    printf("Sum : %i \n", sum(3, 1,2,3));
    printf("Sum : %i \n", sum(5, 1,2,3,4,5));

    return 0;
}