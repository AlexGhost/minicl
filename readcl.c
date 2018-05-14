/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readcl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acourtin <acourtin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/14 14:56:16 by acourtin          #+#    #+#             */
/*   Updated: 2018/05/14 17:46:08 by acourtin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <OpenCL/opencl.h>
#include "libft/libft.h"
#include <stdlib.h>

char	**readcl(char *source, int *i)
{
	char	**kernel_source;
	int		fd;
	char	*line;
	int		j;

	fd = 0;
	if ((fd = open(source, O_RDONLY)) > 0)
		while (get_next_line(fd, &line) == 1)
			j++;
	else
		ft_putendl("Echec de lecture : 1");
	close(fd);
	if (!(kernel_source = ft_memalloc(sizeof(char *) * (j + 1))))
		return (NULL);
	j = 0;
	if ((fd = open(source, O_RDONLY)) > 0)
		while (get_next_line(fd, &kernel_source[j]) == 1)
			j++;
	else
		ft_putendl("Echec de lecture : 2");
	*i = j;
	ft_strdel(&kernel_source[j]);
	close(fd);
	return (kernel_source);
}
