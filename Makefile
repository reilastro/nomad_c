all: star.exe new_catalog.exe new_catalog2.exe

star.exe: star.o star.hpp test_star.cpp
	gcc -fPIC -c test_star.cpp
	gcc -lm -lstdc++ test_star.o star.o -o star.exe

star.o: star.cpp star.hpp
	gcc -fPIC -c star.cpp #-I/usr/include/python2.4


new_catalog.exe: star.o star.hpp new_catalog.cpp
	gcc -fPIC -c new_catalog.cpp
	gcc -lm -lstdc++ new_catalog.o star.o -o new_catalog.exe

new_catalog2.exe: star.o star.hpp new_catalog2.cpp
	gcc -fPIC -Wno-deprecated -c new_catalog2.cpp
	gcc -lm -lstdc++ new_catalog2.o star.o -o new_catalog2.exe



clean:
	rm *.o *.so *.pyc