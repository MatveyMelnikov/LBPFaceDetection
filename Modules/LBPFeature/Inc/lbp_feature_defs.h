#ifndef LBP_FEATURE_DEFS_H
#define LBP_FEATURE_DEFS_H

#include <stdint.h>

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
} lbp_feature_rectangle;

typedef struct
{
	lbp_feature_rectangle rectangles[9];
	lbp_feature_rectangle scaled_rectangles[9];
	int32_t masks[8];
	float left_value;
	float right_value;
} lbp_feature;

#endif
