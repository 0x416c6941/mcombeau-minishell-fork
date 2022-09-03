#include "minishell.h"

/*
	***APPEND***
	Redirection of output in append mode causes the file whose name results 
	from the expansion of word to be opened for appending on file descriptor n, 
	or the standard output (fd 1) if n is not specified. 
	If the file does not exist it is created.

	The general format for appending output is: [n]>>word.

	Focus on the O_APPEND flag of the function open() :
	" O_APPEND
              The file is opened in append mode.  Before each write(2),
              the file offset is positioned at the end of the file, as
              if with lseek(2).  The modification of the file offset and
              the write operation are performed as a single atomic step."
*/

/**
 * When encountering a APPEND typed token ('>>'), this function :
 *  - sets the boolean cmd->iredir_out to TRUE
 * 	- If the next token is valid, it opens the output file (or creates
 * 	 it if it doesn't exist) with the correct relative path retrieved
 * 	 beforehand and appends the previous token(s) to the end of the
 * 	 output file.
 * 	- With the fd information, completes the necessary fields of 
 * 	  the command structure -> fd_out and potentially error and err_msg.
 *
 */
void	parse_append(t_command **last_cmd, t_token **token_lst)
{
	t_token	*temp;
	t_command	*cmd;
	char	*file;
	int		fd;

	temp = *token_lst;
	cmd = *last_cmd;
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~\nPARSE - Parse_append function\n");
	cmd->redir_out = true;
	if (temp->next->type == WORD || temp->next->type == VAR)
	{
		// char *test = get_absolute_path(data->env, temp->next->str);
		// printf("test : %s\n", test);
		file = get_relative_path(temp->next->str);
		fd = open(file, O_CREAT | O_RDWR | O_APPEND, S_IRWXU);
		if (fd == -1)
		{
			cmd->error = errno;
			cmd->err_msg = ft_strdup(strerror(errno));
			cmd->io_fds->fd_out = 2;
			printf("N° d'erreur : %d - Erreur : %s - Fd : %d\n", cmd->error,\
		cmd->err_msg, cmd->io_fds->fd_out);
		}
		else
			cmd->io_fds->fd_out = fd;
		free(file);
	}
	// else // SYNTAX ERROR ex: echo hello > , ou echo hello > |
	// à mettre dans lexer_utils maybe?
	printf("Fd : %d\n", cmd->io_fds->fd_out);
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	*token_lst = temp->next->next;
}