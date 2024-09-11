#ifndef LBP_FEATURE_H
#define LBP_FEATURE_H

#include "lbp_feature_defs.h"

void lbp_feature_generate_scaled_rectangles(
  lbp_feature *const self,
  const float *const scales,
  const uint8_t scales_amount
);
float lbp_feature_calculate_vote(
  lbp_feature *const self,
  const lbp_feature_arguments *const arguments
);
void lbp_feature_destroy(lbp_feature *const self);

#endif
