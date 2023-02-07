

CFLAGS ?= -Wall -Wextra -Ofast -g


LDLIBS ?= -lm

chess: chess.o
	g++ -o chess $^ $(CFLAGS) $(LDLIBS)
%.o: %.cc
	$(CC) -o $@ -c $< $(CFLAGS) 




.PHONY: clean
clean:
	rm -rf *.o
	rm chess