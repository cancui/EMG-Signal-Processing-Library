BIN = main.exe
CC = "gcc" 
#"-std=c11"
CO = "-std=c11"

all: $(BIN)

$(BIN): #all objects
	$(CC) -o $@ $^






queue: queue_test.exe
	$(CC) -o $< queue.c queue_test.c $(CO)

ma: moving_average_test.exe

moving_average_test.exe: queue.o moving_average_test.o moving_average.o
	$(CC) -o $@ $^ $(CO)

pkpk: peak_peak_test.exe

peak_peak_test.exe: queue.c peak_to_peak.c peak_peak_test.c
	$(CC) -o $@ $^ $(CO)

#queue.o: queue.c
#	$(CC) -c $^

#%.o: %.c %.h
#	$(CC) -o $@ -c $^


#$(CC) -o $< queue.c moving_average_test.c moving_average.c #$(CO)

clean:
	rm -f *.o
