#ifndef STAGE_DEFS_H
#define STAGE_DEFS_H

#include <stdint.h>
#include "lbp_feature_defs.h"

#define ADDR_TYPE uint64_t

typedef struct
{
	lbp_feature *features;
	uint8_t features_amount;
	float threshold;
} stage;

#endif
