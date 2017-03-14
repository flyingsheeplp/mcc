CC=gcc

objs=scan.o log.o main.o parse.o

mcc: $(objs)
	$(CC) $^ -o $@

scan.o: scan.c log.h global.h
	$(CC) -c $<

parse.o: parse.c log.h global.h
	$(CC) -c $<

main.o: main.c log.h global.h
	$(CC) -c $<

log.o: log.c log.h global.h
	$(CC) -c $<



.PHONY:clean
clean:
	rm  *.o
	rm  mcc
	rm  mcc.exe