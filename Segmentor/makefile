CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

main : main.cpp segmentor.o
	g++ -g -Wall main.cpp segmentor.o -o main $(CFLAGS) $(LIBS)
segmentor.o : segmentor.cpp segmentor.hpp
	g++ -g -Wall -c segmentor.cpp $(CFLAGS) $(LIBS)
