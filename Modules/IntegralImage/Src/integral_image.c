#include "integral_image.h"
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

// Static variables ----------------------------------------------------------

static uint16_t *integral_image = NULL;
static uint16_t *integral_image_of_squares = NULL;
static integral_image_size image_size = { 0 };

// Defines -------------------------------------------------------------------

#define GET_PIXEL(arr, x, y) \
	arr[(x) + (y) * image_size.width]

// Static prototypes ---------------------------------------------------------

static void integral_image_calculate_initial_column_and_line(
	const uint16_t* const image
);
static void integral_image_calculate_top_line(const uint16_t* const image);
static void integral_image_calculate_start_column(
	const uint16_t* const image
);
static void integral_image_calculate_other_elements(
	const uint16_t* const image
);
static const uint16_t integral_image_calculate_rectangle(
  const uint16_t *const internal_integral_image,
	const integral_image_rectangle_position* const rectangle
);

// Implementations -----------------------------------------------------------

void integral_image_create(integral_image_size size)
{
  image_size = size;
  integral_image = (uint16_t*)calloc(
    image_size.width * image_size.height,
    sizeof(uint16_t)
  );
  integral_image_of_squares = (uint16_t*)calloc(
    image_size.width * image_size.height,
    sizeof(uint16_t)
  );
}

void integral_image_destroy(void)
{
  free(integral_image);
  integral_image = NULL;

  image_size = (integral_image_size){ 0 };
}

void integral_image_calculate(const uint16_t* const image)
{
	integral_image_calculate_initial_column_and_line(image);
	integral_image_calculate_other_elements(image);
}

static void integral_image_calculate_initial_column_and_line(
	const uint16_t* const image
)
{
	GET_PIXEL(integral_image, 0, 0) = GET_PIXEL(image, 0, 0);
  GET_PIXEL(integral_image_of_squares, 0, 0) = 
    powl(GET_PIXEL(image, 0, 0), 2U);

	integral_image_calculate_top_line(image);
	integral_image_calculate_start_column(image);
}

static void integral_image_calculate_top_line(const uint16_t* const image)
{
	for (uint16_t x = 1; x < image_size.width; x++)
	{
		GET_PIXEL(integral_image, x, 0) = GET_PIXEL(image, x, 0) +
			GET_PIXEL(integral_image, x - 1, 0);

    GET_PIXEL(integral_image_of_squares, x, 0) =
      powl(GET_PIXEL(image, x, 0), 2U) +
			GET_PIXEL(integral_image_of_squares, x - 1, 0);
	}
}

static void integral_image_calculate_start_column(const uint16_t* const image)
{
	for (uint16_t y = 1; y < image_size.height; y++)
	{
		GET_PIXEL(integral_image, 0, y) = GET_PIXEL(image, 0, y) +
			GET_PIXEL(integral_image, 0, y - 1);

    GET_PIXEL(integral_image_of_squares, 0, y) = 
      powl(GET_PIXEL(image, 0, y), 2U) +
			GET_PIXEL(integral_image_of_squares, 0, y - 1);
	}
}

static void integral_image_calculate_other_elements(
	const uint16_t* const image
)
{
	for (uint16_t x = 1; x < image_size.width; x++)
	{
		for (uint16_t y = 1; y < image_size.height; y++)
		{
			GET_PIXEL(integral_image, x, y) = 
        (GET_PIXEL(image, x, y) +
				GET_PIXEL(integral_image, x - 1, y) +
        GET_PIXEL(integral_image, x, y - 1)) -
				GET_PIXEL(integral_image, x - 1, y - 1);

      GET_PIXEL(integral_image_of_squares, x, y) = 
        (powl(GET_PIXEL(image, x, y), 2U) +
				GET_PIXEL(integral_image_of_squares, x - 1, y) +
        GET_PIXEL(integral_image_of_squares, x, y - 1)) -
				GET_PIXEL(integral_image_of_squares, x - 1, y - 1);
		}
	}
}

const uint16_t* const integral_image_get()
{
	return integral_image;
}

const uint16_t integral_image_get_rectangle(
	integral_image_rectangle_position* const rectangle
)
{
	return integral_image_calculate_rectangle(integral_image, rectangle);
}

const uint16_t integral_image_get_rectangle_of_squares(
	integral_image_rectangle_position *const rectangle
)
{
  return integral_image_calculate_rectangle(
    integral_image_of_squares,
    rectangle
  );
}

static const uint16_t integral_image_calculate_rectangle(
  const uint16_t *const internal_integral_image,
	const integral_image_rectangle_position* const rectangle
)
{
  uint16_t top_left_corner = GET_PIXEL(
		internal_integral_image,
		rectangle->top_left_corner.x,
		rectangle->top_left_corner.y
	);
	uint16_t bottom_right_corner = GET_PIXEL(
		internal_integral_image,
		rectangle->bottom_right_corner.x,
		rectangle->bottom_right_corner.y
	);
	uint16_t top_right_corner = GET_PIXEL(
		internal_integral_image,
		rectangle->bottom_right_corner.x,
		rectangle->top_left_corner.y
	);
	uint16_t bottom_left_corner = GET_PIXEL(
		internal_integral_image,
		rectangle->top_left_corner.x,
		rectangle->bottom_right_corner.y
	);

	return bottom_right_corner + top_left_corner -
		top_right_corner - bottom_left_corner;
}
