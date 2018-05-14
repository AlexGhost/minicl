/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanscl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acourtin <acourtin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 16:26:56 by acourtin          #+#    #+#             */
/*   Updated: 2018/05/14 17:17:40 by acourtin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libft/libft.h"

void					alexatan(double *av, double *ap)
{
	size_t		i = 0;

	while (i < 100000000)
	{
		*ap = atan(*av) * atan(*av) * atan(*av) * atan(*av) * atan(*av) * atan(*av);
		i++;
	}
}

int						main(void)
{
	double		av;
	double		ap;

	av = 42.f;
	alexatan(&av, &ap);
	printf("sanscl :\tavant = %f\taprès = %f\n", av, ap);
	return (0);
}
