/*
 * Main.cpp
 *
 *  Created on: Apr 11, 2011
 *      Author: Xiang Zhao
 */
#include <iostream>
#include "AdaptiveSequence.h"
int main() {
	//	char* pch = "Hello";
	//	std::cout << *(pch + 1) << std::endl;
	AdaptiveSequence<int> ads;
	ads.push_back(5);
	ads.push_back(6);
	AdaptiveSequence<int>::ite iterator = ads.begin();
	return 0;
}
