#ifndef INTEGRAL_IMAGE_H
#define INTEGRAL_IMAGE_H

#include "integral_image_defs.h"

void integral_image_create(integral_image_size size);
void integral_image_destroy(void);
void integral_image_calculate(const uint16_t *const image);
const uint16_t *const integral_image_get(void);
const uint16_t integral_image_get_rectangle(
	integral_image_rectangle_position *const rectangle
);
const uint16_t integral_image_get_rectangle_of_squares(
	integral_image_rectangle_position *const rectangle
);

#endif
