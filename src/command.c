/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcortes <dcortes@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:38:25 by dcortes           #+#    #+#             */
/*   Updated: 2024/06/17 17:23:18 by dcortes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 

#include "libft.h"
#include "pipex.h"

/*
 * Executes a command by splitting it into arguments, finding the command's path,
 * and then using execve to run it. If execve fails, it handles the error.
 * Finally, it frees the allocated memory for the command path and arguments.
 */
void	exec_cmd(t_pipex *p, char *cmd)
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = ft_split(cmd, ' ');
	cmd_path = get_cmd_path(cmd_args[0], p->path);
	if (execve(cmd_path, cmd_args, p->envp) == -1)
		handle_error("Execve error");
	free_ptr((void **)&cmd_path);
	free_arr((void ***)&cmd_args);
}

/* 
 * Returns an array of strings, 
 * each representing a path from the PATH variable.
 */
char	**get_path(char **envp)
{
	char	*keyword;
	char	*envp_path;
	char	**path;

	keyword = "PATH=";
	while (*envp)
	{
		if (ft_strnstr(*envp, keyword, 5))
		{
			envp_path = *envp + 5;
			break ;
		}
		envp++;
	}
	path = ft_split(envp_path, ':');
	return (path);
}

/*
 * Returns the path of the command if it exists in the PATH
 * Constructs the full path for a command by appending it 
 * to each directory in the PATH variable until a match is found.
 * Returns NULL if the command is not found.
 */
char	*get_cmd_path(char *cmd, char **path)
{
	char	*cmd_path;

	cmd = ft_strjoin("/", cmd);
	while (*path)
	{
		cmd_path = ft_strjoin(*path, cmd);
		if (access(cmd_path, F_OK) == 0)
		{
			return (cmd_path);
		}
		free (cmd_path);
		path++;
	}
	handle_error("Command not found");
	return (NULL);
}
