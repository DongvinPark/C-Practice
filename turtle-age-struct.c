#include <stdio.h>

typedef struct {
    const char* name;
    const char* species;
    int age;
} turtle;

/*
아래의 함수는 이 함수와 기능이 똑같다.

void happy_birthday(turtle* t){
    (*t).age = (*t).age + 1;
    printf("Happy birthday %s! Now you are %i years old :) \n", (*t).name, (*t).age);
}

(*t).name 이라고 쓰기 귀찮으니까 t->name 으로 쓰는 것.
t->name의 정확한 정의는 결국 (*t).name 이므로 잘 기억해두자.

*/
void happy_birthday(turtle* t){
    t->age = t->age + 1;
    printf("Happy birthday %s! Now you are %i years old :) \n", t->name, t->age);
}

int main(){
    turtle my_turtle = {"Kkobook", "Long Live Turtle", 99};
    happy_birthday(&my_turtle);
    printf("%s is now %i years old.\n", my_turtle.name, my_turtle.age);
    return 0;
}