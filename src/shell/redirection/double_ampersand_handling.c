/*
** EPITECH PROJECT, 2025
** handle_ampersand
** File description:
** execute multiple commands but stop at the first failure
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "project.h"
#include "my.h"

void execute_last_command(char **args,
    env_t *head, int *exit_status, separator_index_t *index)
{
    for (int i = index->start; args[i] != NULL; i++) {
        if (args[i] && my_strcmp(args[i], "") != 0) {
            execute_command(&args[index->start], head, exit_status);
            break;
        }
    }
}

static void execute_until_ampersand(double_ampersand_t *ampersand_args,
    separator_index_t *index, int i)
{
    index->end = i;
    execute_segment(ampersand_args->args,
        ampersand_args->head, ampersand_args->exit_status, index);
}

static int should_stop_ampersand_execution(int exit_status)
{
    return exit_status != 0;
}

static void handle_remaining_ampersand(double_ampersand_t *ampersand_args,
    separator_index_t *index)
{
    index->end = -1;
    execute_last_command(ampersand_args->args,
        ampersand_args->head, ampersand_args->exit_status, index);
}

int handle_double_ampersand(char **args, env_t *head, int *exit_status)
{
    double_ampersand_t ampersand_args = {args, head, exit_status};
    separator_index_t index = {0};

    for (int i = 0; args[i] != NULL; i++) {
        if (my_strcmp(args[i], "&&") != 0)
            continue;
        execute_until_ampersand(&ampersand_args, &index, i);
        if (should_stop_ampersand_execution(*exit_status))
            return 0;
        index.start = i + 1;
    }
    handle_remaining_ampersand(&ampersand_args, &index);
    return 0;
}
