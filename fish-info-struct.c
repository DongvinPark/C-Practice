#include <stdio.h>

struct exercise {
    const char* description;
    float duration;
};

struct meal {
    const char* ingredients;
    float weight;
};

struct preferences {
    struct meal food;
    struct exercise exercise;
};

struct fish {
    const char* name;
    const char* species;
    int teeth;
    int age;
    struct preferences care;
};

/*
C에서는 
printf(
        "name : %s\n
        species : %s\n
        teeth : %i\n
        age : %i\n",
        f.name,
        f.species,
        f.teeth,
        f.age
        );
이런 식으로 "" 내부를 작성하는 것이 허용되지 않는다.

줄바꿈이 된 상태로 출력하고 싶더라도,
printf("name : %s\nspecies : %s\nteeth : %i\nage : %i\n",
        f.name,
        f.species,
        f.teeth,
        f.age
        );
이런 식으로 줄바꿈은 오직 \n으로만 표현하여 한 줄 이내에 전부 담아내야 한다.

*/
void print_label(struct fish f){
    printf("name : %s\nspecies : %s\nteeth : %i\nage : %i\n",
        f.name,
        f.species,
        f.teeth,
        f.age
        );
    printf(
        "care : feed %2.2f pounds of %s and let %s in %2.2f hour\n",
        f.care.food.weight,
        f.care.food.ingredients,
        f.care.exercise.description,
        f.care.exercise.duration
        );
}

int main(){
    struct fish snappy = {
        "Snappy", "Firanya", 69, 4,
        {
            {"meat", 0.2},
            {"swim in bath", 7.5}
        }
    };

    print_label(snappy);

    return 0;
}