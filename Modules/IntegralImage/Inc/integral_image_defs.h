#ifndef INTEGRAL_IMAGE_DEFS_H
#define INTEGRAL_IMAGE_DEFS_H

#include <stdint.h>

typedef struct
{
	uint8_t width;
	uint8_t height;
} integral_image_size;

typedef struct
{
	uint16_t x;
	uint16_t y;
} integral_image_position;

typedef struct
{
	integral_image_position top_left_corner;
	integral_image_position bottom_right_corner;
} integral_image_rectangle_position;

#endif