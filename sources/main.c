#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_data data;

	(void)ac;
	(void)av;
	ft_memset(&data, 0, sizeof(t_data));
	if (!init_env(env))
	{
		errmsg("Fatal", NULL, "Could not initialize environment", 1);
		exit_shell(NULL, EXIT_FAILURE);
	}
/*	if (ac >= 2 && (av[1][0] == 't' || av[1][0] == 'T'))
	{
		test_minishell(ac, av);
		exit_shell(NULL, EXIT_SUCCESS);
	}*/
	init_data(&data, env);
	while (1)
	{
		signal(SIGINT, handle_signal);
		signal(SIGQUIT, handle_signal);
		data.user_input = readline(PROMPT);
		if (data.user_input == NULL)
			exit_shell(&data, EXIT_FAILURE);
		printf("input = %s\n", data.user_input);
		add_history(data.user_input);
		if (tokenization(&data, data.user_input) == FAILURE)
		{
			free_data(&data, false);
			return (FAILURE);
		}
		if (data.token->type == END)
					break;
		if (check_if_var(&data.token) == FAILURE)
		{
			free_data(&data, false);
			continue;
		}
		var_expander(&data, &data.token);
		handle_quotes(&data);
		var_tokenization(&data);
		print_token(data.token);
		create_commands(&data, data.token);
		printf("------- MAIN: Launching exec.\n");
		execute(&data);
		printf("------- Exec done.\n");
		printf("Segfault?\n");
	}
	exit_shell(&data, EXIT_SUCCESS);
	return (0);
}
