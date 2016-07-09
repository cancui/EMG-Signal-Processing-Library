#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "emg.h"

int main() {
	puts("Starting EMG test");

	EMG *e = new_EMG(100, 0.1, 25, 150, HIGH_PASS_FILTER_ON, REFERENCE_AVAILABLE);

	for (int i = 1; i < 500; i++) {
		int returned = filter_EMG(e, rand()%20 + i);
		printf("%d: %d\n", i, returned);
	}

	free_EMG(e);
	return 0;
}