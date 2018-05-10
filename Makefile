all : fclean
	gcc -Llibft -lft -framework OpenCL aveccl.c -o aveccl
	gcc -Llibft -lft sanscl.c -o sanscl

fclean :
	rm -rf aveccl
	rm -rf sanscl
