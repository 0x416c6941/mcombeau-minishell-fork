/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcombeau <mcombeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 19:03:08 by mcombeau          #+#    #+#             */
/*   Updated: 2022/09/29 16:59:22 by mcombeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_wds(t_data *data, char *wd)
{
//	printf("UPDATE WORKING DIRECTORIES\n");
//	printf("\twd = %s\n", wd);
//	printf("\toldwd = %s\n", oldwd);
	set_env_var(data, "OLDPWD", get_env_var_value(data->env, "PWD"));
	set_env_var(data, "PWD", wd);
	if (data->old_working_dir)
	{
		free(data->old_working_dir);
		data->old_working_dir = ft_strdup(data->working_dir);
	}
	if (data->working_dir)
	{
		free(data->working_dir);
		data->working_dir = ft_strdup(wd);
	}
//	printf("Updated OLDWD = %s\n", data->old_working_dir);
//	printf("Updated PWD = %s\n", data->working_dir);
}

/* change_dir:
*	Changes the current working directory and updates the
*	OLDPWD environment variable.
*	Returns 1 on success, 0 on failure.
*/
static bool	change_dir(t_data *data, char *path)
{
//	printf("CHANGE DIR\n");
	char	*ret;
	char	cwd[PATH_MAX];
	char	new_cwd[PATH_MAX];

	ret = NULL;
	ret = getcwd(cwd, PATH_MAX);
//	printf("\tgetcwd returned [%s]\n", cwd);
	if (!ret)
	{
		errmsg_cmd("cd: error retrieving current directory",
					"getcwd: cannot access parent directories",
					strerror(errno), errno);
		if (chdir(path) != 0)
		{
//			printf("Chdir failed. Attempting to restore old pwd\n");
			path = data->old_working_dir;
//			printf ("new path = %s\n", path);
		}
	}
	if (chdir(path) != 0)
	{
		errmsg_cmd("cd", path, strerror(errno), errno);
		return (false);
	}
	ret = getcwd(new_cwd, PATH_MAX);
	if (!ret)
		return (false);
	update_wds(data, new_cwd);
	return (true);
}

/* cd_builtin:
*	Executes the builtin cd command by changing the working directory.
*	Returns 0 on success, 1 on failure.
*/
int	cd_builtin(t_data *data, char **args)
{
	char	*path;

	if (!args || !args[1] || ft_isspace(args[1][0]) 
		|| args[1][0] == '\0' || ft_strncmp(args[1], "--", 3) == 0)
	{
		path = get_env_var_value(data->env, "HOME");
		if (!path || *path == '\0' || ft_isspace(*path))
			return (errmsg_cmd("cd", NULL, "HOME not set", EXIT_FAILURE));
		return (!change_dir(data, path));
	}
	if (args[2])
		return (errmsg_cmd("cd", NULL, "too many arguments", EXIT_FAILURE));
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_var_value(data->env, "OLDPWD");
		if (!path)
			return (errmsg_cmd("cd", NULL, "OLDPWD not set", EXIT_FAILURE));
//		printf("%s\n", get_env_var_value(data->env, "OLDPWD"));
		return (!change_dir(data, get_env_var_value(data->env, "OLDPWD")));
	}
	return (!change_dir(data, args[1]));
}
