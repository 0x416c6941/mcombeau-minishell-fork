/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_io.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 17:51:46 by mcombeau          #+#    #+#             */
/*   Updated: 2022/09/18 15:13:27 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO : heredoc functions.

/* restore_io:
*	Restores the original standard input and standard output
*	to their original fds of 0 and 1. Used to clear the input/output
*	fds after execution, in preparation for the next set of user commands.
*	Returns 1 if the duplication was successful, 0 if not.
*/
bool	restore_io(t_io_fds *io)
{
	int	ret;

	ret = true;
	if (!io)
		return (ret);
	if (io->stdin_backup != -1)
		if (dup2(io->stdin_backup, STDIN_FILENO) == -1)
			ret = errmsg_cmd("dup2", "restoring stdin", strerror(errno), false);
	if (io->stdout_backup != -1)
		if (dup2(io->stdout_backup, STDOUT_FILENO) == -1)
			ret = errmsg_cmd("dup2", "restoring stdout",
					strerror(errno), false);
	return (ret);
}

/* redirect_io:
*	Duplicates the input and output fds to the standard input and output.
*	Backs up the standard input and output before replacing them in order
*	to restore them after execution.
*	Returns 1 for success, 0 in case of error.
*/
static bool	redirect_io(t_io_fds *io)
{
	int	ret;

	ret = true;
	io->stdin_backup = dup(STDIN_FILENO);
	if (io->stdin_backup == -1)
		ret = errmsg_cmd("dup", "stdin backup", strerror(errno), false);
	io->stdout_backup = dup(STDOUT_FILENO);
	if (io->stdout_backup == -1)
		ret = errmsg_cmd("dup", "stdout backup", strerror(errno), false);
	if (io->fd_in != -1)
		if (dup2(io->fd_in, STDIN_FILENO) == -1)
			ret = errmsg_cmd("dup2", io->infile, strerror(errno), false);
	if (io->fd_out != -1)
		if (dup2(io->fd_out, STDOUT_FILENO) == -1)
			ret = errmsg_cmd("dup2", io->outfile, strerror(errno), false);
	return (ret);
}

/* redirect_infile_outfile:
*	Checks if the infile and outfile are set correctly.
*	If they are, redirects input and output accordingly.
*	Returns 1 on success, 0 on failure.
*/
bool	redirect_infile_outfile(t_data *data)
{
	t_io_fds	*io;

	io = data->cmd->io_fds;
	if (!io || (!io->infile && !io->outfile))
		return (true);
	if ((io->infile && io->fd_in == -1)
		|| (io->outfile && io->fd_out == -1))
		return (false);
	return (redirect_io(io));
}
