#include <stdio.h>

void go_south_east(int* lat, int* lon){
    // *lat = *lat + 1;
    // *lon = *lon + 1; 아래의 코드와 똑같은 뜻이다.
    *lat += 1;
    *lon += 1;

/*
포인터는 '값'의 개념이 아니라 '공간 이동'의 개념으로 이해하는 것이 나에게 더 맞는 것 같다.

애초에 컴퓨터 입장에서 포인터를 쓰는 이유가,
현재 어떤 작업을 해야 하는데, 그걸 '어디에다가' 해줘야 하는 건지를 알아야 하기 때문이다.

그렇다면, int* lat 이 뜻하는 것은 무엇인가?
이것은 lat은 포인터 변수인데, 그 포인터가 가리키는 '공간'에는 int 타입 변수가 있다는 뜻이다.

결국, 포인터 변수 타입에 전달할 수 있는 값의 종류는 & 연산자를 통해서 얻어낸 '주소값'뿐이다.

최종적으로 
1. 주소값을 넘겨서 호출하면,
2. 역참조 연산자(*lat이다. int* len이 아니다!!)를 통해서 주소값이 가리키는 곳으로 (메모리)공간이동 한후,
3. 거기에다가 +1 이라는 행위를 하라는 뜻이다.
*/
}

int main(){
    int latitude = 32;
    int longitude = -64;

    go_south_east(&latitude, &longitude);

    printf("stop! cur positon : [%i, %i]\n", latitude, longitude);

    return 0;
}