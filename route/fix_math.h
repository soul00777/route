#pragma once
#include <vector>

class Matrix2d
{
public:
	size_t _height = 0;
	size_t _width = 0;
	int* _arr = new int[_width * _height];
	int index(int x, int y) { return (int)(x * _width + y); }
	Matrix2d();
	Matrix2d(size_t x, size_t y);
	~Matrix2d();

	void set(int x, int y, int val);
	int get(int x, int y);
	void init_fix_connection();
};

//class Array
//{
//public:
//	size_t _length = 0;
//	int* _arr = new int[_length];
//	Array();
//	Array(int x);
//	//~Array();
//
//	/*int& operator[](int i)
//	{
//		if (i > _length - 1)
//		{
//			printf("Index out of bounds\n");
//			return _arr[0];
//		}
//		return _arr[i];
//	}*/
//	void set_length(int x);
//	//int get(int x, int y);*/
//
//	class Proxy
//	{
//		Array& a;
//		int idx;
//	public:
//		Proxy(Array& a, int idx) : a(a), idx(idx) {}
//		int& operator= (int x) { a._arr[idx] = x; return a._arr[idx]; }
//		bool operator== (int x) { return (a._arr[idx] == x); }
//		bool operator!= (int x) { return !(a._arr[idx] == x); }
//		operator int() const { return a._arr[idx]; }
//	};
//
//	Proxy operator[] (int index) { return Proxy(*this, index); }
//};

//int find(Array arr, int key);
size_t find(std::vector<int> arr, int key);
int ipow(int, int);
int log2(int i);