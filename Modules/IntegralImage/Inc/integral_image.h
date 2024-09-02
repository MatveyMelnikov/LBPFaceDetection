#ifndef INTEGRAL_IMAGE_H
#define INTEGRAL_IMAGE_H

#include "integral_image_defs.h"

void integral_image_create(integral_image_size size);
// To save memory, we load the black and white
// image directly into the integral
void integral_image_set(FILL_IMAGE_FUNCTOR);
void integral_image_destroy(void);
void integral_image_calculate(void);
const uint16_t integral_image_get_rectangle(
	integral_image_rectangle_position *const rectangle
);

#endif
