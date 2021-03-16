#pragma once
#include "fix_math.h"
#include <vector>

int division_algo(std::vector<int> layerIN, std::vector<int> layerOUT, int iter);
int verification(std::vector<int> input, std::vector<int> output);
void output_coe();