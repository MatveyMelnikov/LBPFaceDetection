#ifndef FACE_DETECTOR_DEFS_H
#define FACE_DETECTOR_DEFS_H

#include <stdint.h>

enum {
  FACE_DETECTOR_SCALES_MIN_ARRAY_SIZE = 10U,
  FACE_DETECTOR_FEATURE_SIZE = 24U
};

typedef struct
{
  const uint8_t *const image;
  const uint8_t image_size_x;
  const uint8_t image_size_y;
  float base_scale;
  float scale_increment;
  float position_increment;
  uint8_t min_neighbors;
} face_detector_arguments;


#endif
