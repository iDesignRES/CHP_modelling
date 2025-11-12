#pragma once

#include <string>
#include <vector>

#include "flow_definitions.h"
/**
 * @brief function to mix to flow withy the same type of chemical composition
 *
 * @param f1 1st input flow
 * @param f2 2nd input flow
 *
 * @return flow with the mix of f1 and f2
 */
flow mix_same_type_flows(flow &f1, flow &f2);