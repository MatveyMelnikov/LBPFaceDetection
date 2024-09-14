#include "face_detector.h"
#include "face_detector_defs.h"
#include "binary_stage_parser.h"
#include "integral_image.h"
#include <stdlib.h>

// Defines -------------------------------------------------------------------

#define MAX(a, b) \
  ({ \
    typeof (a) _a = (a); \
    typeof (b) _b = (b); \
    (_a > _b) ? _a : _b; \
  })

// Static variables ----------------------------------------------------------

static stage *stages;
static uint8_t current_stages_amount;
static float *scales;
static uint8_t scales_amount;

// Static prototypes ---------------------------------------------------------

static float *face_detector_calculate_scales(
  float base_scale,
  float scale_increment,
  const uint8_t image_size_x,
  const uint8_t image_size_y
);
static void face_detector_calculate_scaled_features(void);

// Implementations -----------------------------------------------------------

void face_detector_create(
  const uint8_t *const binary_classifiers_source,
  const uint8_t stages_amount
)
{
  stages = binary_stage_parser_create(
    binary_classifiers_source,
    stages_amount
  );
  current_stages_amount = stages_amount;
}

void face_detector_destroy()
{
  free(scales);
  scales_amount = 0;
  current_stages_amount = 0;
}

area *const face_detector_detect(
  const face_detector_arguments *const arguments
)
{
  // integral_image_create(
  //   (integral_image_size) {
  //     .width = arguments->image_size_x,
  //     .height = arguments->image_size_y
  //   }
  // ); // ?
  face_detector_calculate_scales(
    arguments->base_scale,
    arguments->scale_increment,
    arguments->image_size_x,
    arguments->image_size_y
  );

  free(scales);
  integral_image_destroy();
}

static float *face_detector_calculate_scales(
  float base_scale,
  float scale_increment,
  const uint8_t image_size_x,
  const uint8_t image_size_y
)
{
  float max_scale = MAX(image_size_x, image_size_y) /
    FACE_DETECTOR_FEATURE_SIZE;
  float current_scale = base_scale;
  scales = calloc(FACE_DETECTOR_SCALES_MIN_ARRAY_SIZE, sizeof(float));
  scales_amount = 0;

  while ((scales_amount + 1) < UINT8_MAX)
  {
    scales[scales_amount] = current_scale;
    current_scale *= scale_increment;

    if (current_scale > max_scale)
      break;
    if ((scales_amount + 1) % FACE_DETECTOR_SCALES_MIN_ARRAY_SIZE == 0)
      scales = realloc(scales, scales_amount * 2);
    
    scales_amount++;
  }

  return scales;
}

static void face_detector_calculate_scaled_features()
{
  for (uint8_t i = 0; i < current_stages_amount; i++)
  {
    
  }
}
