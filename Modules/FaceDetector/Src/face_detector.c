#include "face_detector.h"
#include "binary_stage_parser.h"
#include "integral_image.h"
#include <stdlib.h>

// Static functions ----------------------------------------------------------

// Implementations -----------------------------------------------------------

void face_detector_create(const uint8_t *const binary_classifiers_source)
{
  binary_stage_parser_create(binary_classifiers_source);
}

area *const face_detector_detect(
  const uint8_t *const greyscale_image,
  const uint8_t image_size_x,
  const uint8_t image_size_y,
  float base_scale,
  float scale_increment,
  float position_increment,
  uint8_t min_neighbors
)
{
  integral_image_create((integral_image_size) {
    .width = image_size_x,
    .height = image_size_y
  });

  integral_image_destroy();
}
