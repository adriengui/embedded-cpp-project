CC = g++
CFLAGS = -Wall
LDFLAGS = 
SRC = graph.cpp
PROG = graph
OBJS = $(SRC:.cpp=.o)
.SUFFIXES: .cpp .o

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

graph.o: graph.hpp

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o $(PROG)
