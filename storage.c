/*********************************************************************
* @file storage.c
* @brief stores up to 10 vectors
* @author Mac Patterson
* @date 9/30/2026
* @version 5.3
* just adding this line to "edit" code
*********************************************************************/

#include <string.h>
#include "vector.h"

static struct Vector vectors[MAX_VECTORS];

int addvect(struct Vector vector)
{
    int i;
    int location = -1;
    int empty = -1;
    int success = 0;
    for (i = 0; i < MAX_VECTORS; i++)
    {
        if (strcmp(vectors[i].name, vector.name) == 0)
        {
            location = i;
        }

        if (vectors[i].name[0] == '\0' && empty == -1)
        {
            empty = i;
        }
    }
    if (location == -1)
    {
        location = empty;
    }
    if (location != -1)
    {
        vectors[location] = vector;
        success = 1;
    }

    return success;
}

int findvect(const char *name, struct Vector *vector)
{
    int i;
    int found = 0;
    for (i = 0; i < MAX_VECTORS; i++)
    {
        if (vectors[i].name[0] != '\0' &&
            strcmp(vectors[i].name, name) == 0)
        {
            *vector = vectors[i];
            found = 1;
        }
    }

    return found;
}

int getvect(int index, struct Vector *vector)
{
    int found = 0;
    if (index >= 0 && index < MAX_VECTORS)
    {
        if (vectors[index].name[0] != '\0')
        {
            *vector = vectors[index];
            found = 1;
        }
    }

    return found;
}

void clear(void)
{
    int i;
    int j;
    for (i = 0; i < MAX_VECTORS; i++)
    {
        vectors[i].name[0] = '\0';
        for (j = 0; j < 3; j++)
        {
            vectors[i].values[j] = 0.0;
        }
    }
}