#include <iostream>
#include "global.h"
#include "init.h"
#include "fix_math.h"
#include "algo.h"

int main()
{
	init_conf();
	Matrix2d fix_con;
	fix_con.init_fix_connection();
	printf("Input/Output\n");
	for (int i = 0; i < NUM_B; ++i)
		printf("%d ", input[i]);
	printf("\n");
	for (int i = 0; i < NUM_B; ++i)
		printf("%d ", output[i]);
	printf("\n");

	printf("height: %zu\n", fix_con._height);
	printf("width: %zu\n", fix_con._width);
	printf("HEIGHT: %d\n", HEIGHT);
	printf("WIDTH: %d\n", WIDTH);

	for (int i = 0; i < NUM_B; ++i)
	{
		printf("%d, ", fix_con.get(0, i));
	}
	printf("\n");
	for (int i = 0; i < NUM_B; ++i)
	{
		printf("%d, ", fix_con.get(1, i));
	}
	printf("\n");
	printf("\n");

	division_algo(fix_con, a_input, a_input, 0);
	//for (int i = 0; i < NUM_B; ++i)
	//{
	//	printf("%d, ", fix_con.get(2, i));
	//}
	/*Array test(NUM_B);
	for (int i = 0; i < NUM_B; ++i)
	{
		test[i] = fix_con.get(0, i);
		
	}
	for (int i = 0; i < NUM_B; ++i)
	{
		if (test[i] == 5)
		{
			printf("fuck that is 5\n");
			break;
		}
		else
		{
			std::cout << test[i] << " ";
		}
	}*/


	return 0;
}
