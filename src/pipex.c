/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcortes <dcortes@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 16:13:07 by dcortes           #+#    #+#             */
/*   Updated: 2024/06/17 17:22:20 by dcortes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <sys/wait.h>

#include "libft.h"
#include "pipex.h"

/*
 * Simulates the shell command "< file1 cmd1 | cmd2 > file2" 
 * in C as "./pipex file1 cmd1 cmd2 file2".
 * For each command, a child process is created,
 * with pipes facilitating inter-process communication.
 * The parent process oversees the orchestration, 
 * while dup2 is used for redirecting file descriptors,
 * enabling the output of one command to serve as the input for the next.
 */

/*
 * Creates n child process and (n - 1) pipes.
 * It allocates memory for process IDs, forks each process,
 * and directs them to their respective execution functions.
 */
void	create_processes(t_pipex *p)
{
	pipe_create(p);
	p->i = 0;
	p->proc_pid = malloc(sizeof(int) * p->n);
	if (!p->proc_pid)
		handle_error("Malloc error for proc_pid");
	while (p->i < p->n)
	{
		p->proc_pid[p->i] = fork();
		if (p->proc_pid[p->i] == -1)
			handle_error("Fork error");
		if (p->proc_pid[p->i] == CHILD)
			child_process(p);
		p->i++;
	}
	parent_process(p);
}

/*
 * Main function parameters explanation:
 * argc = Total number of command line arguments
 * argv[0] = Program name (./pipex)
 * argv[1] = Input file (file1)
 * argv[2] = First command (cmd1)
 * argv[3] = Second command (cmd2)
 * argv[4] = Output file (file2)
 */
int	main(int argc, char **argv, char **envp)
{
	t_pipex	p;

	p.argv = argv;
	p.envp = envp;
	p.path = get_path(envp);
	p.file[0].file = argv[1];
	p.file[1].file = argv[argc - 1];
	if (argc >= 5)
	{
		p.n = argc - 3;
		create_processes(&p);
	}
	else
		handle_error("Usage : ./pipex file1 cmd1 cmd2 file2");
	free_ptr((void **)&p.proc_pid);
	free_ptr((void **)&p.pipe_fd);
	free_arr((void ***)&p.path);
	return (0);
}
