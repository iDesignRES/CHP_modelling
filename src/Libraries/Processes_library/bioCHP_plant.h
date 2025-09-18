#pragma once

#include <string>
#include <vector>

#include "../../Parameters.h"
#include "../Flows_library/flow_definitions.h"

void get_feedstock(std::vector<flow> &f, object &plant);

void bioCHP_plant_model(object &bioCHP);
