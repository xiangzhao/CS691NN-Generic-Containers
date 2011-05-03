/*
 * ad_algorithm.h
 *
 *  Created on: May 2, 2011
 *      Author: Xiang Zhao
 */
#include <algorithm>
#include "AdaptiveSequence.h"
#ifndef AD_ALGORITHM_H_
#define AD_ALGORITHM_H_

#endif /* AD_ALGORITHM_H_ */
namespace adalgorithm {
template<typename __Tp>
typename AdaptiveSequence<__Tp, std::allocator<__Tp> >::iterator find(typename AdaptiveSequence<__Tp, std::allocator<__Tp> >::iterator in,
		typename AdaptiveSequence<__Tp, std::allocator<__Tp> >::iterator out, const __Tp& __val) {
	if (in.tag == out.tag) {
		typename AdaptiveSequence<__Tp, std::allocator<__Tp> >::iterator result(in.currentSequence);
		switch (in.tag) {
			case 1:
			result.list_bidirectional_iterator = std::find(
					in.list_bidirectional_iterator,
					out.list_bidirectional_iterator, __val);
			break;
			case 2:
			result.vector_random_access_iterator = std::find(
					in.vector_random_access_iterator,
					out.vector_random_access_iterator, __val);
			break;
			case 3:
			result.deque_random_access_iterator = std::find(
					in.deque_random_access_iterator,
					out.deque_random_access_iterator, __val);
			break;

		}
		return result;
	} else
	return 0;

}
}
