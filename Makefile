all : fclean
	clang -Llibft -lft -framework OpenCL aveccl.c -o aveccl
	clang -Llibft -lft sanscl.c -o sanscl

fclean :
	rm -rf aveccl
	rm -rf sanscl
