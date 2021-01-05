#include "init.h"
#include "global.h"
#include "fix_math.h"
#include <stdio.h>

void init_conf()
{
	for (int i = 0; i < HEIGHT; ++i)
		for (int j = 0; j < WIDTH; ++j)
			conf[i][j] = -1;

	a_input.set_length(NUM_B);
	a_output.set_length(NUM_B);
	for (int i = 0; i < NUM_B; ++i)
	{
		a_input[i] = input[i];
		a_output[i] = output[i];
	}
}