/*********************************************************************
* @file main.c
* @brief the main for the program
* @author Mac Patterson
* @date 9/30/2026
* @version 5.3
*********************************************************************/

#include <stdio.h>
#include <string.h>
#include "vector.h"

int main(int argc, char *argv[])
{
    int status = 0;
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