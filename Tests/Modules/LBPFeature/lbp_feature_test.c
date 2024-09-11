#include "unity_fixture.h"
#include "lbp_feature.h"
#include <stddef.h>

// Defines -------------------------------------------------------------------

enum {
  SCALES_AMOUNT = 6U
};

// Static variables ----------------------------------------------------------

static lbp_feature feature = { 0 };

// Static functions ----------------------------------------------------------

// Tests ---------------------------------------------------------------------

TEST_GROUP(lbp_feature_test);

TEST_SETUP(lbp_feature_test)
{
  feature = (lbp_feature) {
    .left_value = 0.234f,
    .right_value = 0.567f,
    .masks = { 0, 1, 2, 3, 4, 5, 6, 7 },
    .scaled_rectangles = NULL,
    .scaled_rectangles_amount = 0,
    .rectangles = {
      { .x = 6, .y = 5, .width = 4, .height = 3 },
      { .x = 10, .y = 5, .width = 4, .height = 3 },
      { .x = 14, .y = 5, .width = 4, .height = 3 },
      { .x = 6, .y = 8, .width = 4, .height = 3 },
      { .x = 10, .y = 8, .width = 4, .height = 3 },
      { .x = 14, .y = 8, .width = 4, .height = 3 },
      { .x = 6, .y = 11, .width = 4, .height = 3 },
      { .x = 10, .y = 11, .width = 4, .height = 3 },
      { .x = 14, .y = 11, .width = 4, .height = 3 }
    }
  };
}

TEST_TEAR_DOWN(lbp_feature_test)
{
  lbp_feature_destroy(&feature);
}

TEST(lbp_feature_test, generate_scaled_rectangles_is_ok)
{
  float scales[SCALES_AMOUNT] = { 1.0f, 1.2f, 1.44f, 1.728f, 2.0736f, 2.48832f };

  lbp_feature_generate_scaled_rectangles(
    &feature,
    (float*)scales,
    SCALES_AMOUNT
  );

  TEST_ASSERT_EQUAL_UINT8(SCALES_AMOUNT, feature.scaled_rectangles_amount);
  for (uint8_t scale_index = 0; scale_index < SCALES_AMOUNT; scale_index++)
  {
    lbp_feature_rectangle *current_scaled_rectangles = 
      feature.scaled_rectangles[scale_index];

    for (
      uint8_t rectangle_index = 0;
      rectangle_index < LBP_FEATURE_RECTANGLES_AMOUNT;
      rectangle_index++
    )
    {
      lbp_feature_rectangle current_rectangle =
        feature.rectangles[rectangle_index];
      float current_scale = scales[scale_index];

      lbp_feature_rectangle expected_scaled_rectangle = 
        (lbp_feature_rectangle) {
          .x = current_rectangle.x * current_scale,
          .y = current_rectangle.y * current_scale,
          .width = current_rectangle.width * current_scale,
          .height = current_rectangle.height * current_scale
        };

      TEST_ASSERT_EQUAL_UINT8_ARRAY(
        (uint8_t*)&expected_scaled_rectangle,
        (uint8_t*)&current_scaled_rectangles[rectangle_index],
        sizeof(lbp_feature_rectangle)
      );
    }
  }
}
