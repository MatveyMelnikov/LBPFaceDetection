#ifndef FACE_DETECTOR_H
#define FACE_DETECTOR_H

#include "face_detector_defs.h"
#include "area.h"

void face_detector_create(
  const uint8_t *const binary_classifiers_source,
  const uint8_t stages_amount
);
void face_detector_destroy(void);
area *const face_detector_detect(
  const face_detector_arguments *const arguments
);

#endif
