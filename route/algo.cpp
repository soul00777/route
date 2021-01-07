#include "algo.h"
#include "global.h"
#include "fix_math.h"
#include <iostream>
#include <iomanip>

int  division_algo(std::vector<int> layerIN, std::vector<int> layerOUT, int n)
{
	size_t factor = ipow(2, n); //factor = 2^n
	size_t divCons = NUM_B / factor;

	//debug use only//////////
	std::cout << "LayerIN & LayerOUT" << std::endl;
	for (int i = 0; i < layerIN.size(); ++i)
		std::cout << std::setw(2) << layerIN[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < layerIN.size(); ++i)
		std::cout << std::setw(2) << layerOUT[i] << " ";
	std::cout << std::endl;
	std::cout << "factor: " << factor << std::endl;
	std::cout << "divCons: " << divCons << std::endl;
	//////////////////////////

	std::vector<int> new_layerIN(NUM_B), new_layerOUT(NUM_B);
	std::vector<int> new_input(NUM_B), new_output(NUM_B);

	for (size_t t = 0; t < factor; ++t)
	{
		Matrix2d table(NUM_B / (2 * factor), NUM_B / (2 * factor));
		Matrix2d AorB(NUM_B / (2 * factor), NUM_B / (2 * factor));
		std::vector<int> A_list(NUM_B, -1), B_list(NUM_B, -1);

		for (int j = 0; j < NUM_B / factor; ++j)
		{
			int offset1 = t * (NUM_B / (2 * factor));
			int offset2 = t * divCons;
			int row = (find(layerIN, layerIN[j + offset2]) / 2) - offset1;
			int pair = output[find(input, layerIN[j + offset2])];
			int col = (find(layerOUT, pair) / 2) - offset1;
			int b1 = find(layerIN, layerIN[j + offset2]) % 2;
			int b0 = find(layerOUT, pair) % 2;
			int bin = (b1 << 1) + b0;

			table.set(row, col, bin);
		}

		//debug use only//////////
		//for (int i = 0; i < NUM_B / (2 * factor); ++i)
		//{
		//	for (int j = 0; j < NUM_B / (2 * factor); ++j)
		//	{
		//		if (j == (NUM_B / (2 * factor)) - 1)
		//		{
		//			printf("%3d\n", table.get(i, j));
		//		}
		//		else
		//		{
		//			printf("%3d", table.get(i, j));
		//		}
		//	}
		//}
		//////////////////////

		for (size_t i = 0; i < NUM_B / (2 * factor); ++i)
		{
			size_t offset1 = t * (NUM_B / (2 * factor));
			size_t offset2 = t * divCons;
			int j_break_flag = 0;

			int row1 = (find(layerIN, layerIN[2 * i + offset2]) / 2) - offset1;
			int pair1 = output[find(input, layerIN[2 * i + offset2])];
			int col1 = (find(layerOUT, pair1) / 2) - offset1;
			int row2 = (find(layerIN, layerIN[2 * i + 1 + offset2]) / 2) - offset1;
			int pair2 = output[find(input, layerIN[2 * i + 1 + offset2])];
			int col2 = (find(layerOUT, pair2) / 2) - offset1;
			AorB.set(row1, col1, 0);
			AorB.set(row2, col2, 1);
			A_list[i] = col1;
			B_list[i] = col2;
			for (size_t j = 0; j < i; ++j)
			{
				if (col1 == A_list[j]) //single conflict
				{
					AorB.set(row1, col1, 1);
					AorB.set(row2, col2, 0);
					A_list[i] = col2;
					B_list[i] = col1;
					for (size_t k = 0; k < i; ++k)
					{
						if (col2 == A_list[k]) //double conflict, k = previous conflicted row
						{
							for (int x = 0; x < NUM_B / (2 * factor); ++x)
							{
								if (AorB.get(k, x) == 0)
								{
									AorB.set(k, x, 1);
									B_list[k] = x;
								}
								else if (AorB.get(k, x) == 1)
								{
									AorB.set(k, x, 0);
									A_list[k] = x;
								}
							}
							//previous case exchanged
							//need exit j loop, go back to last row (i)
							i = k;
							j_break_flag = 1;
							break; //exit k loop
						}
					}
				}

				if (col2 == B_list[j] && j_break_flag == 0) //single conflict
				{
					AorB.set(row1, col1, 1);
					AorB.set(row2, col2, 0);
					A_list[i] = col2;
					B_list[i] = col1;
					for (size_t k = 0; k < i; ++k)
					{
						if (col1 == B_list[k]) //double conflict, k = previous conflicted row
						{
							for (int x = 0; x < NUM_B / (2 * factor); ++x)
							{
								if (AorB.get(k, x) == 0)
								{
									AorB.set(k, x, 1);
									B_list[k] = x;
								}
								else if (AorB.get(k, x) == 1)
								{
									AorB.set(k, x, 0);
									A_list[k] = x;
								}
							}
							//previous case exchanged
							//need exit j loop, go back to last row (i)
							i = k;
							j_break_flag = 1;
							break; //exit k loop
						}
					}
				}

				if (j_break_flag == 1)
					break; //exit j loop
			}
		}

		//debug/////////////////
		printf("AorB table: \n");
		for (int i = 0; i < NUM_B / (2 * factor); ++i)
		{
			for (int j = 0; j < NUM_B / (2 * factor); ++j)
			{
				if (j == (NUM_B / (2 * factor) - 1))
				{
					printf("%c\n", AorB.get(i, j) + 65);
				}
				else
				{
					printf("%c ", AorB.get(i, j) + 65);
				}
			}
		}
		printf("\n");
		//debug/////////////////

		//setup input conf
		for (int i = 0; i < NUM_B / (2 * factor); ++i)
		{
			int offset = t * (NUM_B / factor);
			int offset2 = t * (NUM_B / (2 * factor));
			int row = i;
			int pair = output[find(input, layerIN[2 * i + offset])];
			int col = (find(layerOUT, pair) - offset) / 2;
			int b1 = find(layerIN, layerIN[2 * i + offset]) % 2;
			int b0 = find(layerOUT, pair) % 2;
			int bin = (b1 << 1) + b0;

			if (bin == table.get(row, col))
			{
				if (AorB.get(row, col) == 0)
				{
					conf[i + offset2][n] = 0;
					int temp = layerIN[2 * i + offset];
					new_input[2 * i + offset] = temp;
					temp = layerIN[2 * i + offset + 1];
					new_input[2 * i + offset + 1] = temp;
				}
				else if (AorB.get(row, col) == 1)
				{
					conf[i + offset2][n] = 1;
					int temp = layerIN[2 * i + offset];
					new_input[2 * i + offset + 1] = temp;
					temp = layerIN[2 * i + offset + 1];
					new_input[2 * i + offset] = temp;
				}
			}
			else
			{
				if (AorB.get(row, col) == 0)
				{
					conf[i + offset2][n] = 1;
					int temp = layerIN[2 * i + offset];
					new_input[2 * i + offset + 1] = temp;
					temp = layerIN[2 * i + offset + 1];
					new_input[2 * i + offset] = temp;
				}
				else if (AorB.get(row, col) == 1)
				{
					conf[i + offset2][n] = 0;
					int temp = layerIN[2 * i + offset];
					new_input[2 * i + offset] = temp;
					temp = layerIN[2 * i + offset + 1];
					new_input[2 * i + offset + 1] = temp;
				}
			}
		}

		//setup output conf
		for (int i = 0; i < NUM_B / (2 * factor); ++i)
		{
			int offset = t * (NUM_B / factor);
			int offset2 = t * (NUM_B / (2 * factor));
			int col = i;
			int pair = input[find(output, layerOUT[2 * i + offset])];
			int row = (find(layerIN, pair) - offset) / 2;
			int b1 = find(layerIN, pair) % 2;
			int b0 = find(layerOUT, layerOUT[2 * i + offset]) % 2;
			int bin = (b1 << 1) + b0;

			if (bin == table.get(row, col))
			{
				if (AorB.get(row, col) == 0)
				{
					conf[i + offset2][WIDTH - n - 1] = 0;
					int temp = layerOUT[2 * i + offset];
					new_output[2 * i + offset] = temp;
					temp = layerOUT[2 * i + offset + 1];
					new_output[2 * i + offset + 1] = temp;
				}
				else if (AorB.get(row, col) == 1)
				{
					conf[i + offset2][WIDTH - n - 1] = 1;
					int temp = layerOUT[2 * i + offset];
					new_output[2 * i + offset + 1] = temp;
					temp = layerOUT[2 * i + offset + 1];
					new_output[2 * i + offset] = temp;
				}
			}
			else
			{
				if (AorB.get(row, col) == 0)
				{
					conf[i + offset2][WIDTH - n - 1] = 1;
					int temp = layerOUT[2 * i + offset];
					new_output[2 * i + offset + 1] = temp;
					temp = layerOUT[2 * i + offset + 1];
					new_output[2 * i + offset] = temp;
				}
				else if (AorB.get(row, col) == 1)
				{
					conf[i + offset2][WIDTH - n - 1] = 0;
					int temp = layerOUT[2 * i + offset];
					new_output[2 * i + offset] = temp;
					temp = layerOUT[2 * i + offset + 1];
					new_output[2 * i + offset + 1] = temp;
				}
			}
		}

		//debug///////////////////
		printf("conf table\n");
		for (int i = 0; i < HEIGHT; ++i)
			for (int j = 0; j < WIDTH; ++j)
			{
				if (j == WIDTH - 1)
				{
					printf("%3d\n", conf[i][j]);
				}
				else
				{
					printf("%3d ", conf[i][j]);
				}
			}

		printf("\nnew intermediate layer input:\n");
		for (int i = 0; i < NUM_B; ++i)
			std::cout << new_input[i] << " ";

		printf("\nnew intermediate layer output:\n");
		for (int i = 0; i < NUM_B; ++i)
			std::cout << new_output[i] << " ";

		printf("\n-----------------------------------------\n");
		//////////////////////////////
	}

	for (int i = 0; i < NUM_B; ++i)
	{
		int temp = new_input[i];
		new_layerIN[fix_connection.get(n, i)] = temp;
		temp = new_output[i];
		new_layerOUT[fix_connection.get(n, i)] = temp;
	}

	printf("\nnew final layer inputs: \n");
	for (int i = 0; i < NUM_B; ++i)
		std::cout << new_layerIN[i] << " ";
	printf("\nnew final layer outputs: \n");

	for (int i = 0; i < NUM_B; ++i)
		std::cout << new_layerOUT[i] << " ";
	printf("\n\n");

	if (n + 1 == (WIDTH - 1) / 2)
	{
		std::vector<int> last_layerIN(NUM_B, -1);
		for (int i = 0; i < NUM_B / 2; ++i)
		{
			int pair = output[find(input, new_layerIN[2 * i])];
			if (pair == new_layerOUT[2 * i])
			{
				conf[i][n + 1] = 0;
				int temp = new_layerIN[2 * i];
				last_layerIN[2 * i] = temp;
				temp = new_layerIN[2 * i + 1];
				last_layerIN[2 * i + 1] = temp;
			}
			else
			{
				conf[i][n + 1] = 1;
				int temp = new_layerIN[2 * i];
				last_layerIN[2 * i + 1] = temp;
				temp = new_layerIN[2 * i + 1];
				last_layerIN[2 * i] = temp;
			}
		}

		//debug/////
		printf("final conf table: \n");

		for (int i = 0; i < HEIGHT; ++i)
			for (int j = 0; j < WIDTH; ++j)
			{
				if (j == WIDTH-1)
				{
					printf("%d\n", conf[i][j]);
				}
				else
				{
					printf("%d ", conf[i][j]);
				}
			}

		printf("\n\n");
		printf("final input (middle layer): \n");
		for (int i = 0; i < NUM_B; ++i)
			std::cout << std::setw(2) << last_layerIN[i] << " ";
		printf("\nfinal output (middle layer): \n");

		for (int i = 0; i < NUM_B; ++i)
			std::cout << std::setw(2) << new_layerOUT[i] << " ";
		printf("\n\n");

		return 0;
	}
	else
		division_algo(new_layerIN, new_layerOUT, n + 1);
}