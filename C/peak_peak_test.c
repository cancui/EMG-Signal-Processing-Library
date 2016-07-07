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
		PkPk_data *just_returned = get_pkpk(p, i);
		int after = get_data(just_returned, PKPK_MIN);
		printf("returned: %d\n", after);
		free(just_returned);
	}

	for (int i = 21; i > 0; i--) {
		printf("%d\n", i);
		PkPk_data *just_returned = get_pkpk(p, i);
		int after = get_data(just_returned, PKPK_MIN);
		printf("returned: %d\n", after);
		free(just_returned);
	}

	free_pkpk(p);
	puts("ending");
	return 0;
}