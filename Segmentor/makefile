PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)

BOOST_INC = /usr/local/include
BOOST_LIB = /usr/local/lib

CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

main : main.cpp segmentor.o slic.o
	g++ -g -Wall main.cpp segmentor.o slic.o -o main $(CFLAGS) $(LIBS)
segmentor.o : segmentor.cpp segmentor.hpp
	g++ -g -Wall -I$(PYTHON_INCLUDE) $(CFLAGS) -I$(BOOST_INC) -fPIC -c segmentor.cpp 
slic.o : SLIC/slic.cpp SLIC/slic.h
	g++ -g -Wall -c SLIC/slic.cpp 
clean :
	rm *.o
segmentor.so: segmentor.o
	g++ -shared -Wl,--export-dynamic segmentor.o -L$(BOOST_LIB) -lboost_python -L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION) $(LIBS)-o segmentor.so
 