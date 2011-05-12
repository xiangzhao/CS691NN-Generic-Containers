///*
// * Main.cpp
// *
// *  Created on: Apr 11, 2011
// *      Author: Xiang Zhao
// */
//#include <iostream>
//#include <vector>
//#include "ad_algorithm.h"
//int main() {
//	//	char* pch = "Hello";
//	//	std::cout << *(pch + 1) << std::endl;
//	int t[5] = { 2, 4, 6, 8, 10 };
//	AdaptiveSequence<int> ads;
//	ads.push_back(5);
//	ads.push_back(6);
//	ads.push_back(7);
//	AdaptiveSequence<int>::iterator iterator1 = ads.begin();
//	iterator1++;
//	ads.insert(iterator1, t, t + 4);
//	//	ads.insert(iterator1, 3, 25);
//	//	it++;
//	//	std::cout << *it << std::endl;
//	//	AdaptiveSequence<int>::iterator ite = ads.erase(it);
//	//	std::cout << *ite << std::endl;
//	//	std::cout << *it << std::endl;
//	//	ads.push_back(8);
//	std::cout << *(iterator1) << std::endl;
//	AdaptiveSequence<int>::iterator iterator2 = ads.begin();
//	AdaptiveSequence<int>::iterator iterator3 = ads.end();
//	iterator2++;
//	iterator3--;
//	iterator3--;
//	ads.erase(iterator2, iterator3);
//	std::cout << "-------" << std::endl;
//
//	//	AdaptiveSequence<int>::ite iterator = ads.begin();
//	//	AdaptiveSequence<int>::ite iteratorend = ads.end();
//	//	std::vector<int>::iterator resultite = std::find(
//	//			iterator.vector_random_access_iterator,
//	//			iteratorend.vector_random_access_iterator, 6);
//	for (AdaptiveSequence<int>::iterator iterator = ads.begin(); iterator
//			!= ads.end(); ++iterator) {
//		std::cout << *iterator << std::endl;
//	}
//	//	AdaptiveSequence<int>::iterator result = adalgorithm::find(ads.begin(),
//	//			ads.end(), 6);
//	//	AdaptiveSequence<int>::iterator result = std::find(ads.begin(), ads.end(),
//	//			6);
//	//
//	//	std::cout << *(result++) << std::endl;
//
//	//	ads.getInternals();
//	//	for (int i = 0; i < 10000; i++)
//	//		ads.push_front(i);
//	//	ads.getInternals();
//	//	std::cout << *resultite << std::endl;
//	//	std::cout << *iterator << std::endl;
//	//	ads.getInternals();
//	//	std::cout << *(++iterator) << std::endl;
//	//	std::cout << *(++iterator) << std::endl;
//	//	std::cout << *--iteratorend << std::endl;
//
//	return 0;
//}
#include <vector>
#include <list>
#include <iostream>
#include <time.h>
#include <set>
#include <algorithm>
#include "AdaptiveSequence.h"

const unsigned TEST1 = 1000000;
const unsigned TEST2 = 1000;
/* push back and then push front:
 * Result: Ad-list-deque
 * Time:   380000-410000-170000
 * We already know that deque is best for push front and push back compared to list.
 * The adaptive sequence correctly adapts to deque
 */
void test1() {
	AdaptiveSequence<int> adsequence;
	clock_t t1 = clock();
	for (int j = 0; j < TEST1; j++) {
		adsequence.push_front(j);
		adsequence.push_back(j);
	}
	clock_t t2 = clock();
	std::cout << (float) (t2 - t1) << std::endl;
	std::list<int> l;
	clock_t t3 = clock();
	for (int j = 0; j < TEST1; j++) {
		l.push_front(j);
		l.push_back(j);
	}
	clock_t t4 = clock();
	std::cout << (float) (t4 - t3) << std::endl;
	std::deque<int> d;
	clock_t t5 = clock();
	for (int j = 0; j < TEST1; j++) {
		d.push_front(j);
		d.push_back(j);
	}
	clock_t t6 = clock();
	std::cout << (float) (t6 - t5) << std::endl;
}
/* push back and then sort
 * Result: ad-deque-list-vector
 * Time:   330000-1030000-140000-330000
 * The performance is better than deque, but significantly lower than vector and list
 */
void test2() {
	AdaptiveSequence<int> adsequence;
	clock_t t1 = clock();
	for (int j = TEST2; j >= 0; j--) {
		adsequence.push_back(j);
		adsequence.sort();
	}
	clock_t t2 = clock();
	std::cout << (float) (t2 - t1) << std::endl;
	std::deque<int> d;
	clock_t t3 = clock();
	for (int j = TEST2; j >= 0; j--) {
		d.push_back(j);
		std::sort(d.begin(), d.end());
	}
	clock_t t4 = clock();
	std::cout << (float) (t4 - t3) << std::endl;
	std::list<int> l;
	clock_t t5 = clock();
	for (int j = TEST2; j >= 0; j--) {
		l.push_back(j);
		l.sort();
	}
	clock_t t6 = clock();
	std::cout << (float) (t6 - t5) << std::endl;
	std::vector<int> v;
	clock_t t7 = clock();
	for (int j = TEST2; j >= 0; j--) {
		v.push_back(j);
		std::sort(v.begin(), v.end());
	}
	clock_t t8 = clock();
	std::cout << (float) (t8 - t7) << std::endl;
}
int main() {
	test1();
	test2();
	//	AdaptiveSequence<int> l;
	//	l.push_back(4);
	//	l.push_back(3);
	//	l.push_back(7);
	//	l.sort();
	//	for (AdaptiveSequence<int>::iterator it = l.begin(); it != l.end(); it++)
	//		std::cout << *it << std::endl;
}

//const unsigned N = 3;
//
//extern bool some_test;
//
////	ads.getInternals();
////	std::cout << *resultite << std::endl;
////	std::cout << *iterator << std::endl;
////	ads.getInternals();
////	std::cout << *(++iterator) << std::endl;
////	std::cout << *(++iterator) << std::endl;
////	std::cout << *--iteratorend << std::endl;
//
//std::set<int> get_set(int) {
//	std::set<int> s;
//	for (int i = 0; i < N; ++i)
//		while (!s.insert(std::rand()).second)
//			;
//	if (some_test)
//		return s;
//	return std::set<int>();
//}
//
//AdaptiveSequence<std::set<int> > generate() {
//	AdaptiveSequence<std::set<int> > v;
//	for (int i = 0; i < N; ++i)
//		v.push_back(get_set(i));
//	if (some_test)
//		return v;
//	return AdaptiveSequence<std::set<int> > ();
//}
//
//float time_it() {
//	clock_t t1, t2, t3, t4;
//	clock_t t0 = clock();
////	{
//		AdaptiveSequence<std::set<int> > v = generate();
//		t1 = clock();
//		std::cout << "construction took " << (float) ((t1 - t0)
//				/ (double) CLOCKS_PER_SEC) << std::endl;
//		//		std::sort(v.begin(), v.end());
//		//		v.sort();
//		//		t2 = clock();
//		//		std::cout << "sort took " << (float) ((t2 - t1)
//		//				/ (double) CLOCKS_PER_SEC) << std::endl;
//		//		std::rotate(v.begin(), v.begin() + v.size() / 2, v.end());
//		//		t3 = clock();
//		//		std::cout << "rotate took " << (float) ((t3 - t2)
//		//				/ (double) CLOCKS_PER_SEC) << std::endl;
//	//	}
//	t4 = clock();
//	std::cout << "destruction took " << (float) ((t4 - t3)
//			/ (double) CLOCKS_PER_SEC) << std::endl;
//	std::cout << "done" << std::endl;
//	return (float) ((t4 - t0) / (double) CLOCKS_PER_SEC);
//}
//
//int main() {
//	std::cout << "N = " << N << '\n';
//	float t = time_it();
//	std::cout << "Total time = " << t << '\n';
//}
//
//bool some_test = true;
