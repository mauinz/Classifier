CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv` -lboost_filesystem -lboost_system

% : %.cpp
	g++ $(CFLAGS) -o $@ $<  $(LIBS)