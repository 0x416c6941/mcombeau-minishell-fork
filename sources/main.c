#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_data data;

	(void)ac;
	(void)av;
	if (!init_env(env))
		// print error message here.
		exit_shell(); // exit with error code.
//	test_execution();
	init_data(&data, env);
	while (1)
	{
		signal(SIGINT, handle_signal);
		signal(SIGQUIT, handle_signal);
		data.user_input = readline(PROMPT);
		if (data.user_input == NULL)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			exit_shell();
		}
		printf("input = %s\n", data.user_input);
		add_history(data.user_input);
		if (tokenization(&data, data.user_input) == 1)
			printf("Error with tokenization\n");
		define_token(&data.token);
		check_consecutives(&data.token);
		free(data.user_input);
	}
	exit_shell();
	return (0);
}
