#include "algo.h"
#include "global.h"
#include "fix_math.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#define DEBUG
#define RELEASE

int  division_algo(std::vector<int> layerIN, std::vector<int> layerOUT, int n)
{
	size_t factor = ipow(2, n); //factor = 2^n
	size_t divCons = NUM_B / factor;

#ifdef DEBUG
	std::cout << "LayerIN & LayerOUT" << std::endl;
	for (int i = 0; i < layerIN.size(); ++i)
		std::cout << std::setw(2) << layerIN[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < layerIN.size(); ++i)
		std::cout << std::setw(2) << layerOUT[i] << " ";
	std::cout << std::endl;
	std::cout << "factor: " << factor << std::endl;
	std::cout << "divCons: " << divCons << std::endl;
#endif

	std::vector<int> new_layerIN(NUM_B), new_layerOUT(NUM_B);
	std::vector<int> new_input(NUM_B), new_output(NUM_B);

	for (size_t t = 0; t < factor; ++t)
	{
		Matrix2d table(NUM_B / (2 * factor), NUM_B / (2 * factor));
		Matrix2d AorB(NUM_B / (2 * factor), NUM_B / (2 * factor));
		std::vector<int> A_list(NUM_B, -1), B_list(NUM_B, -1);

		for (size_t i = 0; i < NUM_B / (2 * factor); ++i)
		{
			size_t offset1 = t * (NUM_B / (2 * factor));
			size_t offset2 = t * divCons;
			int j_break_flag = 0;

			int row1 = (find(layerIN, layerIN[2 * i + offset2]) / 2) - offset1;
			int pair1 = output[find(input, layerIN[2 * i + offset2])];
			int col1 = (find(layerOUT, pair1) / 2) - offset1;
			table.set(row1, col1, layerIN[2 * i + offset2]);

			int row2 = (find(layerIN, layerIN[2 * i + 1 + offset2]) / 2) - offset1;
			int pair2 = output[find(input, layerIN[2 * i + 1 + offset2])];
			int col2 = (find(layerOUT, pair2) / 2) - offset1;
			table.set(row2, col2, layerIN[2 * i + 1 + offset2]);

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

#ifdef DEBUG
		printf("\nRanking table: \n");
		for (int i = 0; i < NUM_B / (2 * factor); ++i)
		{
			for (int j = 0; j < NUM_B / (2 * factor); ++j)
			{
				if (j == (NUM_B / (2 * factor)) - 1)
				{
					printf("%3d\n", table.get(i, j));
				}
				else
				{
					printf("%3d", table.get(i, j));
				}
			}
		}
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
#endif

		//setup input conf
		for (int i = 0; i < NUM_B / (2 * factor); ++i)
		{
			int offset = t * (NUM_B / factor);
			int offset2 = t * (NUM_B / (2 * factor));
			int row = i;
			int pair = output[find(input, layerIN[2 * i + offset])];
			int col = (find(layerOUT, pair) - offset) / 2;

			if (layerIN[(2 * i + offset)] == table.get(row, col))
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

			if (pair == table.get(row, col))
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

#ifdef DEBUG
		printf("Conf table\n");
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

		printf("\nNew layer inputs (before fixed connection):\n");
		for (int i = 0; i < NUM_B; ++i)
			std::cout << new_input[i] << " ";

		printf("\nNew layer outputs (before fixed connection):\n");
		for (int i = 0; i < NUM_B; ++i)
			std::cout << new_output[i] << " ";
#endif
	}

	for (int i = 0; i < NUM_B; ++i)
	{
		int temp = new_input[i];
		new_layerIN[fix_connection.get(n, i)] = temp;
		temp = new_output[i];
		new_layerOUT[fix_connection.get(n, i)] = temp;
	}

#ifdef DEBUG
	printf("\nNew layer inputs (after fixed connection): \n");
	for (int i = 0; i < NUM_B; ++i)
		std::cout << new_layerIN[i] << " ";
	printf("\nNew layer outputs (after fixed connection): \n");

	for (int i = 0; i < NUM_B; ++i)
		std::cout << new_layerOUT[i] << " ";
	printf("\n-----------------------------------------\n");
	printf("\n");
#endif

	if (n + 1 == (WIDTH - 1) / 2)								//last iteration
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

#ifdef RELEASE
		printf("Final conf table: \n");

		for (int i = 0; i < HEIGHT; ++i)
			for (int j = 0; j < WIDTH; ++j)
			{
				if (j == WIDTH-1)
				{
					printf("%3d\n", conf[i][j]);
				}
				else
				{
					printf("%3d ", conf[i][j]);
				}
			}

		printf("\n\n");
		printf("Final input (middle layer): \n");
		for (int i = 0; i < NUM_B; ++i)
			std::cout << std::setw(2) << last_layerIN[i] << " ";
		printf("\nFinal output (middle layer): \n");

		for (int i = 0; i < NUM_B; ++i)
			std::cout << std::setw(2) << new_layerOUT[i] << " ";
		printf("\n\n");
#endif

		return verification(last_layerIN, new_layerOUT);
	}
	else
		return division_algo(new_layerIN, new_layerOUT, n + 1);
}

int verification(std::vector<int> in, std::vector<int> out)
{
	int interlayer_IN[NUM_B][WIDTH];
	int interlayer_OUT[NUM_B][WIDTH];
	int new_fix_connection[WIDTH - 1][NUM_B];

	for (int i = 0; i < (WIDTH - 1) / 2; ++i)
	{
		for (int j = 0; j < NUM_B; ++j)
		{
			new_fix_connection[i][j] = fix_connection.get(i, j);
			new_fix_connection[WIDTH - 2 - i][fix_connection.get(i, j)] = j;
		}
	}

	/*printf("\n");
	for (int i = 0; i < WIDTH - 1; ++i)
	{
		for (int j = 0; j < NUM_B; ++j)
		{
			printf("%d ", new_fix_connection[i][j]);
		}
		printf("\n");
	}*/

	for (int i = 0; i < NUM_B; ++i)
		interlayer_IN[i][0] = input[i];

	for (int j = 0; j < HEIGHT; ++j)
	{
		if (conf[j][0] == 0)
		{
			interlayer_OUT[2 * j][0] = interlayer_IN[2 * j][0];
			interlayer_OUT[2 * j + 1][0] = interlayer_IN[2 * j + 1][0];
		}
		else
		{
			interlayer_OUT[2 * j][0] = interlayer_IN[2 * j + 1][0];
			interlayer_OUT[2 * j + 1][0] = interlayer_IN[2 * j][0];
		}
	}

	for (int l = 1; l < WIDTH; ++l)
	{
		for (int i = 0; i < NUM_B; ++i)
		{
			interlayer_IN[new_fix_connection[l - 1][i]][l] = interlayer_OUT[i][l - 1];
		}

		for (int j = 0; j < HEIGHT; ++j)
		{
			if (conf[j][l] == 0)
			{
				interlayer_OUT[2 * j][l] = interlayer_IN[2 * j][l];
				interlayer_OUT[2 * j + 1][l] = interlayer_IN[2 * j + 1][l];
			}
			else
			{
				interlayer_OUT[2 * j][l] = interlayer_IN[2 * j + 1][l];
				interlayer_OUT[2 * j + 1][l] = interlayer_IN[2 * j][l];
			}
		}
	}

	printf("\n");
	printf("\n");
	for (int i = 0; i < WIDTH; ++i)
	{
		for (int j = 0; j < NUM_B; ++j)
			printf("%d ", interlayer_IN[j][i]);
		printf("\n");
		for (int j = 0; j < NUM_B; ++j)
			printf("%d ", interlayer_OUT[j][i]);
		printf("\n");
	}

	int tf = 1;
	for (int i = 0; i < NUM_B; ++i)
	{
		int pair = output[find(input, interlayer_OUT[i][WIDTH - 1])];
		if (pair != i)
		{
			tf = 0;
			printf("Cannot find the route\n");
			break;
		}
	}

	if (tf == 1)
	{
		printf("Find the route successfully\n");
		return 0;
	}
	else
		return 1;
}

void output_coe()
{
	std::string path[19];
	path[0] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram01.coe";
	path[1] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram02.coe";
	path[2] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram03.coe";
	path[3] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram04.coe";
	path[4] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram05.coe";
	path[5] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram06.coe";
	path[6] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram07.coe";
	path[7] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram08.coe";
	path[8] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram09.coe";
	path[9] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram10.coe";
	path[10] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram11.coe";
	path[11] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram12.coe";
	path[12] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram13.coe";
	path[13] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram14.coe";
	path[14] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram15.coe";
	path[15] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram16.coe";
	path[16] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram17.coe";
	path[17] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram18.coe";
	path[18] = "C:\\Users\\soul0\\source\\repos\\route\\output\\bram19.coe";
	std::ofstream myfile;

	for (int j = 0; j < WIDTH; ++j)
	{
		myfile.open(path[j]);
		if (myfile.is_open())
		{
			myfile << "memory_initialization_radix=2;\n";
			myfile << "memory_initialization_vector=\n";
			for (int i = 0; i < HEIGHT; ++i)
			{
				myfile << conf[i][j];
			}
			myfile << ";";
			myfile.close();
		}
	}
}
