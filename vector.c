/*********************************************************************
* @file vector.c
* @brief defines the structs
* @author Mac Patterson
* @date 9/30/2026
* @version 5.3
*********************************************************************/

#include "vector.h"

struct Vector add(struct Vector a, struct Vector b)
{
    struct Vector result =
    {
        "",
        {
            0.0, 0.0, 0.0
        }
    };
    int i;
    for (i = 0; i < 3; i++)
    {
        result.values[i] = a.values[i] + b.values[i];
    }

    return result;
}

struct Vector subtract(struct Vector a, struct Vector b)
{
    struct Vector result =
    {
        "",
        {
            0.0, 0.0, 0.0
        }
    };
    int i;
    for (i = 0; i < 3; i++)
    {
        result.values[i] = a.values[i] - b.values[i];
    }

    return result;
}

struct Vector multiply(struct Vector a, double scalar)
{
    struct Vector result =
    {
        "",
        {
            0.0, 0.0, 0.0
        }
    };
    int i;
    for (i = 0; i < 3; i++)
    {
        result.values[i] = a.values[i] * scalar;
    }

    return result;
}