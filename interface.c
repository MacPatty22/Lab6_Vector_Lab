/*********************************************************************
* @file inrterface.c
* @brief entire interface code
* @author Mac Patterson
* @date 10/13/2026
* @version 5.3
*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include "vector.h"

static int valid_name(const char *name)
{
    int valid = 1;
    size_t i;

    if (strlen(name) == 0 || strlen(name) >= NAME_SIZE)
    {
        valid = 0;
    }
    else
    {
        if (!isalpha((unsigned char)name[0]) && name[0] != '_')
        {
            valid = 0;
        }

        for (i = 1; i < strlen(name); i++)
        {
            if (!isalnum((unsigned char)name[i]) && name[i] != '_')
            {
                valid = 0;
            }
        }

        if (strcmp(name, "quit") == 0 || strcmp(name, "clear") == 0 ||
            strcmp(name, "list") == 0 || strcmp(name, "help") == 0 ||
            strcmp(name, "load") == 0 || strcmp(name, "save") == 0)
        {
            valid = 0;
        }
    }

    return valid;
}

static int read_number(const char *text, double *number)
{
    char *end;
    int valid;

    errno = 0;
    *number = strtod(text, &end);

    valid = end != text && *end == '\0' &&
            errno != ERANGE && isfinite(*number);

    return valid;
}

static void display(struct Vector vector)
{
    printf("%s = %.6g  %.6g  %.6g\n", vector.name,
           vector.values[0], vector.values[1], vector.values[2]);
}

void show_help(void)
{
    printf("Vector calculator: ./minimat [-h]\n");
    printf("Use spaces around =, +, -, and *.\n");
    printf("Names: 1-31 letters, digits, or underscores; start with a letter or _.\n");
    printf("Commands quit, clear, list, help, load, and save are reserved names.\n");
    printf("Store any number of vectors, each with three double components.\n");
    printf("  a = 1 2 3       Create or replace a vector\n");
    printf("  b = 4, 5, 6     Commas or spaces separate components\n");
    printf("  a               Display a vector\n");
    printf("  a + b           Add vectors\n");
    printf("  a - b           Subtract vectors\n");
    printf("  a * 2.5         Multiply by a scalar\n");
    printf("  2.5 * a         Multiply in either order\n");
    printf("  c = a + b       Store an operation's result\n");
    printf("  list            Display stored vectors\n");
    printf("  load file.csv   Load name,x,y,z lines; replaces same-named vectors\n");
    printf("  save file.csv   Save all stored vectors to a CSV file\n");
    printf("  clear           Remove all stored vectors\n");
    printf("  help            Display this help\n");
    printf("  quit            Exit\n");
}

static void load_file(const char *filename)
{
    FILE *file;
    char line[512];
    char *fields[5];
    char *start;
    char *token;
    struct Vector vector;
    int count;
    int line_number = 0;
    int loaded = 0;
    int skipped = 0;
    int ch;

    file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Could not open '%s': %s\n", filename, strerror(errno));
    }
    else
    {
        while (fgets(line, sizeof(line), file) != NULL)
        {
            line_number++;

            if (strchr(line, '\n') == NULL && !feof(file))
            {
                ch = fgetc(file);

                while (ch != '\n' && ch != EOF)
                {
                    ch = fgetc(file);
                }

                printf("Line %d: too long, skipped.\n", line_number);
                skipped++;
            }
            else
            {
                start = line;

                /* skip the UTF-8 byte order mark Excel may add */
                if (line_number == 1 && strncmp(line, "\xEF\xBB\xBF", 3) == 0)
                {
                    start = line + 3;
                }

                count = 0;
                token = strtok(start, " ,\t\r\n");

                while (token != NULL && count < 5)
                {
                    fields[count] = token;
                    count++;
                    token = strtok(NULL, " ,\t\r\n");
                }

                /* blank lines (count == 0) are ignored */
                if (count > 0)
                {
                    if (count == 4 && valid_name(fields[0]) &&
                        read_number(fields[1], &vector.values[0]) &&
                        read_number(fields[2], &vector.values[1]) &&
                        read_number(fields[3], &vector.values[2]))
                    {
                        strcpy(vector.name, fields[0]);

                        if (addvect(vector))
                        {
                            loaded++;
                        }
                        else
                        {
                            printf("Line %d: memory full, not stored.\n",
                                   line_number);
                            skipped++;
                        }
                    }
                    else
                    {
                        printf("Line %d: expected name,x,y,z - skipped.\n",
                               line_number);
                        skipped++;
                    }
                }
            }
        }

        if (ferror(file))
        {
            printf("Error reading '%s'.\n", filename);
        }

        fclose(file);
        printf("Loaded %d vector(s) from '%s'.\n", loaded, filename);

        if (skipped > 0)
        {
            printf("%d line(s) skipped.\n", skipped);
        }
    }
}

static void save_file(const char *filename)
{
    FILE *file;
    struct Vector vector;
    int i = 0;
    int write_error;

    file = fopen(filename, "w");

    if (file == NULL)
    {
        printf("Could not open '%s': %s\n", filename, strerror(errno));
    }
    else
    {
        while (getvect(i, &vector))
        {
            fprintf(file, "%s,%.15g,%.15g,%.15g\n", vector.name,
                    vector.values[0], vector.values[1], vector.values[2]);
            i++;
        }

        write_error = ferror(file);

        if (fclose(file) != 0 || write_error)
        {
            printf("Error writing '%s'.\n", filename);
        }
        else
        {
            printf("Saved %d vector(s) to '%s'.\n", i, filename);
        }
    }
}

static int evaluate(char *parts[], int count, int assignment,
                    struct Vector *result)
{
    struct Vector a;
    struct Vector b;
    double scalar;
    int success = 0;
    int left_number;
    int right_number;

    switch (count)
    {
        case 1:
            success = findvect(parts[0], result);

            if (!success)
            {
                printf("Vector '%s' does not exist.\n", parts[0]);
            }

            break;

        case 3:
            if (assignment &&
                read_number(parts[0], &result->values[0]) &&
                read_number(parts[1], &result->values[1]) &&
                read_number(parts[2], &result->values[2]))
            {
                success = 1;
                break;
            }

            if (strlen(parts[1]) != 1)
            {
                printf("Invalid expression. Type help for examples.\n");
                break;
            }

            switch (parts[1][0])
            {
                case '+':
                case '-':
                    if (!findvect(parts[0], &a))
                    {
                        printf("Vector '%s' does not exist.\n", parts[0]);
                        break;
                    }

                    if (!findvect(parts[2], &b))
                    {
                        printf("Vector '%s' does not exist.\n", parts[2]);
                        break;
                    }

                    switch (parts[1][0])
                    {
                        case '+':
                            *result = add(a, b);
                            break;

                        case '-':
                            *result = subtract(a, b);
                            break;
                    }

                    success = 1;
                    break;

                case '*':
                    left_number = read_number(parts[0], &scalar);
                    right_number = read_number(parts[2], &scalar);

                    switch (left_number * 2 + right_number)
                    {
                        case 2:
                            read_number(parts[0], &scalar);

                            if (findvect(parts[2], &a))
                            {
                                *result = multiply(a, scalar);
                                success = 1;
                            }
                            else
                            {
                                printf("Vector '%s' does not exist.\n", parts[2]);
                            }

                            break;

                        case 1:
                            if (findvect(parts[0], &a))
                            {
                                *result = multiply(a, scalar);
                                success = 1;
                            }
                            else
                            {
                                printf("Vector '%s' does not exist.\n", parts[0]);
                            }

                            break;

                        default:
                            printf("Multiplication requires one vector and one number.\n");
                            break;
                    }

                    break;

                default:
                    printf("Invalid expression. Type help for examples.\n");
                    break;
            }

            break;

        default:
            printf("Invalid expression. Type help for examples.\n");
            break;
    }

    return success;
}

static void process_expression(char *parts[], int count)
{
    struct Vector result =
    {
        "",
        {
            0.0, 0.0, 0.0
        }
    };

    int assignment = count >= 2 && strcmp(parts[1], "=") == 0;
    int start = assignment ? 2 : 0;
    int success = 0;

    if (assignment && !valid_name(parts[0]))
    {
        printf("Invalid vector name. Type help for naming rules.\n");
    }
    else
    {
        success = evaluate(parts + start, count - start, assignment, &result);

        if (success)
        {
            switch (assignment)
            {
                case 1:
                    strcpy(result.name, parts[0]);

                    if (!addvect(result))
                    {
                        printf("Memory full: result displayed but not stored.\n");
                    }

                    break;

                case 0:
                    if (count != 1)
                    {
                        strcpy(result.name, "ans");
                    }

                    break;
            }

            display(result);
        }
    }
}

void run_interface(void)
{
    char line[512];
    char *parts[8];
    char *token;
    struct Vector vector;
    int running = 1;
    int count;
    int i;
    int ch;

    clear();
    printf("Vector calculator. Type help for instructions.\n");

    while (running)
    {
        printf("minimat> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            running = 0;
        }
        else if (strchr(line, '\n') == NULL && !feof(stdin))
        {
            ch = getchar();

            while (ch != '\n' && ch != EOF)
            {
                ch = getchar();
            }

            printf("Input too long.\n");
        }
        else
        {
            count = 0;
            token = strtok(line, " ,\t\r\n");

            while (token != NULL && count < 8)
            {
                parts[count] = token;
                count++;
                token = strtok(NULL, " ,\t\r\n");
            }

            if (count > 0)
            {
                switch (count <= 2 ? parts[0][0] : '\0')
                {
                    case 'q':
                        if (count == 1 && strcmp(parts[0], "quit") == 0)
                        {
                            running = 0;
                        }
                        else
                        {
                            process_expression(parts, count);
                        }

                        break;

                    case 'h':
                        if (count == 1 && strcmp(parts[0], "help") == 0)
                        {
                            show_help();
                        }
                        else
                        {
                            process_expression(parts, count);
                        }

                        break;

                    case 'c':
                        if (count == 1 && strcmp(parts[0], "clear") == 0)
                        {
                            clear();
                            printf("Vector memory cleared.\n");
                        }
                        else
                        {
                            process_expression(parts, count);
                        }

                        break;

                    case 'l':
                        if (count == 1 && strcmp(parts[0], "list") == 0)
                        {
                            i = 0;

                            while (getvect(i, &vector))
                            {
                                display(vector);
                                i++;
                            }

                            if (i == 0)
                            {
                                printf("No vectors stored.\n");
                            }
                        }
                        else if (strcmp(parts[0], "load") == 0)
                        {
                            if (count == 2)
                            {
                                load_file(parts[1]);
                            }
                            else
                            {
                                printf("Usage: load <filename>\n");
                            }
                        }
                        else
                        {
                            process_expression(parts, count);
                        }

                        break;

                    case 's':
                        if (strcmp(parts[0], "save") == 0)
                        {
                            if (count == 2)
                            {
                                save_file(parts[1]);
                            }
                            else
                            {
                                printf("Usage: save <filename>\n");
                            }
                        }
                        else
                        {
                            process_expression(parts, count);
                        }

                        break;

                    default:
                        process_expression(parts, count);
                        break;
                }
            }
        }
    }
}