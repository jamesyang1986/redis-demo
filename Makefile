DEBUG?= -g
CFLAGS?= -O2 -Wall
CCOPT=$(CFLAGS)

OBJ = main.o sds.o
PRGNAME = redis-demo

all: redis-demo

sds.o: sds.c sds.h
main.o: main.c sds.h

redis-demo: $(OBJ)
	$(CC) -o $(PRGNAME) $(CCOPT) $(DEBUG) $(OBJ)
	@echo "start to compile redis-demo."

#.c.o:
#	$(CC) -c $(CCOPT) $(DEBUG) $(COMPILE_TIME) $<

clean:
	rm -rf $(PRGNAME) *.o