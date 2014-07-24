PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)

BOOST_INC = /usr/local/include
BOOST_LIB = /usr/local/lib

SEG_PATH = /home/matthew/Documents/classifier/Segmentor/

CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

main : main.cpp classifier.o segmentor.o
	g++ -g -Wall $(CFLAGS) main.cpp classifier.o segmentor.o -o main $(LIBS) -L$(BOOST_LIB) -lboost_filesystem -lboost_system -std=c++11
classifier.o : classifier.cpp classifier.hpp
	g++ -g -Wall -I$(PYTHON_INCLUDE) $(CFLAGS) -I$(BOOST_INC) -fPIC -c classifier.cpp -std=c++11
segmentor.o : ../Segmentor/segmentor.cpp ../Segmentor/segmentor.hpp
	g++ -g -Wall -I$(PYTHON_INCLUDE) $(CFLAGS) -I$(BOOST_INC) -fPIC -c ../Segmentor/segmentor.cpp -std=c++11
clean :
	rm *.o
classifier.so: classifier.o segmentor.o
	g++ -shared -Wl,--export-dynamic \
	classifier.o -L$(BOOST_LIB) -lboost_python -L$(SEG_PATH) -L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION) -lboost_filesystem -lboost_system  -l:segmentor.so -std=c++11 $(LIBS) \
	-o classifier.so