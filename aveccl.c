/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aveccl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acourtin <acourtin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 15:29:46 by acourtin          #+#    #+#             */
/*   Updated: 2018/05/14 17:51:38 by acourtin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <OpenCL/opencl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"

char			**readcl(char *source, int *i);

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

int				main(int argc, char **argv)
{
	double		av;
	double		ap;
	size_t		work_size[1];
	t_cl		gpu;
	t_program	prog;

	work_size[0] = 1;
	if (!(prog.source = readcl("script.cl", &prog.source_size)))
		return (0);
	clGetPlatformIDs(1, &gpu.platform, NULL);
	clGetDeviceIDs(gpu.platform, CL_DEVICE_TYPE_ALL, 1, &gpu.device, NULL);
	gpu.context = clCreateContext(0, 1, &gpu.device, NULL, NULL, NULL);
	gpu.queue = clCreateCommandQueue(gpu.context, gpu.device, 0, 0);
	av = 42.f;
	if (!(prog.buffers = ft_memalloc(sizeof(cl_mem) * 2)))
		return (0);
	prog.buffers[0] = clCreateBuffer(gpu.context, CL_MEM_READ_ONLY |
		CL_MEM_COPY_HOST_PTR, sizeof(double), &av, 0);
	prog.buffers[1] = clCreateBuffer(gpu.context, CL_MEM_WRITE_ONLY,
		sizeof (double), 0, 0);

	prog.program = clCreateProgramWithSource(gpu.context, prog.source_size,
		(const char**)prog.source, 0, 0);
	clBuildProgram(prog.program, 0, NULL, NULL, NULL, NULL);
	prog.kernel = clCreateKernel(prog.program, "alexatan", NULL);
	if (!prog.kernel)
	{
		printf("!!! Erreur compilation du script OpenCL !!!\n");
		exit(0);
	}
	clSetKernelArg(prog.kernel, 0, sizeof (cl_mem), (void *) &prog.buffers[0]);
	clSetKernelArg(prog.kernel, 1, sizeof (cl_mem), (void *) &prog.buffers[1]);
	clEnqueueNDRangeKernel(gpu.queue, prog.kernel, 1, 0, WorkSize, 0, 0, 0, 0);
	clEnqueueReadBuffer(gpu.queue, prog.buffers[1], CL_TRUE, 0, \
		sizeof(double), &ap, 0, NULL, NULL);
	clReleaseKernel(prog.kernel);
	clReleaseProgram(prog.program);
	clReleaseCommandQueue(gpu.queue);
	clReleaseMemObject(prog.buffers[0]);
	clReleaseMemObject(prog.buffers[1]);
	clReleaseContext(gpu.context);
	printf("aveccl :\tavant = %f\taprès = %f\n", av, ap);
	free(prog.source);
	free(prog.buffers);
	exit(0);
	return (0);
}
