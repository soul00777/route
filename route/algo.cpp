#include "algo.h"
#include "global.h"
#include "fix_math.h"
#include <iostream>

int  division_algo(Matrix2d connection, Array layerIN, Array layerOUT, int n)
{
	int factor = ipow(2, n); //factor = 2^n
	int divCons = NUM_B / factor;

	for (int i = 0; i < layerIN._length; ++i)
		std::cout << layerIN[i] << " ";

	std::cout << std::endl;

	for (int i = 0; i < layerIN._length; ++i)
		std::cout << layerOUT[i] << " ";

	std::cout << std::endl;

	std::cout << "factor: " << factor << std::endl;
	std::cout << "divCons: " << divCons << std::endl;

	Array new_layerIN(NUM_B), new_layerOUT(NUM_B);
	Array new_input(NUM_B), new_output(NUM_B);

	for (int t = 0; t < factor; ++t)
	{
		Matrix2d table(NUM_B / (2 * factor), NUM_B / (2 * factor));
		Matrix2d AorB(NUM_B / (2 * factor), NUM_B / (2 * factor));

		for (int j = 0; j < NUM_B / factor; ++j)
		{
			int offset1 = t * (NUM_B / (2 * factor));
			int offset2 = t * divCons;
			int row = (find(layerIN, layerIN[j + offset2]) / 2) - offset1;
			int pair = a_output[find(a_input, layerIN[j + offset2])];
			int col = (find(layerOUT, pair) / 2) - offset1;
			int b1 = find(layerIN, layerIN[j + offset2]) % 2;
			int b0 = find(layerOUT, pair) % 2;
			int bin = (b1 << 1) + b0;

			table.set(row, col, bin);
		}

		//debug use only//////////
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
		//////////////////////

		for (int i = 0; i < NUM_B / (2 * factor); ++i)
		{
			int state = 0, n0 = 0, n1 = 0, x0 = 0, x1 = 0, y0 = 0, y1 = 0;
			for (int j = 0; j < NUM_B / (2 * factor); ++j)
			{
				if (table.get(j, i) != -1)
				{
					if (state == 0)
					{
						n0 = table.get(j, i);
						x0 = j;
						y0 = i;
						state = 1;
					}
					else if (state == 1)
					{
						n1 = table.get(j, i);
						x1 = j;
						y1 = i;
						state = 2;
					}
				}

				if (j == (NUM_B / (2 * factor) - 1))
				{
					if (state == 2)
					{
						if (n0 > n1)
						{
							AorB.set(x0, y0, 1);
							AorB.set(x1, y1, 0);
						}
						else
						{
							AorB.set(x0, y0, 0);
							AorB.set(x1, y1, 1);
						}
					}
					else if (state == 1)
					{
						AorB.set(x0, y0, 0);
					}
					else
					{
						printf("\nfuck that shit\n");
					}
				}
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
		printf("\n\n");
		//debug/////////////////

		//setup input conf
		for (int i = 0; i < NUM_B / (2 * factor); ++i)
		{
			int offset = t * (NUM_B / factor);
			int offset2 = t * (NUM_B / (2 * factor));
			int row = i;
			int pair = a_output[find(a_input, layerIN[2 * i + offset])];
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
			int pair = a_input[find(a_output, layerOUT[2 * i + offset])];
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
		printf("\nconf table: \n\n");
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

		printf("\nnew input:\n");
		for (int i = 0; i < NUM_B; ++i)
			std::cout << new_input[i] << " ";

		printf("\nnew output:\n");
		for (int i = 0; i < NUM_B; ++i)
			std::cout << new_output[i] << " ";

		printf("\n-----------------------------------------\n");
		//////////////////////////////
	}

	for (int i = 0; i < NUM_B; ++i)
	{
		int temp = new_input[i];
		new_layerIN[connection.get(n, i)] = temp;
		temp = new_output[i];
		new_layerOUT[connection.get(n, i)] = temp;
	}

	printf("\nnew input: \n");
	for (int i = 0; i < NUM_B; ++i)
		std::cout << new_layerIN[i] << " ";
	printf("\nnew output: \n");

	for (int i = 0; i < NUM_B; ++i)
		std::cout << new_layerOUT[i] << " ";
	printf("\n\n");

	if (n + 1 == (WIDTH - 1) / 2)
	{
		Array last_layerIN(NUM_B);
		for (int i = 0; i < NUM_B / 2; ++i)
		{
			int pair = a_output[find(a_input, new_layerIN[2 * i])];
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
		printf("final conf table: \n\n");

		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 5; ++j)
			{
				if (j == 4)
				{
					printf("%d\n", conf[i][j]);
				}
				else
				{
					printf("%d ", conf[i][j]);
				}
			}

		printf("\n\n");
		printf("\nfinal input: \n");
		for (int i = 0; i < NUM_B; ++i)
			std::cout << last_layerIN[i] << " ";
		printf("\nfinal output: \n");

		for (int i = 0; i < NUM_B; ++i)
			std::cout << new_layerOUT[i] << " ";
		printf("\n\n");

		return 0;
	}
	else
		division_algo(connection, new_layerIN, new_layerOUT, n + 1);
}