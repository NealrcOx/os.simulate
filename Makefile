object = sh3.o
cc = gcc
sh3 : $(object)
	cc -o sh3 $(object)
sh3.o : sh3.c
	cc -c -g sh3.c
.PHONY : clean
clean :
	-rm sh3
