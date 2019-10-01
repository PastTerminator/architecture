/*
 * lpm_counter.cpp
 *
 *  Created on: 23/09/2019
 *      Author: joaovicente
 */

#include "lpm_counter.h"

int lpm_counter::counter(int load, int countValue) {

	if (load)
		return countValue++;
	else
		return 0;
}


int main(int argc, char *argv[]) {
	lpm_counter object1;
	
	int returnFromCounter = object1.counter(1, 10);

	cout << "teste " << returnFromCounter;

}
