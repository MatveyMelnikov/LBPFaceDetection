#include <stdio.h>
#include <stdlib.h>
#include "binary_stage_parser_defs.h"
#include "stb_image_defs.h"
#include "face_detector.h"
#include "face_detector_builder.h"
#include "integral_image.h"

// Defines -------------------------------------------------------------------

#define GET_RGB_PIXEL(image, x, y, w) \
  ({ \
    ((image) + 3 * ((x) + (y) * (w))); \
  })

#define APPLY_ROUNDING(n) \
  (uint8_t)(n) >> 5

// Static variables ----------------------------------------------------------

static uint8_t *image;
static uint8_t *binary_source;

// Static prototypes ---------------------------------------------------------

static bool open_image(
  const char *file,
  int *const width,
  int *const height
);
static bool open_classifier(
  const char *file_path,
  const uint32_t size
);
static void fill_integral_image(
  FILL_LINE_FUNCTOR,
  const uint8_t *const source_image,
  integral_image_size image_size
);
static uint16_t rgb_to_greyscale(
  uint8_t x,
  uint8_t y,
  const uint8_t *const source_image,
  uint8_t image_width
);
static uint16_t get_rectangle_summarize(
  const lbp_feature_rectangle *const feature_rectangle
);

// Functions -----------------------------------------------------------------

int main()
{
  printf("LBP Face Detection\n");

  int width, height, bpp;
  if (!open_image("../../Data/test_0.jpg", &width, &height))
    return - 1;
  if (
    !open_classifier(
      "../../Data/lbpcascade_frontalface_improved_integer.bin",
      IMPROVED_BINARY_DATA_SIZE
    )
  )
    return - 1;

  integral_image_create(
    (integral_image_size) {
      .width = width,
      .height = height
    },
    fill_integral_image
  );
  integral_image_set(image);
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
    IMPROVED_BINARY_DATA_STAGES_AMOUNT,
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

static bool open_image(
  const char *file,
  int *const width,
  int *const height
)
{
  int bpp;

  image = stbi_load(file, width, height, &bpp, 3U);

  if (image == NULL)
  {
    printf("Error! The image file cannot be opened!\n");
		return false; 
  }

  return true;
}

static bool open_classifier(
  const char *file_path,
  const uint32_t size
)
{
  binary_source = malloc(size);

  FILE *file = fopen(file_path, "rb");
	if (binary_source == NULL || file == NULL)
  {
    printf("Error! The classifier file cannot be opened!\n");
    fclose(file);
		return false;
  }

	(void)fread(binary_source, sizeof(uint8_t), size, file);
	fclose(file);

  return true;
}

static void fill_integral_image(
  FILL_LINE_FUNCTOR,
  const uint8_t *const source_image,
  integral_image_size image_size
)
{
  uint16_t *convertion_buffer = calloc(image_size.width, sizeof(uint16_t));

  for (uint8_t y = 0; y < image_size.height; y++)
  {
    for (uint8_t x = 0; x < image_size.width; x++)
      convertion_buffer[x] = APPLY_ROUNDING(
        rgb_to_greyscale(x, y, source_image, image_size.width)
      );

    fill_line(convertion_buffer, y);
  }

  free(convertion_buffer);
}

static uint16_t rgb_to_greyscale(
  uint8_t x,
  uint8_t y,
  const uint8_t *const source_image,
  uint8_t image_width
)
{
  const uint8_t *rgb_pixel = GET_RGB_PIXEL(source_image, x, y, image_width);

  return 0.3f * rgb_pixel[0] + 0.59f * rgb_pixel[1] +
    0.11f * rgb_pixel[2];
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
