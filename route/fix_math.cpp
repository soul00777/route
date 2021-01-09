#include "fix_math.h"
#include "global.h"
#include <iostream>

Matrix2d::Matrix2d()
{
	_height = 0;
	_width = 0;
	_arr.resize(0);
}

Matrix2d::Matrix2d(size_t x, size_t y)
{
	_height = x;
	_width = y;
	_arr.resize(_height * _width, -1);
}

Matrix2d::~Matrix2d()
{
	_arr.~vector();
}

void Matrix2d::set(int x, int y, int val)
{
	_arr[index(x, y)] = val;
}

int Matrix2d::get(int x, int y)
{
	return _arr[index(x, y)];
}

void Matrix2d::init_fix_connection()
{
	size_t x = (WIDTH - 1) / 2;
	size_t y = NUM_B;
	_height = x;
	_width = y;
	_arr.resize(_height * _width);

	int even_cnt = 0;
	int odd_cnt = NUM_B / 2;
	size_t pos = 0;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < ipow(2, i); ++j)
		{
			for (int k = 0; k < y / ipow(2, i); ++k)
			{
				if (pos % 2 == 0)
				{
					_arr[index(i, pos)] = even_cnt + (y / ipow(2, i) * j);
					even_cnt++;
				}
				else
				{
					_arr[index(i, pos)] = odd_cnt + (y / ipow(2, i) * j);
					odd_cnt++;
				}
				++pos;
			}
			even_cnt = 0;
			odd_cnt = NUM_B / ipow(2, i + 1);
		}
		even_cnt = 0;
		odd_cnt = NUM_B / ipow(2, i + 2);
		pos = 0;
	}
}

size_t find(std::vector<int> arr, int key)
{
	size_t index = 0;
	for (int i = 0; i < arr.size(); i++)
	{
		if (arr[i] == key)
		{
			index = i;
			break;
		}
	}
	return index;
}

int ipow(int base, int exp)
{
	int result = 1;
	while (true)
	{
		if (exp & 1)
			result *= base;
		exp >>= 1;
		if (!exp)
			break;
		base *= base;
	}

	return result;
}

int log2(int i)
{
		int cnt = 0;
		while (i >>= 1) ++cnt;
		return cnt;
}
