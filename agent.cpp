#include "constant.h"
using namespace constants;

// every AI is given board state 'b' as an input to return a column number
int random_ai(char b[b_h][b_w]) { return rand() % 7 + 1; };

