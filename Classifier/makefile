CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

main : main.cpp classifier.o segmentor.o
	g++ -g -Wall $(CFLAGS) main.cpp classifier.o segmentor.o -o main $(LIBS) -lboost_filesystem -lboost_system -std=c++11
classifier.o : classifier.cpp classifier.hpp
	g++ -g -Wall -c classifier.cpp -std=c++11
segmentor.o : ../Segmentor/segmentor.cpp ../Segmentor/segmentor.hpp
	g++ -g -Wall -c ../Segmentor/segmentor.cpp -std=c++11
clean :
	rm *.o
