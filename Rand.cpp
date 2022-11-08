#include "Rand.h"
#include <stdlib.h>

int RandNum(int min, int max,MINUSFLAG minusFlag) {

	int number = (rand() % (max - min)) + min;

	if (minusFlag == ON) {

		int plusOrMinus = rand() % 2;

		if (plusOrMinus == 0) {
			return number;
		}
		else {
			return -number;
		}

	}

	return number;

}