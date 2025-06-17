#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/internal/test.h>
#include <stdio.h>
#include <stdlib.h>
#include "project.h"

int check_redirection_syntax(char **args);

void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

void get_redirected(void)
{
    cr_get_redirected_stdout();
    cr_get_redirected_stderr();
}

// Tests for resolve_command_path
Test(resolve_command_path, valid_command)
{
    env_t *env = malloc(sizeof(env_t));
    env->line = strdup("PATH=/usr/bin:/bin");
    char *result = find_path("ls", env);
    cr_assert_str_eq(result, "/usr/bin/ls");
    free(result);
    free(env->line);
    free(env);
}

Test(resolve_command_path, invalid_command)
{
    env_t *env = malloc(sizeof(env_t));
    env->line = strdup("PATH=/usr/bin:/bin");
    char *result = find_path("invalid_command", env);
    cr_assert_null(result);
    free(result);
    free(env->line);
    free(env);
}

Test(resolve_command_path, empty_path)
{
    env_t *env = malloc(sizeof(env_t));
    env->line = strdup("PATH=");
    char *result = find_path("ls", env);
    cr_assert_null(result);
    free(result);
    free(env->line);
    free(env);
}

Test(resolve_command_path, non_executable_command)
{
    env_t *env = malloc(sizeof(env_t));
    env->line = strdup("PATH=/usr/bin:/bin");
    char *result = find_path("non_executable_file", env);
    cr_assert_null(result);
    free(result);
    free(env->line);
    free(env);
}

// Tests for is_command
Test(is_command, valid_command)
{
    env_t *env = malloc(sizeof(env_t));
    char **args = malloc(sizeof(char *) * 2);
    args[0] = strdup("ls");
    args[1] = NULL;
    int exit_status = 0;
    cr_assert_eq(is_command(env, args, &exit_status), 0);
    free(args[0]);
    free(args);
    free(env);
}

Test(is_command, invalid_command)
{
    env_t *env = malloc(sizeof(env_t));
    char **args = malloc(sizeof(char *) * 2);
    args[0] = strdup("invalid_command");
    args[1] = NULL;
    int exit_status = 0;
    cr_assert_eq(is_command(env, args, &exit_status), 0);
    free(args[0]);
    free(args);
    free(env);
}

Test(is_command, empty_command)
{
    env_t *env = malloc(sizeof(env_t));
    char **args = malloc(sizeof(char *) * 1);
    args[0] = NULL;
    int exit_status = 0;
    cr_assert_eq(is_command(env, args, &exit_status), 0);
    free(args);
    free(env);
}

Test(is_command, pipe_command)
{
    env_t *env = malloc(sizeof(env_t));
    char **args = malloc(sizeof(char *) * 2);
    args[0] = strdup("|");
    args[1] = NULL;
    int exit_status = 0;
    cr_assert_eq(is_command(env, args, &exit_status), 1);
    free(args[0]);
    free(args);
    free(env);
}

Test(is_command, semicolon_command)
{
    env_t *env = malloc(sizeof(env_t));
    char **args = malloc(sizeof(char *) * 2);
    args[0] = strdup(";");
    args[1] = NULL;
    int exit_status = 0;
    cr_assert_eq(is_command(env, args, &exit_status), 1);
    free(args[0]);
    free(args);
    free(env);
}

Test(is_command, and_command)
{
    env_t *env = malloc(sizeof(env_t));
    char **args = malloc(sizeof(char *) * 2);
    args[0] = strdup("&&");
    args[1] = NULL;
    int exit_status = 0;
    cr_assert_eq(is_command(env, args, &exit_status), 1);
    free(args[0]);
    free(args);
    free(env);
}

Test(is_command, or_command)
{
    env_t *env = malloc(sizeof(env_t));
    char **args = malloc(sizeof(char *) * 2);
    args[0] = strdup("||");
    args[1] = NULL;
    int exit_status = 0;
    cr_assert_eq(is_command(env, args, &exit_status), 1);
    free(args[0]);
    free(args);
    free(env);
}

// Tests for check_redirection_syntax
Test(check_redirection_syntax, valid_syntax)
{
    char **args = malloc(sizeof(char *) * 4);
    args[0] = strdup("ls");
    args[1] = strdup(">");
    args[2] = strdup("output.txt");
    args[3] = NULL;
    cr_assert_eq(check_redirection_syntax(args), 0);
    free(args[0]);
    free(args[1]);
    free(args[2]);
    free(args);
}

Test(check_redirection_syntax, missing_name)
{
    char **args = malloc(sizeof(char *) * 3);
    args[0] = strdup("ls");
    args[1] = strdup(">");
    args[2] = NULL;
    cr_assert_eq(check_redirection_syntax(args), 1);
    free(args[0]);
    free(args[1]);
    free(args);
}

Test(check_redirection_syntax, missing_name_at_end)
{
    char **args = malloc(sizeof(char *) * 3);
    args[0] = strdup("ls");
    args[1] = strdup("<");
    args[2] = NULL;
    cr_assert_eq(check_redirection_syntax(args), 1);
    free(args[0]);
    free(args[1]);
    free(args);
}

// Tests for handle_semicolon
Test(handle_semicolon, valid_semicolon)
{
    env_t *env = malloc(sizeof(env_t));
    char **args = malloc(sizeof(char *) * 4);
    int exit_status = 0;
    args[0] = strdup("ls");
    args[1] = strdup(";");
    args[2] = strdup("echo");
    args[3] = NULL;
    cr_assert_eq(handle_semicolon(args, env, &exit_status), 0);
    free(args[0]);
    free(args[1]);
    free(args[2]);
    free(args);
    free(env);
}

Test(handle_semicolon, empty_command)
{
    env_t *env = malloc(sizeof(env_t));
    char **args = malloc(sizeof(char *) * 1);
    int exit_status = 0;
    args[0] = NULL;
    cr_assert_eq(handle_semicolon(args, env, &exit_status), 0);
    free(args);
    free(env);
}

Test(handle_semicolon, invalid_command)
{
    env_t *env = malloc(sizeof(env_t));
    char **args = malloc(sizeof(char *) * 3);
    int exit_status = 0;
    args[0] = strdup("invalid_command");
    args[1] = strdup(";");
    args[2] = NULL;
    cr_assert_eq(handle_semicolon(args, env, &exit_status), 0);
    free(args[0]);
    free(args[1]);
    free(args);
    free(env);
}

// Tests for my_which
Test(my_which, valid_command, .init = redirect_all_stdout)
{
    env_t *env = malloc(sizeof(env_t));
    env->line = strdup("PATH=/usr/bin:/bin");
    char **args = malloc(sizeof(char *) * 3);
    int exit_status = 0;

    args[0] = strdup("which");
    args[1] = strdup("ls");
    args[2] = NULL;

    cr_assert_eq(my_which(args, env, NULL, &exit_status), 0);
    fflush(stdout);
    fflush(stderr);
    cr_assert_stdout_eq_str("/usr/bin/ls\n");

    free(args[0]);
    free(args[1]);
    free(args);
    free(env->line);
    free(env);
}

Test(my_which, invalid_command, .init = redirect_all_stdout)
{
    env_t *env = malloc(sizeof(env_t));
    env->line = strdup("PATH=/usr/bin:/bin");
    char **args = malloc(sizeof(char *) * 3);
    int exit_status = 0;

    args[0] = strdup("which");
    args[1] = strdup("invalid_command");
    args[2] = NULL;

    cr_assert_eq(my_which(args, env, NULL, &exit_status), 1);
    fflush(stdout);
    fflush(stderr);
    cr_assert_stdout_eq_str("invalid_command: Command not found.\n");

    free(args[0]);
    free(args[1]);
    free(args);
    free(env->line);
    free(env);
}

Test(my_which, no_arguments, .init = redirect_all_stdout)
{
    env_t *env = malloc(sizeof(env_t));
    env->line = strdup("PATH=/usr/bin:/bin");
    char **args = malloc(sizeof(char *) * 2);
    int exit_status = 0;

    args[0] = strdup("which");
    args[1] = NULL;

    cr_assert_eq(my_which(args, env, NULL, &exit_status), 1);
    fflush(stdout);
    fflush(stderr);
    cr_assert_stdout_eq_str("which: Too few arguments.\n");

    free(args[0]);
    free(args);
    free(env->line);
    free(env);
}

Test(my_which, command_with_absolute_path, .init = redirect_all_stdout)
{
    env_t *env = malloc(sizeof(env_t));
    env->line = strdup("PATH=/usr/bin:/bin");
    char **args = malloc(sizeof(char *) * 3);
    int exit_status = 0;

    args[0] = strdup("which");
    args[1] = strdup("/bin/ls");
    args[2] = NULL;

    cr_assert_eq(my_which(args, env, NULL, &exit_status), 0);
    fflush(stdout);
    fflush(stderr);
    cr_assert_stdout_eq_str("/bin/ls\n");

    free(args[0]);
    free(args[1]);
    free(args);
    free(env->line);
    free(env);
}

// Tests for my_where
Test(my_where, valid_command, .init = redirect_all_stdout)
{
    env_t *env = malloc(sizeof(env_t));
    env->line = strdup("PATH=/usr/bin:/bin");
    char **args = malloc(sizeof(char *) * 3);
    int exit_status = 0;

    args[0] = strdup("where");
    args[1] = strdup("ls");
    args[2] = NULL;

    cr_assert_eq(my_where(args, env, NULL, &exit_status), 0);
    fflush(stdout);
    fflush(stderr);
    cr_assert_stdout_eq_str("/usr/bin/ls\n/bin/ls\n");

    free(args[0]);
    free(args[1]);
    free(args);
    free(env->line);
    free(env);
}

Test(my_where, invalid_command, .init = redirect_all_stdout)
{
    env_t *env = malloc(sizeof(env_t));
    env->line = strdup("PATH=/usr/bin:/bin");
    char **args = malloc(sizeof(char *) * 3);
    int exit_status = 0;

    args[0] = strdup("where");
    args[1] = strdup("invalid_command");
    args[2] = NULL;

    cr_assert_eq(my_where(args, env, NULL, &exit_status), 0);
    fflush(stdout);
    fflush(stderr);
    cr_assert_stdout_eq_str("");

    free(args[0]);
    free(args[1]);
    free(args);
    free(env->line);
    free(env);
}

Test(my_where, no_arguments, .init = redirect_all_stdout)
{
    env_t *env = malloc(sizeof(env_t));
    env->line = strdup("PATH=/usr/bin:/bin");
    char **args = malloc(sizeof(char *) * 2);
    int exit_status = 0;

    args[0] = strdup("where");
    args[1] = NULL;

    cr_assert_eq(my_where(args, env, NULL, &exit_status), 1);
    fflush(stdout);
    fflush(stderr);
    cr_assert_stdout_eq_str("where: Too few arguments.\n");

    free(args[0]);
    free(args);
    free(env->line);
    free(env);
}

Test(is_builtin, valid_builtin)
{
    env_t *env = malloc(sizeof(env_t));
    char **args = malloc(sizeof(char *) * 3);
    int exit_status = 0;

    args[0] = strdup("cd");
    args[1] = strdup("..");
    args[2] = NULL;

    cr_assert_eq(is_builtin(env, args, NULL, &exit_status), 1);

    free(args[0]);
    free(args[1]);
    free(args);
    free(env);
}

Test(is_builtin, invalid_builtin)
{
    env_t *env = malloc(sizeof(env_t));
    char **args = malloc(sizeof(char *) * 3);
    int exit_status = 0;

    args[0] = strdup("invalid_builtin");
    args[1] = strdup("..");
    args[2] = NULL;

    cr_assert_eq(is_builtin(env, args, NULL, &exit_status), 0);

    free(args[0]);
    free(args[1]);
    free(args);
    free(env);
}


Test(is_builtin, pipe_command)
{
    env_t *env = malloc(sizeof(env_t));
    char **args = malloc(sizeof(char *) * 2);
    int exit_status = 0;

    args[0] = strdup("|");
    args[1] = NULL;

    cr_assert_eq(is_builtin(env, args, NULL, &exit_status), 0);

    free(args[0]);
    free(args);
    free(env);
}
