# all:
# 	g++ -Wall -Wextra -O2 -g server.cpp -o server
# 	g++ -Wall -Wextra -O2 -g client.cpp -o client
# 
# remove:
# 	rm -f server client

IDIR =../include
CC=g++
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = hash_table.h serialization.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = server.o hash_table.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

