#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int width;
    int height;
} rectangle;

/*
void* 타입 포인터는 어떤 타입의 포인터도 가리킬 수 있지만,
실제로 활용하기 위해서는 적절한 타입의 포인터로 형변환(cast)을 먼저 해줘야 한다.

따라서, compare 함수는
포인터 형변환 > 포인터로부터 값 추출 > 리턴 값 계산
의 순서로 작성돼 있는 경우가 대부분이다.
*/
int compare_int_asc(const void* before, const void* after){
    int bf = *(int*)before;
    int af = *(int*)after;

    return bf-af;
}

int compare_int_desc(const void* before, const void* after){
    int bf = *(int*)before;
    int af = *(int*)after;

    return af - bf;
}

int compare_area_asc(const void* before, const void* after){
    rectangle* bf = (rectangle*)before;
    rectangle* af = (rectangle*)after;

    int area_bf = bf->width * bf->height;
    int area_af = af->width * af->height;

    return area_bf - area_af;
}

int compare_area_desc(const void* before, const void* after){
    return (-1)*(compare_area_asc(before, after));
}

int compare_character_asc(const void* before, const void* after){
    char bf = *(char*)before;
    char af = *(char*)after;

    return bf-af;
}

int compare_character_desc(const void* before, const void* after){
    return compare_character_asc(before, after)*(-1);
}

int compare_string_case_sensitive_asc(const void* before, const void* after){
    // C에서 문자열은 char* 의 배열이라는 사실을 기억하라.
    char** bf = (char**)before;
    char** af = (char**)after;

    // strcmp는 실제 문자열을 받기 때문에, 문자열을 가리키는 포인터인 bf, af에다가
    // 한 번 더 역참조 연산(즉, * 이거) 을 해준 후 호출해야 한다.
    return strcmp(*bf, *af);
}

int compare_string_case_insensitive_asc(const void* before, const void* after) {
    // 포인터 형변환
    char** bf = (char**)before;
    char** af = (char**)after;

    // 문자열 추출
    char* str1 = *bf;
    char* str2 = *af;

    // stdlib.h 내부에는 문자열 자체를 받아서 바로 소문자로 변환된 문자열을 리턴해주는 함수가 없다.
    // 따라서 내가 일일이 원본 문자열을 순회하면서 새로운 소문자 문자열을 만들어줘야 한다.
    char lower_str1[strlen(str1) + 1];
    char lower_str2[strlen(str2) + 1];
    for (int i = 0; str1[i]; i++) {
        lower_str1[i] = tolower(str1[i]);
    }
    lower_str1[strlen(str1)] = '\0';

    for (int i = 0; str2[i]; i++) {
        lower_str2[i] = tolower(str2[i]);
    }
    lower_str2[strlen(str2)] = '\0';

    return strcmp(lower_str1, lower_str2);
}

int main(){

    int integer_arr[] = {18, 24, 874, 987, 547, 983, 98, 2, 7, 9, 8, 4, 2};
    char character_arr[] = {'c','a','d','b','f','e','g'};
    char* string_case_sensitive_arr[] = {"FOX", "What", "does", "say", "?!", "the"};
    rectangle rectangle_arr[] = {
        {1,34}, {39,48}, {29,54}, {90,3}, {927,8}
    };

    puts("integer asc");
    qsort(integer_arr, 13, sizeof(int), compare_int_asc);
    for(int i=0; i<13; i++){
        printf("%i, ", integer_arr[i]);
    }
    printf("\n");

    puts("integer desc");
    qsort(integer_arr, 13, sizeof(int), compare_int_desc);
    for(int i=0; i<13; i++){
        printf("%i, ", integer_arr[i]);
    }
    printf("\n");

    puts("character asc");
    qsort(character_arr, 7, sizeof(char), compare_character_asc);
    for(int i=0; i<7; i++){
        printf("%c, ", character_arr[i]);
    }
    printf("\n");

    puts("integer desc");
    qsort(character_arr, 7, sizeof(char), compare_character_desc);
    for(int i=0; i<7; i++){
        printf("%c, ", character_arr[i]);
    }
    printf("\n");

    puts("rectangle area asc");
    qsort(rectangle_arr, 5, sizeof(rectangle), compare_area_asc);
    for(int i=0; i<5; i++){
        printf("%i, ", rectangle_arr[i].width);
    }
    printf("\n");

    puts("rectangle area desc");
    qsort(rectangle_arr, 5, sizeof(rectangle), compare_area_desc);
    for(int i=0; i<5; i++){
        printf("%i, ", rectangle_arr[i].width);
    }
    printf("\n");

    puts("string case sensitive asc");
    qsort(string_case_sensitive_arr, 6, sizeof(char**), compare_string_case_sensitive_asc);
    for(int i=0; i<6; i++){
        printf("%s, ", string_case_sensitive_arr[i]);
    }
    printf("\n");

    puts("string case insensitive asc");
    qsort(string_case_sensitive_arr, 6, sizeof(char**), compare_string_case_insensitive_asc);
    for(int i=0; i<6; i++){
        printf("%s, ", string_case_sensitive_arr[i]);
    }
    printf("\n");

    return 0;
}