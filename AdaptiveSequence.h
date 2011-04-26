/*
 * AdaptiveSequence.h
 *
 *  Created on: Apr 11, 2011
 *      Authors: Xiang Zhao, Eli Gottlieb
 */

#ifndef ADAPTIVESEQUENCE_H_
#define ADAPTIVESEQUENCE_H_

#include <stdlib.h>
#include <cmath>
#include <list>
#include <vector>
#include <deque>
#include <memory>
#include "adaptivity.h"

template<typename T, class Allocator = std::allocator<T> > class AdaptiveSequence {
protected:
	enum representation_t {
		LIST, VECTOR, DEQUEUE
	};
	typedef union {
		std::list<T>* list;
		std::vector<T>* vector;
		std::deque<T>* dequeue;
	} contents_representation_t;
	class ContentsADT {
	public:
		representation_t representation;
		contents_representation_t contents;
		ContentsADT(representation_t repr) {
			switch (repr) {
			case LIST:
				contents.list = new std::list<T>();
				break;
			case VECTOR:
				contents.vector = new std::vector<T>();
				break;
			case DEQUEUE:
				contents.dequeue = new std::deque<T>();
				break;
			}
		}
		~ContentsADT() {
			switch (representation) {
			case LIST:
				delete contents.list;
				break;
			case VECTOR:
				delete contents.vector;
				break;
			case DEQUEUE:
				delete contents.dequeue;
				break;
			}
		}
		complexity_class_t complexity(operation_t o, representation_t rep) {
			switch (o) {
			case SORT:
				return NLOGN;
				break;
			case ITERATE_OVER:
				return LINEAR;
				break;
			case ACCESS_ELEMENT:
				switch (rep) {
				case LIST:
					return LINEAR;
					break;
				case VECTOR:
					return CONSTANT;
					break;
				case DEQUEUE:
					return LINEAR;
					break;
				}
				break;
			case ACCESS_FRONT:
				return CONSTANT;
				break;
			case ACCESS_BACK:
				switch (rep) {
				case LIST:
					return LINEAR;
					break;
				case VECTOR:
					return CONSTANT;
					break;
				case DEQUEUE:
					return CONSTANT;
					break;
				}
				break;
			}
		}
	};
protected:
	std::list<operation_t> operations;
	void log_operation(operation_t op) {
		operations.push_front(op);
		attempt_adaptation();
	}
	unsigned int represent_costs(representation_t rep) {
		unsigned int result = 0, length = size();
		for (std::list<operation_t>::iterator i = operations.begin(); i
				!= operations.end(); ++i) {
			switch (internals.complexity(*i, rep)) {
			case CONSTANT:
				result++;
				break;
			case LINEAR:
				result += length;
				break;
			case NLOGN:
				result += length * log(length);
				break;
			}
		}
		return result;
	}
	void attempt_adaptation() {
		unsigned int length = size();
		float list_cost = (represent_costs(LIST) + length) / operations.size();
		float vector_cost = (represent_costs(VECTOR) + length)
				/ operations.size();
		float dequeue_cost = (represent_costs(DEQUEUE) + length)
				/ operations.size();
		switch (internals.representation) {
		case LIST:
			if (vector_cost < list_cost || dequeue_cost < list_cost) {
				if (vector_cost < dequeue_cost) {
					ContentsADT insides(VECTOR);
					insides.contents.vector->resize(length);
					typename std::list<T,Allocator>::iterator iter = internals.contents.list->begin();
					for (int i = 0; i < length; i++) {
						insides.contents.vector->at(i) = *iter;
						iter++;
					}
					//~internals;
					internals = insides;
				} else {
					ContentsADT insides(DEQUEUE);
					insides.contents.dequeue->resize(length);
					typename std::list<T,Allocator>::iterator iter = internals.contents.list->begin();
					for (int i = 0; i < length; i++) {
						insides.contents.dequeue->at(i) = *iter;
						iter++;
					}
					//~internals;
					internals = insides;
				}
				operations.clear();
			}
			break;
		case VECTOR:
			if (list_cost < vector_cost || dequeue_cost < vector_cost) {
				if (list_cost < dequeue_cost) {
					ContentsADT insides(LIST);
					insides.contents.list->resize(length);
					typename std::list<T,Allocator>::iterator iter = insides.contents.list->begin();
					for (int i = 0; i < length; i++) {
						*iter = internals.contents.vector->at(i);
						iter++;
					}
					//~internals;
					internals = insides;
				} else {
					ContentsADT insides(DEQUEUE);
					insides.contents.dequeue->resize(length);
					for (int i = 0; i < length; i++)
						insides.contents.dequeue->at(i)
								= internals.contents.vector->at(i);
					//~internals;
					internals = insides;
				}
				operations.clear();
			}
			break;
		case DEQUEUE:
			if (list_cost < dequeue_cost || vector_cost < dequeue_cost) {
				if (list_cost < vector_cost) {
					ContentsADT insides(LIST);
					insides.contents.list->resize(length);
					typename std::list<T,Allocator>::iterator iter = insides.contents.list->begin();
					for (int i = 0; i < length; i++) {
						*iter = internals.contents.dequeue->at(i);
						iter++;
					}
					//~internals;
					internals = insides;
				} else {
					ContentsADT insides(VECTOR);
					insides.contents.vector->resize(length);
					for (int i = 0; i < length; i++)
						insides.contents.vector->at(i)
								= internals.contents.dequeue->at(i);
					//~internals;
					internals = insides;
				}
				operations.clear();
			}
			break;
		}
	}
	ContentsADT internals;
public:
	typedef Allocator allocator_type;
	typedef typename Allocator::value_type value_type;
	typedef typename Allocator::reference reference;
	typedef typename Allocator::const_reference const_reference;
	typedef typename Allocator::pointer pointer;
	typedef typename Allocator::const_pointer const_pointer;
	typedef typename Allocator::size_type size_type;
	typedef typename Allocator::difference_type difference_type;
	//typedef size_t size_type;
	//typedef ptrdiff_t difference_type;
	//Iterators
	struct ite {
		typename std::list<T>::iterator list_bidirectional_iterator;
		typename std::vector<T>::iterator vector_random_access_iterator;
		typename std::deque<T>::iterator deque_random_access_iterator;
		AdaptiveSequence* currentSequence;
		int tag;
		ite(AdaptiveSequence* sequence) {
			switch (sequence->internals.representation) {
			case LIST:
				tag = 1;
				break;
			case VECTOR:
				tag = 2;
				break;
			case DEQUEUE:
				tag = 3;
				break;
			}
		}
			reference operator*() {
				switch (tag) {
				case 1:
					return *list_bidirectional_iterator;
					break;
				case 2:
					return *vector_random_access_iterator;
					break;
				case 3:
					return *deque_random_access_iterator;
					break;
				}
			}

			ite& operator++() {
				switch (tag) {
				case 1:
					list_bidirectional_iterator++;
					break;
				case 2:
					vector_random_access_iterator++;
					break;
				case 3:
					deque_random_access_iterator++;
					break;
				}
			}
			ite& operator--() {
				switch (tag) {
				case 1:
					list_bidirectional_iterator--;
					break;
				case 2:
					vector_random_access_iterator--;
					break;
				case 3:
					deque_random_access_iterator--;
					break;
				}
			}
			bool operator==(const ite& __i) {
				switch (tag) {
				case 1:
					return list_bidirectional_iterator
							== __i.list_bidirectional_iterator;
					break;
				case 2:
					return vector_random_access_iterator
							== __i.vector_random_access_iterator;
					break;
				case 3:
					return deque_random_access_iterator
							== __i.deque_random_access_iterator;
					break;
				}
			}
			bool operator!=(const ite& __i) {
				switch (tag) {
				case 1:
					return list_bidirectional_iterator
							!= __i.list_bidirectional_iterator;
					break;
				case 2:
					return vector_random_access_iterator
							!= __i.vector_random_access_iterator;
					break;
				case 3:
					return deque_random_access_iterator
							!= __i.deque_random_access_iterator;
					break;
				}
			}

		}
		;
		typedef typename std::list<T>::iterator list_bidirectional_iterator;
		typedef typename std::list<T>::const_iterator
				const_list_bidirectonal_iterator;
		typedef typename std::vector<T>::iterator vector_random_access_iterator;
		typedef typename std::vector<T>::const_iterator
				const_vector_random_access_iterator;
		typedef typename std::deque<T>::iterator deque_random_access_iterator;
		typedef typename std::deque<T>::const_iterator
				const_deque_random_access_iterator;
		typedef std::reverse_iterator<const_vector_random_access_iterator>
				const_reverse_iterator;
		typedef std::reverse_iterator<vector_random_access_iterator>
				reverse_iterator;

		//typedef iterator
		//Bidirectional iterator
		//const_iterator	Constant bidirectional iterator


		//typedef reverse_iterator<iterator> reverse_iterator;
		//typedef reverse_iterator<const_iterator> const_reverse_iterator;

		AdaptiveSequence() :
		internals(VECTOR) {
			//		internals = ContentsADT(LIST);
		}
		virtual ~AdaptiveSequence() {
			//		~internals;
		}

		ite begin() {
			return ite(this);
		}
		//	const_iterator begin() const;
		//	iterator end();
		//	const_iterator end() const;

		//iterator rbegin();
		//const_iterator rbegin() const;
		//iterator rend();
		//const_iterator rend() const;

		bool empty() const {
			switch (internals.representation) {
			case LIST:
				return internals.contents.list->empty();
				break;
			case VECTOR:
				return internals.contents.vector->empty();
				break;
			case DEQUEUE:
				return internals.contents.dequeue->empty();
				break;
			}
			log_operation(ACCESS_FRONT);
		}

		size_type size() {
			switch (internals.representation) {
			case LIST:
				return internals.contents.list->size();
				break;
			case VECTOR:
				return internals.contents.vector->size();
				break;
			case DEQUEUE:
				return internals.contents.dequeue->size();
				break;
			}
			log_operation(ACCESS_BACK);
		}
		size_type max_size() {
			switch (internals.representation) {
			case LIST:
				return internals.contents.list->max_size();
				break;
			case VECTOR:
				return internals.contents.vector->max_size();
				break;
			case DEQUEUE:
				return internals.contents.dequeue->max_size();
				break;
			}
			log_operation(ACCESS_BACK);
		}
		void resize(size_type sz, T c = T()) {
			switch (internals.representation) {
			case LIST:
				internals.contents.list->resize(sz, c);
				break;
			case VECTOR:
				internals.contents.vector->resize(sz, c);
				break;
			case DEQUEUE:
				internals.contents.dequeue->resize(sz, c);
				break;
			}
			log_operation(ITERATE_OVER);
		}

		reference front() {
			switch (internals.representation) {
			case LIST:
				return internals.contents.list->front();
				break;
			case VECTOR:
				return internals.contents.vector->front();
				break;
			case DEQUEUE:
				return internals.contents.dequeue->front();
				break;
			}
			log_operation(ACCESS_FRONT);
		}

		const_reference front() const {
			switch (internals.representation) {
			case LIST:
				return internals.contents.list->front();
				break;
			case VECTOR:
				return internals.contents.vector->front();
				break;
			case DEQUEUE:
				return internals.contents.dequeue->front();
				break;
			}
			log_operation(ACCESS_FRONT);
		}

		reference operator[](size_type n) {
			return at(n);
		}
		const_reference operator[](size_type n) const {
			return at(n);
		}

		const_reference at(size_type n) const {
			switch (internals.representation) {
			case LIST:
				typename std::list<T,Allocator>::iterator iter = internals.contents.list->begin();
				for(int i=0;i<n;i++)
					iter++;
				T& result = *iter;
				return result;
				break;
			case VECTOR:
				return internals.contents.vector->at(n);
				break;
			case DEQUEUE:
				return internals.contents.dequeue->at(n);
				break;
			}
			log_operation(ACCESS_ELEMENT);
		}

		reference at(size_type n) {
			switch (internals.representation) {
			case LIST:
				typename std::list<T,Allocator>::iterator iter = internals.contents.list->begin();
				for(int i=0;i<n;i++)
					iter++;
				T& result = *iter;
				return result;
				break;
			case VECTOR:
				return internals.contents.vector->at(n);
				break;
			case DEQUEUE:
				return internals.contents.dequeue->at(n);
				break;
			}
			log_operation(ACCESS_ELEMENT);
		}

		template<class InputIterator> void assign(InputIterator first,
				InputIterator last);
		void assign(size_type n, const T& u) {
			switch (internals.representation) {
			case LIST:
				internals.contents.list->assign(n, u);
				break;
			case VECTOR:
				internals.contents.vector->assign(n, u);
				break;
			case DEQUEUE:
				internals.contents.dequeue->assign(n, u);
				break;
			}
			log_operation(ITERATE_OVER);
		}

		void push_front(const T& x) {
			switch (internals.representation) {
			case LIST:
				internals.contents.list->push_front(x);
				log_operation(ACCESS_FRONT);
				break;
			case VECTOR:
				int size = internals.contents.vector->size();
				internals.contents.vector->resize(size+1);
				for(int i=0;i<size;i++)
					internals.contents.vector->at(i+1) = internals.contents.vector->at(i);
				internals.contents.vector->at(0) = x;
				log_operation(ITERATE_OVER);
				break;
			case DEQUEUE:
				internals.contents.dequeue->push_front(x);
				log_operation(ACCESS_FRONT);
				break;
			}
		}

		void pop_front() {
			switch (internals.representation) {
			case LIST:
				internals.contents.list->pop_front();
				break;
			case VECTOR:
				internals.contents.vector->pop_front();
				break;
			case DEQUEUE:
				internals.contents.dequeue->pop_front();
				break;
			}
			log_operation(ACCESS_FRONT);
		}

		void push_back(const T& x) {
			switch (internals.representation) {
			case LIST:
				internals.contents.list->push_back(x);
				break;
			case VECTOR:
				internals.contents.vector->push_back(x);
				break;
			case DEQUEUE:
				internals.contents.dequeue->push_back(x);
				break;
			}
			log_operation(ACCESS_BACK);
		}

		void pop_back() {
			switch (internals.representation) {
			case LIST:
				internals.contents.list->pop_back();
				break;
			case VECTOR:
				internals.contents.vector->pop_back();
				break;
			case DEQUEUE:
				internals.contents.dequeue->pop_back();
				break;
			}
			log_operation(ACCESS_BACK);
		}
		//iterator insert(iterator position, const T& x);
		//void insert(iterator position, size_type n, const T& x);
		//template<class InputIterator> void insert(iterator position,
		//		InputIterator first, InputIterator last);
		//iterator erase(iterator position);
		//iterator erase(iterator first, iterator last);
		void swap(AdaptiveSequence<T>& seq);
		void clear() {
			switch (internals.representation) {
			case LIST:
				internals.contents.list->clear();
				break;
			case VECTOR:
				internals.contents.vector->clear();
				break;
			case DEQUEUE:
				internals.contents.dequeue->clear();
				break;
			}
			log_operation(ACCESS_BACK);
		}

		//void splice(iterator position, AdaptiveSequence<T>& x);
		//void splice(iterator position, AdaptiveSequence<T>& x, iterator i);
		//void splice(iterator position, AdaptiveSequence<T>& x, iterator first,
		//		iterator last);
		void remove(const T& value) {
			switch (internals.representation) {
			case LIST:
				internals.contents.list->remove(value);
				break;
			case VECTOR:
				internals.contents.vector->remove(value);
				break;
			case DEQUEUE:
				internals.contents.dequeue->remove(value);
				break;
			}
			log_operation(ITERATE_OVER);
		}

		template<class Predicate> void remove_if(Predicate pred);
		void unique();
		template<class BinaryPredicate> void
		unique(BinaryPredicate binary_pred);

		void merge(AdaptiveSequence<T>& x);
		template<class Compare> void
		merge(AdaptiveSequence<T>& x, Compare comp);

		void sort() {
			switch (internals.representation) {
			case LIST:
				internals.contents.list->sort();
				break;
			case VECTOR:
				internals.contents.vector->sort();
				break;
			case DEQUEUE:
				internals.contents.dequeue->sort();
				break;
			}
			log_operation(SORT);
		}
		template<class Compare> void sort(Compare comp);

		void reverse() {
			switch (internals.representation) {
			case LIST:
				internals.contents.list->reverse();
				break;
			case VECTOR:
				internals.contents.vector->reverse();
				break;
			case DEQUEUE:
				internals.contents.dequeue->reverse();
				break;
			}
			log_operation(ITERATE_OVER);
		}
	};

#endif /* ADAPTIVESEQUENCE_H_ */
