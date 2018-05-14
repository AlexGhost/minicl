all : fclean
	gcc -g3 -Llibft -lft -framework OpenCL aveccl.c readcl.c -o aveccl
	gcc -g3 -Llibft -lft sanscl.c -o sanscl

fclean :
	rm -rf aveccl
	rm -rf sanscl
