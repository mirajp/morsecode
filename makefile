CC = gcc
CFLAGS = -Wall
all:	text2morse
		@echo "To run: ./text2morse <dittime in ms> <frequency in Hz>"

text2morse: text2morse.o dsp.o
	$(CC) $(LDFLAGS) -o $@ $^

text2morse.o: text2morse.c morsetable.h dsp.h
	$(CC) $(CFLAGS) -c -o $@ $<

morsetable.h: morsetable CharToMorse.txt
	./morsetable CharToMorse.txt > morsetable.h

morsetable: morsetable.c
	$(CC) $(CFLAGS) -o morsetable morsetable.c

dsp.o: dsp.c dsp.h
	$(CC) $(CCFLAGS) -c -o $@ $<