__kernel void add42(__global double *av, __global double *ap)
{
	*ap = atan(*av);
}
