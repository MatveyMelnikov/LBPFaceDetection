#ifndef STAGE_DEFS_H
#define STAGE_DEFS_H

#include <stdint.h>
#include "lbp_feature_defs.h"

typedef struct
{
	lbp_feature *features;
	uint8_t features_amount;
	float threshold;
} stage;

#endif
