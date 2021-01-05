#pragma once
#include "fix_math.h"

extern constexpr int NUM_B = 8;
constexpr int log2c(int i)
{
	int cnt = 0;
	while (i >>= 1) ++cnt;

	return cnt;
}
extern constexpr int HEIGHT = NUM_B / 2;
extern constexpr int WIDTH = 2 * log2c(NUM_B) - 1;


extern int conf[HEIGHT][WIDTH];
extern int input[NUM_B];
extern int output[NUM_B];

extern Array a_input;
extern Array a_output;

