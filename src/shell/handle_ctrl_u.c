/*
** EPITECH PROJECT, 2025
** B-PSU-200-LIL-2-1-42sh-lucas.lhomme
** File description:
** handle_ctrl_u
*/
#include "project.h"
#include <unistd.h>

static void clear_and_display(char *line, int remaining, int old_len)
{
    int i = 0;

    if (remaining > 0)
        write(STDOUT_FILENO, line, remaining);
    for (i = remaining; i < old_len; i++)
        write(STDOUT_FILENO, " ", 1);
    for (i = remaining; i < old_len; i++)
        write(STDOUT_FILENO, "\b", 1);
}

static int shift_line_content(char *line, int *pos, int *len, int *remaining)
{
    int i = 0;

    if (*pos < *len) {
        *remaining = *len - *pos;
        for (i = 0; i < *remaining; i++)
            line[i] = line[i + *pos];
        line[i] = '\0';
        *len = *remaining;
    } else {
        line[0] = '\0';
        *len = 0;
    }
    return 0;
}

int handle_ctrl_u(char *line, int *pos, int *len)
{
    int i = 0;
    int old_len = *len;
    int remaining = 0;

    if (*pos <= 0)
        return 0;
    for (i = 0; i < *pos; i++)
        write(STDOUT_FILENO, "\b", 1);
    shift_line_content(line, pos, len, &remaining);
    clear_and_display(line, remaining, old_len);
    *pos = *len;
    return 1;
}
