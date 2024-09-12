#include "mock_integral_image.h"
#include "mock_defs.h"
#include "unity_fixture.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

// Static variables ----------------------------------------------------------

static char *report_not_init = "Integral image mock not initialized";
static char *report_no_room_for_exp = "No room for expectations in integral "
  "image mock";
static char *report_verify_error = "Verify error in integral image mock. "
  "Expected %u operations, but got %u";
static char *report_kind_error = "Error kind in integral image mock. "
  "Num of expectation %u ";

static expectation *expectations = NULL;
static int set_expectation_count;
static int get_expectation_count;
static int max_expectation_count;

// Static prototypes ---------------------------------------------------------

static void fail_when_no_room_for_expectations(void);
static void fail_when_no_init(void);
static void record_expectation(
  const int kind,
  const uint16_t *const data,
  const uint8_t data_size
);
static void check_kind(
  const expectation *const current_expectation,
  int kind
);

// Implementations -----------------------------------------------------------

void mock_integral_image_create(const uint8_t max_expectations)
{
  if (expectations != NULL)
    free(expectations);
  expectations = calloc(max_expectations, sizeof(expectation));
  max_expectation_count = max_expectations;
}

void mock_integral_image_destroy()
{
  if (expectations)
    free(expectations);
  expectations = NULL;

  set_expectation_count = 0;
  get_expectation_count = 0;
  max_expectation_count = 0;
}

void mock_integral_image_expect_read_then_return(
  const uint16_t *const data
)
{
  fail_when_no_room_for_expectations();
  record_expectation(READ, data, sizeof(uint16_t));
}

static void fail_when_no_room_for_expectations()
{
  fail_when_no_init();
  if (set_expectation_count >= max_expectation_count)
    FAIL(report_no_room_for_exp);
}

static void fail_when_no_init()
{
  if (expectations == NULL)
    FAIL(report_not_init);
}

static void record_expectation(
  const int kind,
  const uint16_t *const data,
  const uint8_t data_size
)
{
  expectations[set_expectation_count].kind = kind;
  expectations[set_expectation_count].data = (const uint8_t*)data;
  expectations[set_expectation_count].data_size = data_size;
  set_expectation_count++;
}

void mock_integral_image_verify_complete()
{
  char *message[sizeof(report_verify_error) + 10];

  if (set_expectation_count == get_expectation_count)
    return;
    
  sprintf(
    (char*)message,
    report_verify_error,
    set_expectation_count,
    get_expectation_count
  );
  FAIL((char*)message);
}

uint16_t mock_integral_image_get_rectangle(
	integral_image_rectangle_position *const rectangle
)
{
  (void)rectangle;
  expectation current_expectation = expectations[get_expectation_count];

  fail_when_no_init();
  check_kind(&current_expectation, READ);

  get_expectation_count++;

  return *(uint16_t*)current_expectation.data;
}

static void check_kind(
  const expectation *const current_expectation,
  int kind
)
{
  char *message[sizeof(report_kind_error) + 10];

  if (current_expectation->kind == kind)
    return;

  sprintf((char*)message, report_kind_error, get_expectation_count);
  FAIL(message);
}