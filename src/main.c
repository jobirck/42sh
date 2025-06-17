/*
** EPITECH PROJECT, 2025
** B-PSU-200-LIL-2-1-minishell1-joseph.birck
** File description:
** main
*/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include "project.h"

void process_command(char *line, env_t *head, int *exit_status,
    char **env)
{
    int size = strlen(line);
    char *args[size];

    for (int i = 0; i < size; i++)
        args[i] = NULL;
    parse_args(line, args, size);
    if (is_builtin(head, args, env, exit_status) == 1) {
        free_args(args);
        return;
    }
    execute_command(args, head, exit_status);
    free_args(args);
}

int main(int ac, char **av, char **env)
{
    env_t *head = def_linked_list(env[0], 0, env);
    int exit_status = 0;
    history_t *history = NULL;

    (void)ac;
    (void)av;
    signal(SIGINT, handle_ctr_c);
    handle_input(head, &exit_status, env, &history);
    print_history(history);
    free_history(history);
    free_list(head);
    return exit_status;
}
