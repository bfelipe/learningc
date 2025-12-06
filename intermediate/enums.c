#include <stdio.h>

/*
    Enums is a list of integers
    constrained to a new type, where
    each is a given name.
*/

typedef enum Planets {
    MERCURY,
    VENUS,
    EARTH,
    MARS,
    JUPYTER,
    SATURN,
    URANUS,
    NEPTUNE,
    PLUTO,
} planets_t;

/*
    You can also provide non-default values to enums.
*/

enum Galaxy {
    MILKWAY = 123,
    ANDROMEDA = 789,
} ;

/*
    You can also provide the first value of the enum,
    then the compiler will fill the remain;
*/

enum Constelation {
    AQUILA = 207,
    SCUTUM,
    SERPENS_CAUDA,
    OPHIUCHUS,
    SCORPIUS,
    CORONA_AUSTRALIS,
    TELESCOPIUM,
    INDUS,
    MICROSCOPIUM,
    CAPRICORNUS,
    SAGITTARIUS,
};

int main() {
    planets_t earth =  EARTH;
    printf("%d\n", earth);
    enum Galaxy milkway = MILKWAY;
    printf("%d\n", milkway);
    enum Constelation sagittarius = SAGITTARIUS;
    printf("%d\n", sagittarius);
    
    /*
        sizeof also works on enums.
        As structs, enum size is the size of an int: 4 bytes.

        However in case you are giving an enum a really big size, the compiler
        will try to acomodate that number using a bigger type: unsigned int, long
    */

    printf("%zu bytes\n", sizeof(enum Galaxy));
    return 0;
}