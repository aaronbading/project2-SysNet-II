# g++ file.c -lstdc++
# Compiler flags:
CC = g++
DEBUG = -g
CFLAGS = $(DEBUG) -c -Wall -std=c++11 
LDLIBS =

TARGET1 = client
TARGET2 = server

CLIENTSOURCES = client.cpp clienthelper.cpp
SERVERSOURCES = server.cpp serverhelper.cpp

CLIENTOBJECTS = $(CLIENTSOURCES:.cpp=.o)
SERVEROBJECTS = $(SERVERSOURCES:.cpp=.o)
all: $(TARGET1) $(TARGET2)

$(TARGET1): $(CLIENTOBJECTS)
	$(CC) -pthread -lpthread -o $(TARGET1) $(CLIENTOBJECTS) $(LDLIBS)
$(TARGET2): $(SERVEROBJECTS)
	$(CC) -pthread -lpthread -o $(TARGET2) $(SERVEROBJECTS) $(LDLIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ 
.PHONY : all $(TARGET1) $(TARGET2)

# Must call 'make clean' for cleaning to occur
clean:
	
clobber: clean
	rm -f $(TARGET1) $(TARGET2) $(TARGET1).o $(TARGET2).o clienthelper.o serverhelper.o