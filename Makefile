#object = sh3.o
#object = pi2.o
#object = sort.o
object = pc2.o
cc = gcc
#sh3 : $(object)
#pi2 : $(object)
#sort : $(object)
pc2 : $(object)
#	cc -o sh3 $(object)
#	cc -o pi2 $(object) -lpthread
#cc -o sort $(object) -lpthread
	cc -o pc2 $(object) -lpthread
#sh3.o : sh3.c
#pi2.o : pi2.c
#sort.o : sort.c
pc2.o : pc2.c
#	cc -c -g sh3.c
#	cc -c -g pi2.c
#	cc -c -g sort.c
	cc -c -g pc2.c
.PHONY : clean
clean :
#	-rm sh3 sh3.o
#-rm pi2 pi2.o
#	-rm sort sort.o
	-rm pc2 pc2.o
