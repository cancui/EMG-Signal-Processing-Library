#include <stdio.h>
#include "queue.h"

int main(int argc, char *argv[]) {
	puts("Starting test");

	Queue * q = queue_new();
	int first = 13;
	queue_push_head(q, &first);
	int firstout = *((int *)queue_peek_tail(q));
	printf("Value: %d\n", firstout);

	puts("next");

	//Queue *q1;
	//q1->head;

	puts("Test ended");
	return 0;
}