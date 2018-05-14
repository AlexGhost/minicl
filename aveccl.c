/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aveccl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acourtin <acourtin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 15:29:46 by acourtin          #+#    #+#             */
/*   Updated: 2018/05/14 16:39:11 by acourtin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// importer opencl
#include <OpenCL/opencl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"

char	**readcl(char *source, int *i);

typedef struct	s_cl
{
	cl_device_id		device;
	cl_platform_id		platform;
	cl_context			context;
	cl_command_queue	queue;
}				t_cl;

// code du kernel
int main (int argc, char **argv)
{
	char	**kernelSource;

	int		i;
	t_cl	gpu;

	if (!(kernelSource = readcl("script.cl", &i)))
		return (0);

	// creer un contexte
	clGetPlatformIDs (1, &gpu.platform, NULL);
	clGetDeviceIDs (gpu.platform, CL_DEVICE_TYPE_CPU, 1, &gpu.device, NULL);
	gpu.context = clCreateContext (0, 1, &gpu.device, NULL, NULL, NULL);

	//creer une file de commandes
	gpu.queue = clCreateCommandQueue(gpu.context, gpu.device, 0, 0);

	// allouer et initialiser la memoire du device
	double		av;
	double		ap;
	av = 42.f;
	cl_mem inputBufferDevice = clCreateBuffer (gpu.context, CL_MEM_READ_ONLY |
		CL_MEM_COPY_HOST_PTR, sizeof(double), &av, 0);
	cl_mem outputBufferDevice = clCreateBuffer (gpu.context, CL_MEM_WRITE_ONLY,
		sizeof (double), 0, 0);

	// charger et compiler le kernel ; i correspond aux lignes compris dans le script
	cl_program kernelProgram = clCreateProgramWithSource (gpu.context, i,
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
	clEnqueueNDRangeKernel (gpu.queue, kernel, 1, 0, WorkSize, 0, 0, 0, 0);

	// recuperer les donnees calculees dans la memoire du device
	clEnqueueReadBuffer (gpu.queue, outputBufferDevice, CL_TRUE, 0, \
		sizeof (double), &ap, 0, NULL, NULL);

	// liberer les ressources
	clReleaseKernel (kernel);
	clReleaseProgram (kernelProgram);
	clReleaseCommandQueue (gpu.queue);
	clReleaseMemObject (inputBufferDevice);
	clReleaseMemObject (outputBufferDevice);
	clReleaseContext (gpu.context);

	printf("aveccl :\tavant = %f\taprès = %f\n", av, ap);
	free(kernelSource);
	exit(0);
	return (0);
}
