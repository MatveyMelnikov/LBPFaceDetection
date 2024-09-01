#ifndef BINARY_STAGE_PARSER_H
#define BINARY_STAGE_PARSER_H

#include <stdint.h>
#include "stage_defs.h"
#include "lbp_feature.h"

const stage *binary_stage_create(uint8_t *const source, uint8_t stages_amount);
void binary_stage_destroy(void);
stage *const binary_stage_get(uint8_t index);

#endif
