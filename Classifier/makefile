CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

main : main.cpp classifier.o
	g++ -g -Wall main.cpp classifier.o -o main $(CFLAGS) $(LIBS) -lboost_filesystem -lboost_system
classifier.o : classifier.cpp classifier.hpp
	g++ -g -Wall -c classifier.cpp 
