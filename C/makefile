BIN = main.exe
CC = "gcc"
CO = "-std=c99"

all: $(BIN)

$(BIN): #all objects
	$(CC) -o $@ $^






queue: queue_test.exe
	$(CC) -o $< queue.c queue_test.c $(CO)

ma: moving_average_test.exe

moving_average_test.exe: queue.o moving_average_test.o moving_average.o
	$(CC) -o $@ $^

queue.o: queue.c
	$(CC) -c $^

#%.o: %.c %.h
#	$(CC) -o $@ -c $^


#$(CC) -o $< queue.c moving_average_test.c moving_average.c $(CO)