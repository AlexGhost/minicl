__kernel void alexatan(__global double *av, __global double *ap)
{
	for (int i = 0; i < 100000000; i++)
	{
		*ap = atan(*av) * atan(*av) * atan(*av) * atan(*av) * atan(*av);
	}
}
