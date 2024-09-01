#ifndef FACE_DETECTOR_H
#define FACE_DETECTOR_H

#include "area.h"

void face_detector_create(const uint8_t *const binary_classifiers_source);
area *const face_detector_detect(
  const uint8_t *const image,
  const uint8_t image_size_x,
  const uint8_t image_size_y,
  float base_scale,
  float scale_increment,
  float position_increment,
  uint8_t min_neighbors
);

#endif
