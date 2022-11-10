/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_append.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexa <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 00:06:46 by alexa             #+#    #+#             */
/*   Updated: 2022/11/10 00:08:37 by alexa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/* open_outfile_append:
*	Opens an outfile in append mode. If an outfile was already set, frees it
*	and overwrites it. If a previous infile or outfile open failed (file does
*	not exist or permission denied), does not open any further output file.
*
*	Ex.:
*		echo hello > forbidden_file >> test
*		echo hello >> forbidden_file >> test
*		< forbidden_file cat >> test
*	In these 3 cases, the test file should not be opened or created.
*/
static void	open_outfile_append(t_io_fds *io, char *file, char *var_filename)
{
	if (!remove_old_file_ref(io, false))
		return ;
	io->outfile = ft_strdup(file);
	if (io->outfile && io->outfile[0] == '\0' && var_filename)
	{
		errmsg_cmd(var_filename, NULL, "ambiguous redirect", false);
		return ;
	}
	io->fd_out = open(io->outfile, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (io->fd_out == -1)
		errmsg_cmd(io->outfile, NULL, strerror(errno), false);
}

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
	t_token		*temp;
	t_command	*cmd;

	temp = *token_lst;
	cmd = lst_last_cmd(*last_cmd);
	init_io(cmd);
	open_outfile_append(cmd->io_fds, temp->next->str, temp->next->str_backup);
	if (temp->next->next)
		temp = temp->next->next;
	else
		temp = temp->next;
	*token_lst = temp;
}
