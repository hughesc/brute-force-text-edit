CC=g++
CFLAGS=-c -Wall -w
LDFLAGS=-lncurses
SOURCES=main.cpp cursor.cpp preline.cpp line.cpp pane.cpp 
OBJS=$(SOURCES:.cpp=.o)
EXE=brute

all: $(SOURCES) $(EXE)
	
$(EXE): $(OBJS) 
	$(CC) $(OBJS) $(LDFLAGS) -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJS) $(EXE)


