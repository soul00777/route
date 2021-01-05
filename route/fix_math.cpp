#include "fix_math.h"
#include "global.h"
#include <stdio.h>

int find(Array arr, int key)
{
	int index = -1;
	for (int i = 0; i < arr._length; i++)
	{
		if (arr[i] == key)
		{
			index = i;
			break;
		}
	}
	return index;
}

Matrix2d::Matrix2d()
{
	_height = 0;
	_width = 0;
	_arr = nullptr;
}

Matrix2d::Matrix2d(int x, int y)
{
	_height = x;
	_width = y;
	_arr = new int[_width * _height];
	for (int i = 0; i < x * y; ++i)
		_arr[i] = -1;
}

Matrix2d::~Matrix2d()
{
	delete[] _arr;
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
	int x = (WIDTH - 1) / 2;
	int y = NUM_B;
	_height = x;
	_width = y;
	_arr = new int[_height * _width];

	int even_cnt = 0;
	int odd_cnt = NUM_B / 2;
	int pos = 0;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < ipow(2, i); ++j)
		{
			for (int k = 0; k < y / ipow(2, i); ++k)
			{
				if (pos % 2 == 0)
				{
					this->set(i, pos, even_cnt + (y / ipow(2, i) * j));
					even_cnt++;
				}
				else
				{
					this->set(i, pos, odd_cnt + (y / ipow(2, i) * j));
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

Array::Array()
{
	_length = 0;
	_arr = nullptr;
}

Array::Array(int x)
{
	_length = x;
	_arr = new int[_length];
	for (int i = 0; i < x; ++i)
		_arr[i] = -1;
}

//Array::~Array()
//{
//	delete[] _arr;
//}

void Array::set_length(int x)
{
	_length = x;
	_arr = new int[_length];
	for (int i = 0; i < x; ++i)
		_arr[i] = -1;
}
