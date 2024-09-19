#ifndef MOCK_STAGE_H
#define MOCK_STAGE_H

#include <stdint.h>
#include "face_detector_defs.h"

void mock_stage_create(const uint8_t max_expectations);
void mock_stage_destroy(void);
void mock_stage_verify_complete(void);
void mock_stage_expect_create_stage_from_source(
  const stage *const stages,
  const uint8_t stages_amount
);
void mock_stage_expect_calculate_scaled_features(void);
void mock_stage_expect_calculate_prediction(
  const bool *const value
);
stage *const mock_stage_create_stage_from_source(
  const uint8_t *const source,
  uint8_t stages_amount
);
void mock_stage_calculate_scaled_features(
  stage *const self,
  const float *const scales,
  const uint8_t scales_amount
);
bool mock_stage_calculate_prediction(
  stage *const self,
  const lbp_feature_arguments *const arguments
);

#endif
