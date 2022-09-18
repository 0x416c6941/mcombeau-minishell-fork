/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 17:12:08 by mcombeau          #+#    #+#             */
/*   Updated: 2022/09/18 18:44:35 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* execute_builtin:
*	Executes the given command if it is a builtin command.
*	Returns -1 if the command is not a builtin command.
*	Returns 0 or 1 if the builtin command succeeded or failed.
*/
int	execute_builtin(t_data *data, t_command *cmd)
{
	int	ret;

	ret = CMD_NOT_FOUND;
	if (ft_strncmp(cmd->command, "cd", 3) == 0)
		ret = cd_builtin(data, cmd->args);
	else if (ft_strncmp(cmd->command, "echo", 5) == 0)
		ret = echo_builtin(data, cmd->args);
	else if (ft_strncmp(cmd->command, "env", 4) == 0)
		ret = env_builtin(data, cmd->args);
	else if (ft_strncmp(cmd->command, "export", 7) == 0)
		ret = export_builtin(data, cmd->args);
	else if (ft_strncmp(cmd->command, "pwd", 4) == 0)
		ret = pwd_builtin(data, cmd->args);
	else if (ft_strncmp(cmd->command, "unset", 6) == 0)
		ret = unset_builtin(data, cmd->args);
	else if (ft_strncmp(cmd->command, "exit", 5) == 0)
		ret = exit_builtin(data, cmd->args);
	return (ret);
}

/* execute_sys_bin:
*	Executes the command's system binary file if it can be found
*	among the environment executable paths.
*	Returns CMD_NOT_FOUND if a path to the executable bin file cannot be
*	found. Returns 1 in case of failure to run existing, executable
*	file.
*/
static int	execute_sys_bin(t_data *data, t_command *cmd)
{
	cmd->path = get_cmd_path(data, cmd->command);
	if (!cmd->path)
		return (CMD_NOT_FOUND);
	if (execve(cmd->path, cmd->args, data->env) == -1)
		errmsg_cmd("execve", NULL, strerror(errno), errno);
	return (EXIT_FAILURE);
}

/* execute_local_bin:
*	Attempts to execute the given command as is, in case
*	it is a local directory file or already contains the
*	path to bin.
*	Returns CMD_NOT_FOUND if the command is not an existing executable
*	file. Returns 1 in case of failure to launch executable.
*/
static int	execute_local_bin(t_data *data, t_command *cmd)
{
	if (access(cmd->command, F_OK) != 0)
		return (CMD_NOT_FOUND);
	else if (access(cmd->command, F_OK | X_OK) != 0)
		return (errmsg_cmd(cmd->command, NULL, strerror(errno), CMD_NOT_FOUND - 1));
	if (execve(cmd->command, cmd->args, data->env) == -1)
		errmsg_cmd("execve", NULL, strerror(errno), errno);
	return (EXIT_FAILURE);
}

/* execute_command:
*	Child process tries to execute the given command by setting
*	its input/output fds and searching for an executable.
*	Searching for executable in this order:
*		1. Execute builtin command
*		2. Execute system binaries for command.
*		3. Execute given command name directly (local bin)
*	If it cannot find a matching builtin or executable,
*	prints an error message.
*	Child exits with it's executed program's exit code, or 1 if
*	it could not find one.
*/
int	execute_command(t_data *data, t_command *cmd)
{
	int	ret;

	set_pipe_fds(data->cmd, cmd);
	close_fds(data->cmd, false);
	if (!cmd->command)
		exit(errmsg_cmd("child process", NULL,
				"parsing error: no command to execute!", EXIT_FAILURE));
	if (ft_strchr(cmd->command, '/') == NULL)
	{
		ret = execute_builtin(data, cmd);
		if (ret != CMD_NOT_FOUND)
			exit(ret);
		ret = execute_sys_bin(data, cmd);
		if (ret != CMD_NOT_FOUND)
			exit(ret);
	}
	ret = execute_local_bin(data, cmd);
	if (ret != CMD_NOT_FOUND)
		exit(ret);
	if (get_env_var_value(data->env, "PATH") == NULL)
		exit(errmsg_cmd(cmd->command, NULL, "No such file or directory", CMD_NOT_FOUND));
	exit(errmsg_cmd(cmd->command, NULL, "command not found", CMD_NOT_FOUND));
}
