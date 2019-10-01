/*
 * lpm_compare.cpp
 *
 *  Created on: 23/09/2019
 *      Author: joaob
 */

#include "lpm_compare.h"

bool lpm_compare::compare(int countValue) {

	if (countValue == magicNumber)
		return true;
	else
		return false;

}


