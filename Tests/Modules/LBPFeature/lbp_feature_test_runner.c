#include "unity_fixture.h"

TEST_GROUP_RUNNER(lbp_feature_test)
{
  RUN_TEST_CASE(lbp_feature_test, generate_scaled_rectangles_is_ok);
  RUN_TEST_CASE(lbp_feature_test, calculate_vote_right_value_returned);
}