#include "unity_fixture.h"
#include "unity_config.h"
#include <stdio.h>

void unity_config_put_c(uint8_t c);

static void run_all_tests()
{
	RUN_TEST_GROUP(integral_image);
}

int main(int argc, char *argv[])
{
	return UnityMain(argc, argv, run_all_tests);
}

void unity_config_put_c(uint8_t c)
{
	(void)putchar(c);
}
