/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcortes <dcortes@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 13:27:18 by dcortes           #+#    #+#             */
/*   Updated: 2024/06/17 14:23:49 by dcortes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	free_ptr(void **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	free_arr(void ***ptr)
{
	void	**temp;

	temp = *ptr;
	while (*temp)
	{
		free_ptr(&(*temp));
		temp++;
	}
	free_ptr((void **)ptr);
}
