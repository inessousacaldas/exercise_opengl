#include "utils/random.h"

// std includes
#include <cstdlib>

bool Random::chance(int chance) 
{
    return (rand() % (100 - chance)) == 0;
}
