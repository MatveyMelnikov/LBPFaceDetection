#include "binary_stage_parser.h"
#include "binary_stage_parser_defs.h"
#include <stddef.h>

// Static variables ----------------------------------------------------------

static uint8_t *binary_source = NULL;
static uint8_t stages_amount_in_source = 0;

// Defines -------------------------------------------------------------------

#define GET_ABSOLUTE_ADDRESS(base_obj_ptr, obj_ptr) \
	((ADDR_TYPE)(obj_ptr) + (ADDR_TYPE)(base_obj_ptr))

#define GET_RELATIVE_ADDRESS(base_obj_ptr, obj_ptr) \
	((ADDR_TYPE)(obj_ptr) - (ADDR_TYPE)(base_obj_ptr))

// Implementations -----------------------------------------------------------

stage *const binary_stage_create(uint8_t* source, uint8_t stages_amount)
{
	binary_source = source;
	stages_amount_in_source = stages_amount;

	for (uint8_t i = 0; i < stages_amount; i++)
	{
		stage* stage = binary_stage_get(i);

		stage->features = (lbp_feature*)GET_ABSOLUTE_ADDRESS(
			binary_source,
			stage->features
		);
	}

	return (stage*)binary_source;
}

void binary_stage_destroy()
{
	binary_source = NULL;
}

stage* const binary_stage_get(uint8_t index)
{
	if (index >= stages_amount_in_source)
		return NULL;

	uint8_t* current_stage_in_source = binary_source + (index * sizeof(stage));

	return (stage*)current_stage_in_source;
}

lbp_feature* const binary_stage_get_features(
	stage* const base_stage,
	uint16_t index
)
{
	if (index >= base_stage->features_amount)
		return NULL;

	lbp_feature* current_features = (lbp_feature*)GET_ABSOLUTE_ADDRESS(
		binary_source,
		base_stage->features
	);

	return current_features + index;
}
