/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanscl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acourtin <acourtin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 16:26:56 by acourtin          #+#    #+#             */
/*   Updated: 2018/05/10 20:00:22 by acourtin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libft/libft.h"

void					add42(double *av, double *ap)
{
	size_t		i = 0;

	while (i < 100000000)
	{
		*ap = atan(*av);
		i++;
	}
}

int						main(void)
{
	double		av;
	double		ap;

	av = 42.f;
	add42(&av, &ap);
	printf("sanscl :\tavant = %f\taprès = %f\n", av, ap);
	return (0);
}