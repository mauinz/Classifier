CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

main : main.cpp classifier.o
	g++ -g -Wall $(CFLAGS) main.cpp classifier.o -o main $(LIBS) -lboost_filesystem -lboost_system -std=c++11
classifier.o : classifier.cpp classifier.hpp
	g++ -g -Wall -c classifier.cpp -std=c++11

clean :
	rm *.o
