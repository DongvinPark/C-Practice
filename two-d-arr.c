#include <stdio.h>

/*
이건 잘못된 버전이다.
배열의 이름에다가 sizeof() 해봤자 배열의 0 index 요소의 포인터 값의 길이가 나올뿐,
실제 배열의 요소의 개수는 나오지 않기 때문이다.

C에서는 배열만 가지고서는 해당 배열이 char 배열이 아닌 이상 그 배열의 끝이 어디인지를 알 수 없다.
그래서 2차 배열을 순회하기 위해서는 결국 해당 2차 배열 내부에 존재하는 1차 배열의 개수를 수동으로
설정해줘야 한다.

void print_strings(char* arr[]){

    int elem_len = sizeof(arr);

    for(int i=0; i<elem_len; i++){

        char* elem = arr[i];

        int print_len = sizeof(elem);
        for(int j=0; j<print_len; j++){
            printf("%c", *(elem + j));
        }
        printf("\n");
    }
}

올바른 버전은 바로 아래와 같다.
*/

// 넘겨 받은 2차 배열의 길이를 같이 넘겨 받아야 한다.
void print_strings_in_2_d_arr(char arr[][10], int len){
    for(int i=0; i<len; i++){
        char* elem = arr[i];

        // elem 은 char 타입 포인터이므로, 출력을 위한 실제 내용물을 얻기 위해서는
        // *elem과 같이 역참조 연산을 해야 한다.
        while(*elem != '\0'){
            printf("%c", *elem);

            // 다음에 읽어야 하는 곳으로 포인터를 이동시켜야 한다. 포인터에다가 ++ 등의
            // 연산을 해주는 것이 바로 포인터 연산이다.
            elem++;
        }

        printf("\n");
    }
}


// 아래의 두 함수는 사실상 똑같다. 어떤 걸로 호출해도 상관이 없다.
// 단, char arr[][] 같은 2차 배열은 안 된다.
// 따라서, 2차 배열보다는 포인터의 배열 또는 2차 포인터가 훨씬 호환도 잘 되고 
// C 스타일에 맞게 직관적임을 알 수 있다.
void print_strings_in_char_ptr_arr(char* arr[], int len){
    for(int i=0; i<len; i++){
        char* elem = arr[i];

        while(*elem != '\0'){
            printf("%c", *elem);
            elem++;
        }

        printf("\n");
    }
}
void print_strings_in_char_ptr_arr_receiving_2_d_ptr_parameter(char** arr, int len){
    for(int i=0; i<len; i++){
        char* elem = arr[i];

        while(*elem != '\0'){
            printf("%c", *elem);
            elem++;
        }

        printf("\n");
    }
}

int main(){
    int two_d_arr_len = 3;

    char arr_arr[][10] = {
        "abcd", "ef", "ghi"
    };

    print_strings_in_2_d_arr(arr_arr, two_d_arr_len);
    printf("\n");


    char* char_ptr_arr[] = {
        "abcde", "f", "ghi"
    };

    print_strings_in_char_ptr_arr(char_ptr_arr, two_d_arr_len);
    printf("\n");

    print_strings_in_char_ptr_arr_receiving_2_d_ptr_parameter(char_ptr_arr, two_d_arr_len);

    return 0;
}