#include "lbp_feature.h"
#include <stdlib.h>

// Static prototypes ---------------------------------------------------------

static void lbp_feature_allocate_scaled_rectangles(
  lbp_feature *const self,
  const float *const scales,
  const uint8_t scales_amount
);

// Implementations -----------------------------------------------------------

void lbp_feature_generate_scaled_rectangles(
  lbp_feature *const self,
  const float *const scales,
  const uint8_t scales_amount
)
{
  // for (uint8_t i = 0; i < LBP_FEATURE_RECTANGLES_AMOUNT; i++)
  // {
  //   lbp_feature_rectangle current_rectangle = self->rectangles[i];

  //   self->scaled_rectangles[i] = (lbp_feature_rectangle) {
  //     .x = current_rectangle.x * 
  //   }
  // }

  lbp_feature_allocate_scaled_rectangles(self, scales, scales_amount);

  for (uint8_t scale_index = 0; scale_index < scales_amount; scale_index++)
  {
    float current_scale = scales[scale_index];
    lbp_feature_rectangle *current_scaled_rectangles =
      self->scaled_rectangles[scale_index];
    
    for (
      uint8_t rectangle_index = 0;
      rectangle_index < LBP_FEATURE_RECTANGLES_AMOUNT;
      rectangle_index++
    )
    {
      lbp_feature_rectangle current_rectangle =
        self->rectangles[rectangle_index];

      current_scaled_rectangles[rectangle_index] = (lbp_feature_rectangle) {
        .x = current_rectangle.x * current_scale,
        .y = current_rectangle.y * current_scale,
        .width = current_rectangle.width * current_scale,
        .height = current_rectangle.height * current_scale
      };
    }
  }
}

static void lbp_feature_allocate_scaled_rectangles(
  lbp_feature *const self,
  const float *const scales,
  const uint8_t scales_amount
)
{
  self->scaled_rectangles = calloc(sizeof(void*), scales_amount);
  for (uint8_t i = 0; i < scales_amount; i++)
  {
    self->scaled_rectangles[i] = calloc(
      sizeof(lbp_feature_rectangle),
      LBP_FEATURE_RECTANGLES_AMOUNT
    );
  }

  self->scaled_rectangles_amount = scales_amount;
}

float lbp_feature_calculate_vote(
  lbp_feature *const self,
  const lbp_feature_arguments *const arguments
)
{

}

void lbp_feature_destroy(lbp_feature *const self)
{
  if (self->scaled_rectangles_amount == 0)
    return;

  for (
    uint8_t scaled_rectangles_index = 0;
    scaled_rectangles_index < self->scaled_rectangles_amount;
    scaled_rectangles_index++
  )
  {
    free(self->scaled_rectangles[scaled_rectangles_index]);
  }
  free(self->scaled_rectangles);

  self->scaled_rectangles_amount = 0;
}
