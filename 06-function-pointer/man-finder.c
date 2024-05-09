#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sports_no_bieber(char* s){
    return strstr(s, "sports") && !strstr(s, "bieber");
}

int sports_or_workout(char* s){
    return strstr(s, "sports") || strstr(s, "exercise");
}

int no_somke_movie(char* s){
    return strstr(s, "no-smoke") && strstr(s, "movie");
}

int art_or_movie_or_dining(char* s){
    return strstr(s, "art") || strstr(s, "movie") || strstr(s, "restaurant");
}

// 함수포인터를 정의하는 형식을 잘 기억해두자.
// 반환 타임 (* 함수이름 )(인자 타입1, 인자 타입 2, ...)
// 와 같다.
void find( int(*match_filter)(char*), char** arr, int len ){
    puts(">> result:");
    puts("----------------------");
    for(int i=0; i<len; i++){
        if(match_filter(arr[i])){
            printf("%s\n", arr[i]);
        }
    }
    puts("----------------------\n");
}

int main(){
    int NUM_ADS = 7;

    char* ADS[] = {
        "Will: black, sports, TV, restaurant",
        "Matt: white, no-smoke, art, movie, musical",
        "Louise: latino, no-alcohol, book, musical, art",
        "Mike: white, devorced, alcohol, smoke, truct, sports, bieber",
        "Peter: asian, chess, exercise, art",
        "Josh: judish, sports, movie, musical",
        "Jed: devorced, musical, book, restaurant"
    };

    /*
    함수의 이름은 그 자체로 포인터다.
    따라서 

    이렇게 함수 이름만으로 호출하는 것과,
    find( sports_or_workout, ADS, NUM_ADS);

    이렇게 함수의 주소를 넘겨주는 것은
    find( &no_somke_movie, ADS, NUM_ADS);

    본질적으로 똑같고, 동작도 똑같다.

    함수포인터는 Call Back 이라는 기능을 구현하는데 필수적이다.
    Call Back의 근원이라고 볼 수 있다.
    */

    find(sports_no_bieber, ADS, NUM_ADS);
    find(sports_or_workout, ADS, NUM_ADS);
    find(&no_somke_movie, ADS, NUM_ADS);
    find(&art_or_movie_or_dining, ADS, NUM_ADS);

    return 0;
}