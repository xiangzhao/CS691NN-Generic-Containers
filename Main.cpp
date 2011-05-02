/*
 * Main.cpp
 *
 *  Created on: Apr 11, 2011
 *      Author: Xiang Zhao
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include "AdaptiveSequence.h"
int main() {
	//	char* pch = "Hello";
	//	std::cout << *(pch + 1) << std::endl;
	AdaptiveSequence<int> ads;
	ads.push_front(5);
	ads.push_front(6);
	ads.push_front(7);
	AdaptiveSequence<int>::iterator iterator1 = ads.begin();
	ads.push_front(8);
	std::cout << *iterator1 << std::endl;

	//	AdaptiveSequence<int>::ite iterator = ads.begin();
	//	AdaptiveSequence<int>::ite iteratorend = ads.end();
	//	std::vector<int>::iterator resultite = std::find(
	//			iterator.vector_random_access_iterator,
	//			iteratorend.vector_random_access_iterator, 6);
	for (AdaptiveSequence<int>::iterator iterator = ads.begin(); iterator
			!= ads.end(); ++iterator) {
		std::cout << *iterator << std::endl;
	}
	ads.getInternals();
	for (int i = 0; i < 10000; i++)
		ads.push_front(i);
	ads.getInternals();
	//	std::cout << *resultite << std::endl;
	//	std::cout << *iterator << std::endl;
	//	ads.getInternals();
	//	std::cout << *(++iterator) << std::endl;
	//	std::cout << *(++iterator) << std::endl;
	//	std::cout << *--iteratorend << std::endl;

	return 0;
}
