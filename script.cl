__kernel void add42(__global double *av, __global double *ap)
{
	*ap = atan(*av) * atan(*av);
	*ap = 42;
	*ap = atan(*av) * atan(*av);
		*ap = atan(*av) * atan(*av);
		*ap = 42;
		*ap = atan(*av) * atan(*av);
			*ap = atan(*av) * atan(*av);
			*ap = 42;
			*ap = atan(*av) * atan(*av);
}
