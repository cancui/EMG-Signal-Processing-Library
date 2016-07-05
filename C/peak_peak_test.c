#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "peak_to_peak.h"

int main() {
	puts("starting");

	PkPk *p = new_pkpk(8,2,4);

	if (p != NULL) {
		puts("not null");
	}
	test_print(p);

	for (int i = 1; i < 21; i++) {
		//continue;
		printf("%d\n", i);
	}

	//free_pkpk(p);
	puts("ending");
	return 0;
}