#include "tools/utilFunctions.h"


// Calcula un numero al azar entre min y max.
// Necesita que se ejecute "srand(time(NULL));" previamente.
int getRandomBetween(int min, int max) {
   return min + (rand() % static_cast<int>(max - min + 1));
}
