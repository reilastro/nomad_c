all: star.exe new_catalog.exe 

star.exe: star.o star.h test_star.cpp
	gcc -fPIC -c test_star.cpp
	gcc -lm -lstdc++ test_star.o star.o -o star.exe

star.o: star.cpp star.h
	gcc -fPIC -c star.cpp #-I/usr/include/python2.4


new_catalog.exe: star.o star.h new_catalog.cpp
	gcc -fPIC -c new_catalog.cpp
	gcc -lm -lstdc++ new_catalog.o star.o -o new_catalog.exe

clean:
	rm *.o *.so *.pyc