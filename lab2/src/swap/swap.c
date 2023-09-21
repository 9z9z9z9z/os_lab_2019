#include "swap.h"

void Swap(char *left, char *right)
{
	char tmpP = *left;
	*left = *right;
	*right = tmpP;
}
