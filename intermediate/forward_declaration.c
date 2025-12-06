#include <stdio.h>

/*
    Forward declaration allows us to define an eventual declaration of
    struct, so it can reference itself.
*/

// typedef struct Node node_t;

// typedef struct Node {
//     int x;
//     int y;
//     node_t *next;
// } node_t;

/*
    We could also simplify like this
*/
typedef struct Node {
    int x;
    int y;
    struct Node * next;
} node_t;


/*
    We can also use forward definition to reference two structs
    with each other
*/

typedef struct Galaxy {
    char *name;
    struct Solar_System *solar_system;
} galaxy_t;

typedef struct Solar_System{
    char *name;
    struct Galaxy *galaxy;
} solar_system_t;

int main() {
    node_t a = {1, 2};
    node_t b = {3, 4};
    a.next = &b;

    node_t *curr = &a;

    while (curr != NULL) {
        printf("%d, %d\n", curr->x, curr->y);
        curr = curr->next;
    }
    
    galaxy_t galaxy = {"xyz"};
    solar_system_t sls = {"abc"};

    galaxy.solar_system = &sls;
    sls.galaxy = &galaxy;

    printf("galaxy: %s, solar system: %s\n", galaxy.name, galaxy.solar_system->name);
    printf("solar system: %s, galaxy: %s\n", sls.name, sls.galaxy->name);
    return 0;
}