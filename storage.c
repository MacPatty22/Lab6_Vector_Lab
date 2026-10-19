/*********************************************************************
* @file storage.c
* @brief stores vectors in a dynamically allocated array
* @author Mac Patterson
* @date 10/13/2026
* @version 5.3
*********************************************************************/

#include <stdlib.h>
#include <string.h>
#include "vector.h"

#define INITIAL_CAPACITY 4

static struct Vector *vectors = NULL;
static int vector_count = 0;
static int capacity = 0;

int addvect(struct Vector vector)
{
    struct Vector *larger;
    int new_capacity;
    int i;
    int location = -1;
    int success = 0;
    for (i = 0; i < vector_count; i++)
    {
        if (strcmp(vectors[i].name, vector.name) == 0)
        {
            location = i;
        }
    }
    if (location == -1)
    {
        if (vector_count == capacity)
        {
            new_capacity = capacity == 0 ? INITIAL_CAPACITY : capacity * 2;
            larger = realloc(vectors,
                             (size_t)new_capacity * sizeof(struct Vector));

            if (larger != NULL)
            {
                vectors = larger;
                capacity = new_capacity;
            }
        }

        if (vector_count < capacity)
        {
            location = vector_count;
            vector_count++;
        }
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
    for (i = 0; i < vector_count; i++)
    {
        if (strcmp(vectors[i].name, name) == 0)
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
    if (index >= 0 && index < vector_count)
    {
        *vector = vectors[index];
        found = 1;
    }

    return found;
}

void clear(void)
{
    free(vectors);
    vectors = NULL;
    vector_count = 0;
    capacity = 0;
}