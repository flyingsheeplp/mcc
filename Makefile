CC=gcc
CFLAGS= -c -g

objs=scan.o log.o main.o parse.o

mcc: $(objs)
	$(CC) $^ -o $@

scan.o: scan.c log.h global.h
	$(CC) $(CFLAGS) $<

parse.o: parse.c log.h global.h
	$(CC) $(CFLAGS) $<

main.o: main.c log.h global.h
	$(CC) $(CFLAGS) $<

log.o: log.c log.h global.h
	$(CC) $(CFLAGS) $<



.PHONY:clean
clean:
	rm  *.o
	rm  mcc
