CC = gcc
default: infix

infix: infix.o decimal.o
	@$(CC) $(CFLAGS) -o infix decimal.o infix.o -lm

infix.o: infix.c
	@$(CC) $(CFLAGS) -c infix.c

decimal.o: decimal.c decimal.h
	@$(CC) $(CFLAGS) -c decimal.c

run: default
	./infix history.txt

clean:
	$(RM) infix *.o *~ *.txt

	
