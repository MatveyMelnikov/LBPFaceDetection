#ifndef MOCK_DEFS_H
#define MOCK_DEFS_H

#include <stdint.h>

typedef struct
{
  int kind;
  const uint8_t *data;
  uint8_t data_size;
} expectation;

typedef enum
{
  READ,
  ACTIVE,
  INACTIVE,
  GENERATE,
  DESTROY,
  CALCULATE,
  NO_EXPECTED_VALUE = -1
} expectation_type;

#endif