/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aveccl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acourtin <acourtin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 15:29:46 by acourtin          #+#    #+#             */
/*   Updated: 2018/05/14 17:44:55 by acourtin         ###   ########.fr       */
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

typedef struct	s_program
{
	int			source_size;
	cl_mem		*buffers;
	cl_program	program;
	cl_kernel	kernel;
	char		**source;
}				t_program;

// code du kernel
int main (int argc, char **argv)
{
	t_cl		gpu;
	t_program	prog;

	if (!(prog.source = readcl("script.cl", &prog.source_size)))
		return (0);

	// creer un contexte
	clGetPlatformIDs (1, &gpu.platform, NULL);
	clGetDeviceIDs (gpu.platform, CL_DEVICE_TYPE_ALL, 1, &gpu.device, NULL);
	gpu.context = clCreateContext (0, 1, &gpu.device, NULL, NULL, NULL);

	//creer une file de commandes
	gpu.queue = clCreateCommandQueue(gpu.context, gpu.device, 0, 0);

	// allouer et initialiser la memoire du device
	double		av;
	double		ap;
	av = 42.f;
	if (!(prog.buffers = ft_memalloc(sizeof(cl_mem) * 2)))
		return (0);
	prog.buffers[0] = clCreateBuffer (gpu.context, CL_MEM_READ_ONLY |
		CL_MEM_COPY_HOST_PTR, sizeof(double), &av, 0);
	prog.buffers[1] = clCreateBuffer (gpu.context, CL_MEM_WRITE_ONLY,
		sizeof (double), 0, 0);

	// charger et compiler le kernel ; i correspond aux lignes compris dans le script
	prog.program = clCreateProgramWithSource (gpu.context, prog.source_size,
		(const char**)prog.source, 0, 0);
	clBuildProgram (prog.program, 0, NULL, NULL, NULL, NULL);
	prog.kernel = clCreateKernel (prog.program, "alexatan", NULL);
	if (!prog.kernel)
	{
		printf("!!! Erreur compilation du script OpenCL !!!\n");
		exit(0);
	}
	clSetKernelArg (prog.kernel, 0, sizeof (cl_mem), (void *) &prog.buffers[0]);
	clSetKernelArg (prog.kernel, 1, sizeof (cl_mem), (void *) &prog.buffers[1]);

	// ajouter le kernel dans la file de commandes
	size_t WorkSize[1] = { 1 };
	clEnqueueNDRangeKernel (gpu.queue, prog.kernel, 1, 0, WorkSize, 0, 0, 0, 0);

	// recuperer les donnees calculees dans la memoire du device
	clEnqueueReadBuffer (gpu.queue, prog.buffers[1], CL_TRUE, 0, \
		sizeof (double), &ap, 0, NULL, NULL);

	// liberer les ressources
	clReleaseKernel (prog.kernel);
	clReleaseProgram (prog.program);
	clReleaseCommandQueue (gpu.queue);
	clReleaseMemObject (prog.buffers[0]);
	clReleaseMemObject (prog.buffers[1]);
	clReleaseContext (gpu.context);

	printf("aveccl :\tavant = %f\taprès = %f\n", av, ap);
	free(prog.source);
	free(prog.buffers);
	exit(0);
	return (0);
}
