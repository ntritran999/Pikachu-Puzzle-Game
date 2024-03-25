#include "block_logic.h"

void genRandoms(int *a, int size)
{
    // Assign two position at the same time with the same value
    // to make sure that there are pairs in the array.
    for (int i = 0; i < size; i += 2)
    {
        a[i] = rand() % 26; // get value of the alphabet from 0 -> 25.
        a[i + 1] = a[i];
    }
}

void shuffleRandoms(int *a, int size)
{
    for (int i = 0; i < size; i++)
    {
        // Choose a random position base on current position.
        int j = i + rand() % (size - i);
        
        // Swap two position.
        int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
}
