/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcortes <dcortes@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:35:38 by dcortes           #+#    #+#             */
/*   Updated: 2024/06/17 17:49:57 by dcortes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> 
#include <stdlib.h> 
#include <sys/wait.h>
#include <fcntl.h>

#include "pipex.h"

/*
 * Configures file descriptors for the first child process in the pipeline.
 * Close all pipes except the first one.
 */
void	child_process_first(t_pipex *p)
{
	close_pipes(p, p->i + 1, p->n - 2);
	close(p->pipe_fd[READ]);
	p->proc_input_fd = open(p->file[0].file, O_RDONLY);
	if (p->proc_input_fd == -1)
		handle_error("File open error");
	p->proc_output_fd = p->pipe_fd[WRITE];
}

/*
 * Configures file descriptors for the last child process in the pipeline.
 * Close all pipes except the last one.
 */
void	child_process_last(t_pipex *p)
{
	close_pipes(p, 0, p->n - 3);
	close(p->pipe_fd[p->pipe_prev + WRITE]);
	p->proc_input_fd = p->pipe_fd[p->pipe_prev + READ];
	p->proc_output_fd = open(p->file[1].file,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (p->proc_output_fd == -1)
		handle_error("File open error");
}

/*
 * Configures file descriptors for a middle process in the pipeline.
 * Close all pipes except the current and previous one.
 */
void	child_process_middle(t_pipex *p)
{
	close_pipes(p, 0, p->i - 2);
	close_pipes(p, p->i + 1, p->n - 2);
	close(p->pipe_fd[p->pipe_prev + WRITE]);
	close(p->pipe_fd[p->pipe_curr + READ]);
	p->proc_input_fd = p->pipe_fd[p->pipe_prev + READ];
	p->proc_output_fd = p->pipe_fd[p->pipe_curr + WRITE];
}

/*
 * Handles the execution of a child process in a pipelined command, 
 * setting up input and output redirection based on 
 * its position (first, middle, last) in the pipeline, 
 * and executing the specified command.
 */
void	child_process(t_pipex *p)
{
	p->pipe_prev = (p->i - 1) * 2;
	p->pipe_curr = p->i * 2;
	if (p->i == 0)
		child_process_first(p);
	else if (p->i == (p->n - 1))
		child_process_last(p);
	else
		child_process_middle(p);
	dup2(p->proc_input_fd, STDIN_FILENO);
	dup2(p->proc_output_fd, STDOUT_FILENO);
	if (p->i != 0)
		close(p->pipe_fd[p->pipe_prev + READ]);
	if (p->i != p->n - 1)
		close(p->pipe_fd[p->pipe_curr + WRITE]);
	exec_cmd(p, p->argv[p->i + 2]);
	exit(0);
}

/*
 * Cleans up after all child processes have finished: closes all pipes, 
 * waits for each child process to terminate, 
 * and frees the allocated memory for pipe file descriptors.
 */
void	parent_process(t_pipex *p)
{
	int	i;

	i = 0;
	while (i < p->n)
	{
		close(p->pipe_fd[i * 2 + READ]);
		close(p->pipe_fd[i * 2 + WRITE]);
		waitpid(p->proc_pid[i], NULL, 0);
		i++;
	}
	free(p->pipe_fd);
	p->pipe_fd = NULL;
}
