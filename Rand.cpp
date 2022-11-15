#include "Rand.h"
#include <stdlib.h>

int RandNum(int min, int max,OPTION option) {

	if ((max + 1 - min) == 0) {
		return 1;
	}

	int number = (rand() % (max + 1 - min)) + min;

	if (option == PLUSMINUS) {

		int plusOrMinus = rand() % 2;

		if (plusOrMinus == 0) {
			return number;
		}
		else {
			return -number;
		}

	}

	if (option == BINARY) {

		int minOrMax = rand() % 2;

		if (minOrMax == 0) {
			return min;
		}
		else {
			return max;
		}

	}

	return number;

}