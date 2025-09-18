#pragma once

#include "../Flows_library/flow_definitions.h"
#include "../../Parameters.h"

/**
 * @brief function to calculate a dry scrubber model
 *
 * @input &in = flow representing input raw flue gas
 * @input &par = object with input scrubber parameters
 * @output &out = flow representing calculated cleaned flue gas
 * @output &par = object with calculated output parameters
 */
void dry_scrubber_model(flow &in, flow &out, object &par);
