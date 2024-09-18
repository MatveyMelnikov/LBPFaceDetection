#include "face_detector.h"
#include "face_detector_defs.h"
#include "integral_image.h"
#include <stdlib.h>

// Defines -------------------------------------------------------------------

#define MAX(a, b) \
  ({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    (_a > _b) ? _a : _b; \
  })

// Static variables ----------------------------------------------------------

static face_detector_stage_handler stage_handler; 
static stage *stages;
static uint8_t current_stages_amount;
static float *scales;
static uint8_t scales_amount;

static area faces_areas[FACE_DETECTOR_MAX_FACE_AREAS];
static uint8_t faces_amount;
static area result_faces[FACE_DETECTOR_MAX_FACE_AREAS];
static uint8_t result_faces_amount;

// Static prototypes ---------------------------------------------------------

static float *face_detector_calculate_scales(
  float base_scale,
  float scale_increment,
  const uint8_t image_size_x,
  const uint8_t image_size_y
);
static void face_detector_calculate_scaled_features(void);
static void face_detector_detect_faces(
  const face_detector_arguments *const arguments
);
__attribute__((always_inline))
inline static void face_detector_detect_face_window(
  const lbp_feature_arguments *const feature_arguments,
  uint8_t feature_size
);
static void face_detector_merge_faces_areas(uint8_t min_neighbours);
static void face_detector_calculate_indexes_of_neighbours(
  uint8_t *const similar_areas_indexes,
  uint8_t *const number_of_similar_areas
);
static void face_detector_merge_neighbours(
  uint8_t *const similar_areas_indexes,
  const uint8_t number_of_similar_areas,
  uint8_t min_neighbours
);
__attribute__((always_inline))
inline static area face_detector_combine_neighbours(
  const uint8_t *const indexes,
  const uint8_t amount
);

// Implementations -----------------------------------------------------------

void face_detector_create(
  const uint8_t *const binary_classifiers_source,
  const uint8_t stages_amount,
  face_detector_stage_handler handler
)
{
  stage_handler = handler;
  current_stages_amount = stages_amount;

  stages = stage_handler.create_stage_from_source(
    binary_classifiers_source,
    stages_amount
  );
}

void face_detector_destroy()
{
  stage_handler = (face_detector_stage_handler) { 0 };
  free(scales);
  scales_amount = 0;
  current_stages_amount = 0;
}

void face_detector_detect(
  const face_detector_arguments *const arguments
)
{
  face_detector_calculate_scales(
    arguments->base_scale,
    arguments->scale_increment,
    arguments->image_size_x,
    arguments->image_size_y
  );
  face_detector_calculate_scaled_features();

  face_detector_detect_faces(arguments);
  face_detector_merge_faces_areas(arguments->min_neighbours);

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

  while (scales_amount < UINT8_MAX)
  {
    scales[scales_amount++] = current_scale;
    current_scale *= scale_increment;

    if (current_scale > max_scale)
      break;
    if (scales_amount % FACE_DETECTOR_SCALES_MIN_ARRAY_SIZE == 0)
      scales = realloc(scales, scales_amount * 2);
  }

  return scales;
}

static void face_detector_calculate_scaled_features()
{
  for (uint8_t i = 0; i < current_stages_amount; i++)
  {
    stage_handler.calculate_scaled_features(
      &stages[i],
      scales,
      scales_amount
    );
  }
}

static void face_detector_detect_faces(
  const face_detector_arguments *const arguments
)
{
  lbp_feature_arguments feature_arguments = { 0 };

  for (uint8_t scale_index = 0; scale_index < scales_amount; scale_index++)
  {
    uint8_t feature_size = scales[scale_index] * FACE_DETECTOR_FEATURE_SIZE;
    uint8_t step = scales[scale_index] * arguments->position_increment;
    feature_arguments.scale_index = scale_index;

    for (
      ;
      feature_arguments.offset_x < 
        arguments->image_size_x - feature_size + 1;
      feature_arguments.offset_x += step
    )
    {
      for (
        feature_arguments.offset_y = 0;
        feature_arguments.offset_y < 
          arguments->image_size_y - feature_size + 1;
        feature_arguments.offset_y += step
      )
      {
        face_detector_detect_face_window(&feature_arguments, feature_size);
      }
    }
  }
}

__attribute__((always_inline))
inline static void face_detector_detect_face_window(
  const lbp_feature_arguments *const feature_arguments,
  uint8_t feature_size
)
{
  for (uint8_t i = 0; i < current_stages_amount; i++)
  {
    if (
      !stage_handler.calculate_prediction(&stages[i], feature_arguments)
    )
    {
      return;
    }
  }

  if (faces_amount >= FACE_DETECTOR_MAX_FACE_AREAS)
    return;

  faces_areas[faces_amount++] = (area) {
    .x = feature_arguments->offset_x,
    .y = feature_arguments->offset_y,
    .size = feature_size
  };
}

static void face_detector_merge_faces_areas(uint8_t min_neighbours)
{
  uint8_t similar_areas_indexes[FACE_DETECTOR_MAX_FACE_AREAS] = { 0 };
  uint8_t number_of_similar_areas = 0;
  
  face_detector_calculate_indexes_of_neighbours(
    similar_areas_indexes,
    &number_of_similar_areas
  );
  face_detector_merge_neighbours(
    similar_areas_indexes,
    number_of_similar_areas,
    min_neighbours
  );
}

static void face_detector_calculate_indexes_of_neighbours(
  uint8_t *const similar_areas_indexes,
  uint8_t *const number_of_similar_areas
)
{
  for (
    uint8_t area_index = 0;
    area_index < faces_amount;
    area_index++
  )
  {
    bool found = false;

    // The higher the index, the larger the area. We prefer large areas
    for (
      int16_t comparable_read_index = faces_amount - 1;
      comparable_read_index >= 0;
      comparable_read_index--
    )
    {
      if (
        area_is_similar(
          &faces_areas[comparable_read_index],
          &faces_areas[area_index]
        )
      )
      {
        found = true;
        similar_areas_indexes[area_index] =
          similar_areas_indexes[comparable_read_index];
        break;
      }

      if (!found)
      {
        similar_areas_indexes[area_index] = *number_of_similar_areas;
        (*number_of_similar_areas)++;
      }
    }
  }
}

static void face_detector_merge_neighbours(
  uint8_t *const similar_areas_indexes,
  const uint8_t number_of_similar_areas,
  uint8_t min_neighbours
)
{
  uint8_t neighbours_group_indexes[FACE_DETECTOR_MAX_FACE_AREAS];

  for (
    uint8_t area_index = 0;
    area_index < faces_amount;
    area_index++
  )
  {
    uint8_t neighbours_in_group_amount = 0;
    
    for (
      int8_t comparable_area_index = 0;
      comparable_area_index < number_of_similar_areas;
      comparable_area_index++
    )
    {
      if (similar_areas_indexes[comparable_area_index] == area_index)
      {
        neighbours_group_indexes[neighbours_in_group_amount++] =
          comparable_area_index;
      }
    }

    if (neighbours_in_group_amount < min_neighbours)
      continue;
    if (neighbours_in_group_amount == 0)
      continue;

    result_faces[result_faces_amount++] = face_detector_combine_neighbours(
      neighbours_group_indexes,
      neighbours_in_group_amount
    );
  }
}

__attribute__((always_inline))
inline static area face_detector_combine_neighbours(
  const uint8_t *const indexes,
  const uint8_t amount
)
{
  area result = { 0 };

  for (uint8_t i = 0; i < amount; i++)
  {
    area current_area = faces_areas[indexes[i]];

    result.x += current_area.x;
    result.y += current_area.y;
    result.size += current_area.size;
  }

  result.x /= amount;
  result.y /= amount;
  result.size /= amount;

  return result;
}

face_detector_result face_detector_get_result(void)
{
  return (face_detector_result) {
    .faces = result_faces,
    .faces_amount = result_faces_amount
  };
}
