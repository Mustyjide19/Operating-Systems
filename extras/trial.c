#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main() {
    srand(time(0));
    sleep(1+(rand()%4));
    int result = rand() - (RAND_MAX / 2);
    printf("Result of the trial was: %d\n", result);
}