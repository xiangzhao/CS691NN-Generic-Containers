/*
 * Main.cpp
 *
 *  Created on: Apr 11, 2011
 *      Author: Xiang Zhao
 */
#include <iostream>
#include <vector>
#include "ad_algorithm.h"
int main() {
	//	char* pch = "Hello";
	//	std::cout << *(pch + 1) << std::endl;
	int t[5] = { 2, 4, 6, 8, 10 };
	AdaptiveSequence<int> ads;
	ads.push_back(5);
	ads.push_back(6);
	ads.push_back(7);
	AdaptiveSequence<int>::iterator iterator1 = ads.begin();
	iterator1++;
	ads.insert(iterator1, t, t + 4);
	//	ads.insert(iterator1, 3, 25);
	//	it++;
	//	std::cout << *it << std::endl;
	//	AdaptiveSequence<int>::iterator ite = ads.erase(it);
	//	std::cout << *ite << std::endl;
	//	std::cout << *it << std::endl;
	//	ads.push_back(8);
	std::cout << *(iterator1) << std::endl;

	//	AdaptiveSequence<int>::ite iterator = ads.begin();
	//	AdaptiveSequence<int>::ite iteratorend = ads.end();
	//	std::vector<int>::iterator resultite = std::find(
	//			iterator.vector_random_access_iterator,
	//			iteratorend.vector_random_access_iterator, 6);
	for (AdaptiveSequence<int>::iterator iterator = ads.begin(); iterator
			!= ads.end(); ++iterator) {
		std::cout << *iterator << std::endl;
	}
	//	AdaptiveSequence<int>::iterator result = adalgorithm::find(ads.begin(),
	//			ads.end(), 6);
	//	AdaptiveSequence<int>::iterator result = std::find(ads.begin(), ads.end(),
	//			6);
	//
	//	std::cout << *(result++) << std::endl;

	//	ads.getInternals();
		for (int i = 0; i < 10000; i++)
			ads.push_front(i);
	//	ads.getInternals();
	//	std::cout << *resultite << std::endl;
	//	std::cout << *iterator << std::endl;
	//	ads.getInternals();
	//	std::cout << *(++iterator) << std::endl;
	//	std::cout << *(++iterator) << std::endl;
	//	std::cout << *--iteratorend << std::endl;

	return 0;
}
