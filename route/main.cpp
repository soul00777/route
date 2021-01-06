#include <iostream>
#include "global.h"
#include "init.h"
#include "fix_math.h"
#include "algo.h"

int main()
{
	init_conf();
	fix_connection.init_fix_connection();
	printf("Input/Output\n");
	for (int i = 0; i < NUM_B; ++i)
		printf("%d ", input[i]);
	printf("\n");
	for (int i = 0; i < NUM_B; ++i)
		printf("%d ", output[i]);
	printf("\n--Division routing algorithm start--\n");
	division_algo(input, input, 0);


	return 0;
}
