#include <stdio.h>
#include <stdlib.h>
#include "binary_stage_parser.h"
#include "binary_stage_parser_defs.h"
#include "stb_image_defs.h"

int main()
{
  printf("LBP Face Detection\n");

  int width, height, bpp;
  uint8_t* rgb_image = stbi_load("../../Data/test.jpg", &width, &height, &bpp, 3);

  stbi_image_free(rgb_image);

  uint8_t *binary_source = malloc(DEFAULT_BINARY_DATA_SIZE);
	
  FILE *file = fopen("../../Data/lbpcascade_frontalface.bin", "rb");
	if (binary_source == NULL || file == NULL)
		return -1; 

	fread(binary_source, DEFAULT_BINARY_DATA_SIZE, 1, file);

	fclose(file);

	const stage *stages = binary_stage_parser_create(
    binary_source,
    DEFAULT_BINARY_DATA_STAGES_AMOUNT
  );

  return 0;
}
