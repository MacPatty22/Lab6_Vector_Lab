/*********************************************************************
* @file main.c
* @brief the main for the program
* @author Mac Patterson
* @date 10/13/2026
* @version 5.3
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

int main(int argc, char *argv[])
{
    int status = 0;

    atexit(clear);

    if (argc == 1)
    {
        run_interface();
    }
    else if (argc == 2 && strcmp(argv[1], "-h") == 0)
    {
        show_help();
    }
    else
    {
        printf("Usage: %s [-h]\n", argv[0]);
        status = 1;
    }

    return status;
}