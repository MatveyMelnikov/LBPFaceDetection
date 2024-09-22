#include <stdio.h>
#include <stdlib.h>
#include "binary_stage_parser_defs.h"
#include "stb_image_defs.h"
#include "face_detector.h"
#include "face_detector_builder.h"
#include "integral_image.h"

#define GET_RGB_PIXEL(image, x, y, w) \
  ({ \
    __typeof__(w) _true_width = 3 * (w); \
    ((image) + (3 * (x)) + (y) * _true_width); \
  })

static uint8_t *image;

static void fill_integral_image(
  FILL_LINE_FUNCTOR,
  integral_image_size image_size
);
static uint16_t rgb_to_greyscale(uint8_t x, uint8_t y, uint8_t image_width);
static uint16_t get_rectangle_summarize(
  const lbp_feature_rectangle *const feature_rectangle
);

int main()
{
  printf("LBP Face Detection\n");

  int width, height, bpp;
  image = stbi_load(
    "../../Data/test_0.jpg",
    &width,
    &height,
    &bpp,
    3
  );

  uint8_t *binary_source = malloc(DEFAULT_BINARY_DATA_SIZE);
  FILE *file = fopen("../../Data/lbpcascade_frontalface.bin", "rb");
	if (binary_source == NULL || file == NULL)
		return -1; 
	fread(binary_source, DEFAULT_BINARY_DATA_SIZE, 1, file);
	fclose(file);

	// stage *const stages = binary_stage_parser_create(
  //   binary_source,
  //   DEFAULT_BINARY_DATA_STAGES_AMOUNT
  // );

  //face_detector_create();

  integral_image_create(
    (integral_image_size) {
      .width = width,
      .height = height
    }
  );
  integral_image_set(fill_integral_image);
  integral_image_calculate();

  face_detector_arguments arguments = (face_detector_arguments) {
    .base_scale = 1.f,
    .position_increment = 0.1f,
    .scale_increment = 1.1f,
    .image_size_x = width,
    .image_size_y = height,
    .min_neighbours = 1U
  };

  
  face_detector_builder_create(
    binary_source,
    DEFAULT_BINARY_DATA_STAGES_AMOUNT,
    get_rectangle_summarize
  );

  face_detector_detect(&arguments);
  face_detector_result result = face_detector_get_result();
  
  for (uint8_t face_index = 0; face_index < result.faces_amount; face_index++)
  {
    area current_face = result.faces[face_index];

    printf(
      "Detected face: x - %d, y - %d, size - %d\n",
      current_face.x,
      current_face.y,
      current_face.size
    );
  }
  
  face_detector_destroy();
  free(binary_source);
  stbi_image_free(image);

  return 0;
}

static void fill_integral_image(
  FILL_LINE_FUNCTOR,
  integral_image_size image_size
)
{
  uint16_t *convertion_buffer = calloc(image_size.width, sizeof(uint16_t));

  for (uint8_t y = 0; y < image_size.height; y++)
  {
    for (uint8_t x = 0; x < image_size.width; x++)
      convertion_buffer[x] = rgb_to_greyscale(x, y, image_size.width);

    fill_line((uint16_t*)convertion_buffer, y);
  }

  free(convertion_buffer);
}

static uint16_t rgb_to_greyscale(uint8_t x, uint8_t y, uint8_t image_width)
{
  const uint8_t *rgb_pixel = GET_RGB_PIXEL(image, x, y, image_width);

  return 0.299f * rgb_pixel[0] + 0.587f * rgb_pixel[1] +
    0.114f * rgb_pixel[2];
}

static uint16_t get_rectangle_summarize(
  const lbp_feature_rectangle *const feature_rectangle
)
{
  integral_image_rectangle_position integral_position = {
    .top_left_corner = {
      .x = feature_rectangle->x,
      .y = feature_rectangle->y
    },
    .bottom_right_corner = {
      .x = feature_rectangle->x + feature_rectangle->width,
      .y = feature_rectangle->y + feature_rectangle->height
    }
  };

  return integral_image_get_rectangle(&integral_position);
}
