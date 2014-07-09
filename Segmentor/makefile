CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

main : main.cpp segmentor.o slic.o
	g++ -g -Wall main.cpp segmentor.o slic.o -o main $(CFLAGS) $(LIBS)
segmentor.o : segmentor.cpp segmentor.hpp
	g++ -g -Wall -c segmentor.cpp 
slic.o : SLIC/slic.cpp SLIC/slic.h
	g++ -g -Wall -c SLIC/slic.cpp 
