#pragma once
#include "fix_math.h"
#include <vector>

extern constexpr size_t NUM_B = 32;
constexpr size_t log2c(int i)
{
	size_t cnt = 0;
	while (i >>= 1) ++cnt;

	return cnt;
}
extern constexpr size_t HEIGHT = NUM_B / 2;
extern constexpr size_t WIDTH = 2 * log2c(NUM_B) - 1;


extern int conf[HEIGHT][WIDTH];
extern std::vector<int> input;
extern std::vector<int> output;
extern Matrix2d fix_connection;
