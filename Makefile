CC=gcc

objs=scan.o log.o main.o

mcc: $(objs)
	$(CC) $^ -o $@

scan.o: scan.c log.h
	$(CC) -c $<

main.o: main.c log.h
	$(CC) -c $<

log.o: log.c log.h
	$(CC) -c $<



.PHONY:clean
clean:
	rm -rf *.o
	rm -rf mcc