#include "sim.h"
#include <iostream>

void conv2d_1()
{
	for (int i = 0; i < 24; ++i)
		if (i % 5 == 0)
		{
			for (int j = 0; j < 24; ++j)
			{
				if (j % 5 == 0)
				{
					for (int ii = 0; ii < 5; ++ii)
						for (int jj = 0; jj < 5; ++jj)
						{
							printf("%d ", (i + ii) * 28 + j + jj);
						}
					printf("\n");
				}
			}
		}
}
