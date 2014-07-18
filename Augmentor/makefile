CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

main : main.cpp augmentor.o
	g++ -g -Wall main.cpp augmentor.o -o main $(CFLAGS) $(LIBS) -lboost_filesystem -lboost_system -std=c++11
classifier.o : augmentor.cpp augmentor.hpp
	g++ -g -Wall -c classifier.cpp -std=c++11

clean :
	rm *.o
