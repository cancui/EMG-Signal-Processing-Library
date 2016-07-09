#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ecg.h"

int main() {
	puts("Starting ECG test");

	ECG *e = new_ECG(20, REFERENCE_UNAVAILABLE);
	
	for (int i = 1; i <= 70; i++) {
		
		int bpm = get_BPM(e, i);
		printf("%d: %d\n", i, bpm);
	}

	free_ECG(e);
	puts("ECG test ended");
	return 0;
}