/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcortes <dcortes@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 11:41:33 by dcortes           #+#    #+#             */
/*   Updated: 2024/06/17 17:49:11 by dcortes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# define READ 0
# define WRITE 1
# define STDIN 0
# define STDOUT 1
# define CHILD 0

typedef struct s_file
{
	char	*file;
	int		fd;
}	t_file;

/*
 * Processes
 * - Total number of child processes
 * - Current process number
 * - Array of child process IDs (PIDs)
 * Child Process
 * - Input descriptor
 * - Output descriptor
 * Pipes
 * - Array of pipe fds, size is (n - 1) * 2 (each pipe has 2 descriptors)
 * - Index of the previous pipe
 * - Index of the current pipe
 * Files
 * - Input and output files
 */
typedef struct s_pipex
{
	int		n;
	int		i;
	int		*proc_pid;
	int		proc_input_fd;
	int		proc_output_fd;
	int		*pipe_fd;
	int		pipe_prev;
	int		pipe_curr;
	t_file	file[2];
	char	**envp;
	char	**path;
	char	**argv;
}	t_pipex;

/*
 * Error handling
 */ 
void	handle_error(char *msg);

/*
 * Processes
 */ 
void	child_process_first(t_pipex *p);
void	child_process_last(t_pipex *p);
void	child_process_middle(t_pipex *p);
void	child_process(t_pipex *p);
void	parent_process(t_pipex *p);

/*
 * Command
 */ 
void	exec_cmd(t_pipex *p, char *cmd);
char	**get_path(char **envp);
char	*get_cmd_path(char *cmd, char **path);

/*
 * Pipe
 */
void	pipe_create(t_pipex *p);
void	close_pipes(t_pipex *p, int start, int end);

/*
 * Free
 */
void	free_ptr(void **ptr);
void	free_arr(void ***ptr);

#endif