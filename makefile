CC = gcc
CFLAGS = -Wall
all:	text2morse.out
		@echo "To run: ./text2morse.out <dittime in ms> <frequency in Hz>"

text2morse.out: text2morse.o dsp.o
	$(CC) $(LDFLAGS) -o $@ $^ -lm

text2morse.o: text2morse.c morsetable.h dsp.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

morsetable.h: morsetable.out CharToMorse.txt
	./morsetable.out CharToMorse.txt > morsetable.h

morsetable.out: morsetable.c
	$(CC) $(CFLAGS) -o morsetable.out morsetable.c -lm

dsp.o: dsp.c dsp.h
	$(CC) $(CCFLAGS) -c -o $@ $<
