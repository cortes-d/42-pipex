/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcortes <dcortes@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:36:17 by dcortes           #+#    #+#             */
/*   Updated: 2024/06/17 17:28:48 by dcortes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 

#include "pipex.h"

/* Creates (n - 1) pipes.
 * Each pipe has 2 file descriptors (reading and writing).
 * We malloc (n - 1) * 2 in order to store the 2 file descriptors of each pipe.
 */
void	pipe_create(t_pipex *p)
{
	int	i;

	p->pipe_fd = malloc(sizeof(int) * ((p->n - 1) * 2));
	if (!p->pipe_fd)
		handle_error("Malloc error for pipe_fd");
	i = 0;
	while (i < (p->n - 1))
	{
		if (pipe(p->pipe_fd + (i * 2)) == -1)
			handle_error("Pipe error");
		i++;
	}
}

/*
 * Closes all pipe fds in the range from `start` to `end`.
 * Each pipe has two fds (for reading and writing), 
 * accessed by multiplying the index `i` by 2 
 * and adding the offset for READ or WRITE.
 */
void	close_pipes(t_pipex *p, int start, int end)
{
	int	i;

	if (start > end)
		return ;
	i = start;
	while (i <= end)
	{
		close(p->pipe_fd[i * 2 + READ]);
		close(p->pipe_fd[i * 2 + WRITE]);
		i++;
	}
}
