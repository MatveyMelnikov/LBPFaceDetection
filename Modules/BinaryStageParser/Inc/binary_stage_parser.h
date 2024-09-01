#ifndef BINARY_STAGE_PARSER_H
#define BINARY_STAGE_PARSER_H

#include <stdint.h>
#include "stage_defs.h"
#include "lbp_feature.h"

stage *const binary_stage_create(uint8_t* source, uint8_t stages_amount);
void binary_stage_destroy(void);
stage *const binary_stage_get(uint8_t index);
lbp_feature *const binary_stage_get_features(
	stage *const base_stage,
	uint16_t index
);

#endif
