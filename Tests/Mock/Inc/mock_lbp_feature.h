#ifndef MOCK_LBP_FEATURE_H
#define MOCK_LBP_FEATURE_H

#include <stdint.h>
#include "lbp_feature_defs.h"

void mock_lbp_feature_create(const uint8_t max_expectations);
void mock_lbp_feature_destroy(void);
// void mock_lbp_feature_expect_read_then_return(
//   const uint16_t *const data
// );
void mock_lbp_feature_expect_generate_scaled_rectangles(void);
void mock_lbp_feature_expect_destroy_feature(void);
void mock_lbp_feature_expect_calculate_vote(const float *const data);
void mock_lbp_feature_verify_complete(void);
void mock_lbp_feature_generate_scaled_rectangles(
	lbp_feature *const self,
  const float *const scales,
  const uint8_t scales_amount
);
void mock_lbp_feature_destroy_feature(lbp_feature *const self);
float mock_lbp_feature_calculate_vote(
  lbp_feature *const self,
  const lbp_feature_arguments *const arguments
);

#endif
