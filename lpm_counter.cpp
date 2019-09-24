/*
 * lpm_counter.cpp
 *
 *  Created on: 23/09/2019
 *      Author: joaob
 */

#include "lpm_counter.h"

int lpm_counter::counter(int load, int countValue) {

	if (load)
		return countValue++;
	else
		return 0;
}

