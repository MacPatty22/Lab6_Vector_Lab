/*********************************************************************
* @file vector.h
* @brief header for the vector calculator
* @author Mac Patterson
* @date 9/30/2026
* @version 5.3
*********************************************************************/

#ifndef VECTOR_H
#define VECTOR_H

#define MAX_VECTORS 10
#define NAME_SIZE 32

struct Vector
{
    char name[NAME_SIZE];
    double values[3];
};

struct Vector add(struct Vector a, struct Vector b);
struct Vector subtract(struct Vector a, struct Vector b);
struct Vector multiply(struct Vector a, double scalar);
int addvect(struct Vector vector);
int findvect(const char *name, struct Vector *vector);
int getvect(int index, struct Vector *vector);
void clear(void);
void show_help(void);
void run_interface(void);

#endif