#include <iostream>
#include "global.h"
#include "init.h"
#include "algo.h"
#include "sim.h"

int main()
{
	init_conf();
	fix_connection.init_fix_connection();
	/*input.resize(NUM_B);
	output.resize(NUM_B);
	for (int i = 0; i < NUM_B; ++i)
	{
		input[i] = i;
		output[i] = NUM_B - 1 - i;
	}*/
	printf("Input/Output\n");
	for (int i = 0; i < NUM_B; ++i)
		printf("%2d ", input[i]);
	printf("\n");
	for (int i = 0; i < NUM_B; ++i)
		printf("%2d ", output[i]);
	printf("\n--Division routing algorithm start--\n");


	if (division_algo(input, input, 0) == 0)
	{
		//output_coe();
		return 0;
	}
	else
		return 1;
}
