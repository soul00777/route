#include "global.h"

int conf[HEIGHT][WIDTH];
std::vector<int> input { 0, 1, 2 ,3 ,4 ,5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
std::vector<int> output{ 6, 23, 26, 5, 20, 4, 16, 24, 2, 0, 9, 1, 17, 19, 30, 12, 3, 14, 15, 21, 29, 13, 8, 31, 22, 7, 27, 10, 11, 18, 25, 28 };

Matrix2d fix_connection;

//std::vector<int> input{ 0, 1, 2 ,3 ,4 ,5, 6, 7 };
//std::vector<int> output{ 3, 0, 7, 1, 4, 6, 2, 5 };