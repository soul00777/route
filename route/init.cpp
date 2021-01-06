#include "init.h"
#include "global.h"
#include "fix_math.h"
#include <stdio.h>

void init_conf()
{
	for (int i = 0; i < HEIGHT; ++i)
		for (int j = 0; j < WIDTH; ++j)
			conf[i][j] = -1;
}