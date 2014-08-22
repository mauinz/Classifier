CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

main : main.cpp augmentor.o segmentor.o
	g++ -g -Wall main.cpp augmentor.o segmentor.o -o main $(CFLAGS) $(LIBS) -lboost_filesystem -lboost_system -std=c++11
augmentor.o : augmentor.cpp augmentor.hpp
	g++ -g -Wall -c augmentor.cpp -std=c++11
segmentor.o : ../Segmentor/segmentor.cpp ../Segmentor/segmentor.hpp
	g++ -g -Wall -c ../Segmentor/segmentor.cpp -std=c++11
clean :
	rm *.o
