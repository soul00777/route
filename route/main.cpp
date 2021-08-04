#include <iostream>
#include <algorithm>
#include "global.h"
#include "init.h"
#include "algo.h"
#include "sim.h"

int main()
{
	init_conf();
	fix_connection.init_fix_connection();
	input.resize(NUM_B);
	output.resize(NUM_B);
	for (int i = 0; i < NUM_B; ++i)
	{
		input[i] = i;
		output[i] = i;
	}
	
	int magic[5] = { 2,7,12,17,22 };
	int offsetM[25] = { -58,-57,-56,-55,-54,-30,-29,-28,-27,-26,-2,-1,0,1,2,26,27,28,29,30,54,55,56,57,58 };
	std::vector<int> labelOUT;
	labelOUT.resize(NUM_B);
	int startR = 784;
	int endR = 959;
	int mark = 0;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			int tmp = magic[i] * 28 + magic[j];
			for (int c = 0; c < 25; ++c)
			{
				labelOUT[(i * 5 + j) * 32 + c] = tmp + offsetM[c];
				mark = (i * 5 + j) * 32 + c;
			}
			for (int r = 0; r < 7; ++r)
			{
				labelOUT[mark + 1 + r] = startR;
				startR = startR + 1;
			}
		}
	}

	//for (int i = 0; i < 800; ++i)
	//{
	//	printf("%3d ", labelOUT[i]);
	//}

	printf("\n\n");

	std::vector<int> vtmp(1024);

	std::copy(labelOUT.begin(), labelOUT.end(), vtmp.begin());

	std::sort(vtmp.begin(), vtmp.end());

	for (int i = 0; i < 1024; ++i)
	{
		printf("%3d ", vtmp[i]);
	}

	std::vector<int> miss(0);

	int val = 0;

	for (int i = 224; i < 1024; ++i)
	{
		if (vtmp[i + 1] == val + 1)
			val = val + 1;
		else
		{
			miss.push_back(val + 1);
			i = i - 1;
			val = val + 1;
		}
			
	}

	printf("\n\n");
	printf("%d ", miss.size());
	//for (int i = 0; i < miss.size(); ++i)
	//{
	//	printf("%d ", miss[i]);
	//}


	//for (int i = 0; i < 1024; ++i)
	//{

	//}

	//printf("Input/Output\n");
	//for (int i = 0; i < NUM_B; ++i)
	//	printf("%4d ", input[i]);
	//printf("\n");
	//for (int i = 0; i < NUM_B; ++i)
	//	printf("%4d ", output[i]);

	//printf("\n--Division routing algorithm start--\n");
	//if (division_algo(input, input, 0) == 0)
	//{
	//	//output_coe();
	//	return 0;
	//}
	//else
	//	return 1;
}
