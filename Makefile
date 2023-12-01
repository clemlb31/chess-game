

CFLAGS ?= -Wall -Wextra -Ofast


LDLIBS ?= -lm

chess: chess.o piece.o game.o square.o
	g++ -o chess $^ $(CFLAGS) $(LDLIBS)
%.o: %.cc
	$(CC) -o $@ -c $< $(CFLAGS) 




.PHONY: clean
clean:
	rm -rf *.o
	rm chess