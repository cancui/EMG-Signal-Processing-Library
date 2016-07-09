#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ecg.h"

int main() {
	puts("Starting ECG test");

	ECG *e = new_ECG(200, 1.8, REFERENCE_AVAILABLE, AUTODETECT_THRESHOLD_ON);
	
	for (int i = 1; i <= 20; i++) {
		for (int j = 1; j <= 170; j++) {


			int bpm = get_BPM_r(e, rand() % 20 + 100, -50);
			printf("%d: %d\n", i * 180 + j, bpm);
		}
		for (int j = 1; j <= 10; j++) {


			int bpm = get_BPM_r(e, rand() % 80 + 60, -50);
			printf("%d: %d\n", i * 180 + 170 + j, bpm);
		}	
	}

	free_ECG(e);
	puts("ECG test ended");
	return 0;
}