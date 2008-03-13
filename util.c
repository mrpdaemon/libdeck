#include <stdlib.h>
#include <time.h>

void
LibDeck_InitRandom(void)
{
   srand(time(NULL));
}

int
LibDeck_Random(int modulus)
{
   return random() % modulus;
}
