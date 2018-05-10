/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aveccl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acourtin <acourtin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 15:29:46 by acourtin          #+#    #+#             */
/*   Updated: 2018/05/10 19:55:24 by acourtin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// importer opencl
#include <OpenCL/opencl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"

// code du kernel
int main (int argc, char **argv)
{
	char	**kernelSource;

	kernelSource = ft_memalloc(sizeof(char*) * 100);
	int i;
	int j;
	int fd;
	char *line;
	i = 0;
	while (i < 100)
	{
		kernelSource[i] = ft_memalloc(sizeof(char) * 100);
		i++;
	}
	if ((fd = open("script.cl", O_RDONLY)) > 0)
	{
		j = 0;
		while (get_next_line(fd, &line) == 1)
		{
			kernelSource[j] = ft_strdup(line);
			j++;
			ft_strdel(&line);
		}
	}
	kernelSource[j] = 0;
	/*i = 0;
	while (kernelSource[i])
	{
		ft_putendl(kernelSource[i]);
		i++;
	}*/
	// creer un contexte
	cl_platform_id platform;
	clGetPlatformIDs (1, &platform, NULL);
	cl_device_id device;
	clGetDeviceIDs (platform, CL_DEVICE_TYPE_ALL, 1, &device, NULL);
	cl_context context = clCreateContext (0, 1, &device, NULL, NULL, NULL);

	//creer une file de commandes
	cl_command_queue commandQueue = clCreateCommandQueue(context, device, 0, 0);

	// allouer et initialiser la memoire du device
	double		av;
	double		ap;
	av = 42.f;
	cl_mem inputBufferDevice = clCreateBuffer (context, CL_MEM_READ_ONLY |
		CL_MEM_COPY_HOST_PTR, sizeof(double), &av, 0);
	cl_mem outputBufferDevice = clCreateBuffer (context, CL_MEM_WRITE_ONLY,
		sizeof (double), 0, 0);

	// charger et compiler le kernel
	cl_program kernelProgram = clCreateProgramWithSource (context, 4,
		(const char**)kernelSource, 0, 0);
	clBuildProgram (kernelProgram, 0, NULL, NULL, NULL, NULL);
	cl_kernel kernel = clCreateKernel (kernelProgram, "add42", NULL);
	if (!kernel)
	{
		printf("!!! Erreur compilation du script OpenCL !!!\n");
		exit(0);
	}
	clSetKernelArg (kernel, 0, sizeof (cl_mem), (void *) &inputBufferDevice);
	clSetKernelArg (kernel, 1, sizeof (cl_mem), (void *) &outputBufferDevice);

	// ajouter le kernel dans la file de commandes
	size_t WorkSize[1] = { 1 };
	clEnqueueNDRangeKernel (commandQueue, kernel, 1, 0, WorkSize, 0, 0, 0, 0);

	// recuperer les donnees calculees dans la memoire du device
	clEnqueueReadBuffer (commandQueue, outputBufferDevice, CL_TRUE, 0,
		sizeof (double), &ap, 0, NULL, NULL);

	// liberer les ressources
	clReleaseKernel (kernel);
	clReleaseProgram (kernelProgram);
	clReleaseCommandQueue (commandQueue);
	clReleaseMemObject (inputBufferDevice);
	clReleaseMemObject (outputBufferDevice);
	clReleaseContext (context);

	printf("aveccl :\tavant = %f\taprès = %f\n", av, ap);
	free(kernelSource);
	exit(0);
	return (0);
}
