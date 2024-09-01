#include "integral_image.h"
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Static variables ----------------------------------------------------------

static uint16_t *integral_image = NULL;
static integral_image_size image_size = { 0 };

// Defines -------------------------------------------------------------------

#define GET_PIXEL(arr, x, y) \
	arr[(x) + (y) * image_size.width]

// Static prototypes ---------------------------------------------------------

static void integral_image_set_data(const uint8_t *const image_data);
static void integral_image_calculate_initial_column_and_line(void);
static void integral_image_calculate_top_line(void);
static void integral_image_calculate_start_column(void);
static void integral_image_calculate_other_elements(void);

// Implementations -----------------------------------------------------------

void integral_image_create(integral_image_size size)
{
  image_size = (integral_image_size) {
    .height = size.height + 1,
    .width = size.width + 1
  };
  integral_image = (uint16_t*)calloc(
    image_size.width * image_size.height,
    sizeof(uint16_t)
  );
}

void integral_image_set(
  void (*fill_integral_image)(
    void (*set_data)(const uint8_t *const),
    integral_image_size
  )
)
{
  fill_integral_image(
    integral_image_set_data,
    (integral_image_size) {
      .width = image_size.width - 1,
      .height = image_size.height - 1
    }
  );
}

static void integral_image_set_data(const uint8_t *const image_data)
{
  integral_image_size image_data_size = (integral_image_size){
    .width = image_size.width - 1,
    .height = image_size.height - 1
  };

  // for (uint16_t y = 0; y < image_size.height; y++)
	// {
  //   //uint8_t y_offset = y * image_data_line_width;

	// 	memcpy(
  //     integral_image + 1 + (1 + y) * image_size.height,
  //     image_data + y * image_data_line_width,
  //     image_data_line_width
  //   );
	// }

  for (uint16_t y = 0; y < image_data_size.height; y++)
  {
    for (uint16_t x = 0; x < image_data_size.height; x++)
    {
      integral_image[(x + 1) + (y + 1) * image_size.height] =
        image_data[x + y * image_data_size.width];
    }
  }
}

void integral_image_destroy(void)
{
  free(integral_image);
  integral_image = NULL;

  image_size = (integral_image_size){ 0 };
}

void integral_image_calculate(void)
{
	integral_image_calculate_initial_column_and_line();
	integral_image_calculate_other_elements();
}

static void integral_image_calculate_initial_column_and_line()
{
	integral_image_calculate_top_line();
	integral_image_calculate_start_column();
}

static void integral_image_calculate_top_line()
{
	for (uint16_t x = 0; x < image_size.width; x++)
	{
		GET_PIXEL(integral_image, x, 0) = 0U;
	}
}

static void integral_image_calculate_start_column()
{
	for (uint16_t y = 1; y < image_size.height; y++)
	{
		GET_PIXEL(integral_image, 0, y) = 0U;
	}
}

static void integral_image_calculate_other_elements()
{
	for (uint16_t x = 1; x < image_size.width; x++)
	{
		for (uint16_t y = 1; y < image_size.height; y++)
		{
			GET_PIXEL(integral_image, x, y) +=
        GET_PIXEL(integral_image, x - 1, y) +
        GET_PIXEL(integral_image, x, y - 1) -
				GET_PIXEL(integral_image, x - 1, y - 1);
		}
	}
}

const uint16_t integral_image_get_rectangle(
	integral_image_rectangle_position* const rectangle
)
{
	uint16_t top_left_corner = GET_PIXEL(
		integral_image,
		rectangle->top_left_corner.x,
		rectangle->top_left_corner.y
	);
	uint16_t bottom_right_corner = GET_PIXEL(
		integral_image,
		rectangle->bottom_right_corner.x,
		rectangle->bottom_right_corner.y
	);
	uint16_t top_right_corner = GET_PIXEL(
		integral_image,
		rectangle->bottom_right_corner.x,
		rectangle->top_left_corner.y
	);
	uint16_t bottom_left_corner = GET_PIXEL(
		integral_image,
		rectangle->top_left_corner.x,
		rectangle->bottom_right_corner.y
	);

	return bottom_right_corner + top_left_corner -
		top_right_corner - bottom_left_corner;
}
