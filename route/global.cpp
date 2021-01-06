#include "global.h"

int conf[HEIGHT][WIDTH];
std::vector<int> input { 0, 1, 2 ,3 ,4 ,5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
std::vector<int> output{ 5, 7, 12, 15, 9, 10, 11, 0, 4, 6, 8, 3, 1, 13, 2, 14 };
Matrix2d fix_connection;

//std::vector<int> input{ 0, 1, 2 ,3 ,4 ,5, 6, 7 };
//std::vector<int> output{ 3, 0, 7, 1, 4, 6, 2, 5 };