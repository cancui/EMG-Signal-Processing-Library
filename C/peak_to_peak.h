#ifndef PEAK_TO_PEAK_H
#define PEAK_TO_PEAK_H

#include <stdint.h>
#include <stdbool.h>

#include "queue.h"

//#define PKPK_MAX 1
//#define PKPK_MIN 2
//#define PKPK_PKPK 3
//#define PKPK_NEUTRAL 4

typedef struct PkPk_ PkPk;
typedef struct PkPk_data_ PkPk_data;

typedef enum {
	PKPK_MAX,
	PKPK_MIN,
	PKPK_PKPK,
	PKPK_NEUTRAL
} PKPK_Data_Type;


PkPk  	   *new_pkpk(uint16_t sample_frequency, uint16_t min_frequency, uint16_t max_frequency);
void 		free_pkpk(PkPk *self);
PkPk_data  *get_pkpk(PkPk *self, int data_entry_);

//helper functions
void		test_print(PkPk *self);
//bool		empty_queue(Queue *self);
bool		get_queue_max_min(Queue *self, int *max, int *min);

//SWITCH TO USING ENUM
int			get_data(PkPk_data *package, PKPK_Data_Type attribute/*uint8_t attribute*/);

#endif