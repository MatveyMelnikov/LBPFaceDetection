#ifndef LBP_FEATURE_DEFS_H
#define LBP_FEATURE_DEFS_H

#include <stdint.h>

enum {
  LBP_FEATURE_RECTANGLES_AMOUNT = 9U,
  LBP_FEATURE_MASKS_AMOUNT = 8U
};

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
} lbp_feature_rectangle;

typedef struct
{
  uint8_t offset_x;
  uint8_t offset_y;
  float scale_index;
} lbp_feature_arguments;

typedef struct
{
	lbp_feature_rectangle rectangles[LBP_FEATURE_RECTANGLES_AMOUNT];
  // There should just be a pointer - a dynamic array, since we
  // generate them at runtime and don't know the size
	lbp_feature_rectangle **scaled_rectangles;
  uint8_t scaled_rectangles_amount;
	int32_t masks[LBP_FEATURE_MASKS_AMOUNT];
	float left_value;
	float right_value;
} lbp_feature;

#endif
