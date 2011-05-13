/*
 * AdaptiveSequence.h
 *
 *  Created on: Apr 11, 2011
 *      Authors: Xiang Zhao, Eli Gottlieb
 */

#ifndef ADAPTIVESEQUENCE_H_
#define ADAPTIVESEQUENCE_H_

#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <list>
#include <vector>
#include <deque>
#include <memory>
#include <algorithm>
#include <bits/stl_iterator_base_types.h>
#include "adaptivity.h"

//const int OPSIZE = 100;
template<typename T, class Allocator = std::allocator<T> > class AdaptiveSequence {
protected:
	int opsize;
	enum representation_t {
		LIST = 1, VECTOR = 2, DEQUE = 3
	};
	typedef struct {
		std::list<T>* list;
		std::vector<T>* vector;
		std::deque<T>* deque;
	} contents_representation_t;
	class ContentsADT {
	public:
		representation_t representation;
		contents_representation_t contents;
		ContentsADT(representation_t repr) {
			switch (repr) {
			case LIST:
				contents.list = new std::list<T>();
				representation = LIST;
				break;
			case VECTOR:
				contents.vector = new std::vector<T>();
				representation = VECTOR;
				break;
			case DEQUE:
				contents.deque = new std::deque<T>();
				representation = DEQUE;
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
			case DEQUE:
				delete contents.deque;
				break;
			}
		}
		complexity_class_t complexity(operation_t o, representation_t rep) {
			switch (o) {
			case INSERT:
				switch (rep) {
				case LIST:
					return CONSTANT;
					break;
				case VECTOR:
					return LINEAR;
					break;
				case DEQUE:
					return LINEAR;
					break;
				}
				break;
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
				case DEQUE:
					return LINEAR;
					break;
				}
				break;
			case READ_FRONT:
				return CONSTANT;
				break;
			case WRITE_FRONT:
				switch (rep) {
				case LIST:
					return CONSTANT;
					break;
				case VECTOR:
					return LINEAR;
					break;
				case DEQUE:
					return CONSTANT;
					break;
				}
				break;
			case READ_BACK:
				switch (rep) {
				case LIST:
					return LINEAR;
					break;
				case VECTOR:
					return CONSTANT;
					break;
				case DEQUE:
					return CONSTANT;
					break;
				}
				break;
			case WRITE_BACK:
				switch (rep) {
				case LIST:
					return LINEAR;
					break;
				case VECTOR:
					return CONSTANT;
					break;
				case DEQUE:
					return CONSTANT;
					break;
				}
				break;
			}
		}
	};
public:
	//<<<<<<< HEAD
	//	struct ite;
	void syncIterator();
	//=======
	struct iterator;
	//>>>>>>> refs/remotes/origin/master
	typedef Allocator allocator_type;
	typedef typename Allocator::value_type value_type;
	typedef typename Allocator::reference reference;
	typedef typename Allocator::const_reference const_reference;
	typedef typename Allocator::pointer pointer;
	typedef typename Allocator::const_pointer const_pointer;
	typedef typename Allocator::size_type size_type;
	typedef typename Allocator::difference_type difference_type;
protected:
	unsigned int iteratorVersion;
public:
	//typedef size_t size_type;
	//typedef ptrdiff_t difference_type;
	//Iterators
	struct iterator {
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		//		typedef std::random_access_iterator_tag iterator_category;
		typename std::list<T, Allocator>::iterator list_bidirectional_iterator;
		typename std::vector<T, Allocator>::iterator
				vector_random_access_iterator;
		typename std::deque<T, Allocator>::iterator
				deque_random_access_iterator;
		int list_bidirectional_iterator_counter;
		int vector_random_access_iterator_counter;
		int deque_random_access_iterator_counter;
		unsigned int iteratorVersion;
		AdaptiveSequence* currentSequence;
		representation_t tag;
		iterator(AdaptiveSequence* sequence) {
			list_bidirectional_iterator_counter = 0;
			vector_random_access_iterator_counter = 0;
			deque_random_access_iterator_counter = 0;
			tag = sequence->internals->representation;
			currentSequence = sequence;
			iteratorVersion = sequence->iteratorVersion;
		}
		reference operator*() {
			if(iteratorVersion < currentSequence->iteratorVersion)
				currentSequence->syncIterator(this);
			//			currentSequence->log_operation(ACCESS_ELEMENT);
			switch (tag) {
			case LIST:
				return *list_bidirectional_iterator;
				break;
			case VECTOR:
				return *vector_random_access_iterator;
				break;
			case DEQUE:
				return *deque_random_access_iterator;
				break;
			}
		}

		iterator& operator++() {
			if(iteratorVersion < currentSequence->iteratorVersion)
				currentSequence->syncIterator(this);
			switch (tag) {
			case LIST:
				++list_bidirectional_iterator;
				list_bidirectional_iterator_counter++;
				break;
			case VECTOR:
				++vector_random_access_iterator;
				vector_random_access_iterator_counter++;
				break;
			case DEQUE:
				++deque_random_access_iterator;
				deque_random_access_iterator_counter++;
				break;
			}
			return *this;
		}
		// It should be a new iterator
		iterator operator++(int) {
			if(iteratorVersion < currentSequence->iteratorVersion)
				currentSequence->syncIterator(this);
			switch (tag) {
			case LIST:
				list_bidirectional_iterator++;
				list_bidirectional_iterator_counter++;
				break;
			case VECTOR:
				vector_random_access_iterator++;
				vector_random_access_iterator_counter++;
				break;
			case DEQUE:
				deque_random_access_iterator++;
				deque_random_access_iterator_counter++;
				break;
			}
			return *this;
		}
		iterator& operator--() {
			if(iteratorVersion < currentSequence->iteratorVersion)
				currentSequence->syncIterator(this);
			switch (tag) {
			case LIST:
				--list_bidirectional_iterator;
				break;
			case VECTOR:
				--vector_random_access_iterator;
				break;
			case DEQUE:
				--deque_random_access_iterator;
				break;
			}
			return *this;
		}
		iterator operator--(int) {
			if(iteratorVersion < currentSequence->iteratorVersion)
				currentSequence->syncIterator(this);
			switch (tag) {
			case LIST:
				list_bidirectional_iterator--;
				break;
			case VECTOR:
				vector_random_access_iterator--;
				break;
			case DEQUE:
				deque_random_access_iterator--;
				break;
			}
			return *this;
		}

		bool operator==(const iterator& __i) {
			if(iteratorVersion < currentSequence->iteratorVersion)
				currentSequence->syncIterator(this);
			switch (tag) {
			case LIST:
				return list_bidirectional_iterator
						== __i.list_bidirectional_iterator;
				break;
			case VECTOR:
				return vector_random_access_iterator
						== __i.vector_random_access_iterator;
				break;
			case DEQUE:
				return deque_random_access_iterator
						== __i.deque_random_access_iterator;
				break;
			}

		}
		bool operator!=(const iterator& __i) {
			if(iteratorVersion < currentSequence->iteratorVersion)
				currentSequence->syncIterator(this);
			switch (tag) {
			case LIST:
				return list_bidirectional_iterator
						!= __i.list_bidirectional_iterator;
				break;
			case VECTOR:
				return vector_random_access_iterator
						!= __i.vector_random_access_iterator;
				break;
			case DEQUE:
				return deque_random_access_iterator
						!= __i.deque_random_access_iterator;
				break;
			}
		}
		iterator& operator+=(const difference_type& __n) {
			if(iteratorVersion < currentSequence->iteratorVersion)
				currentSequence->syncIterator(this);
			switch (tag) {
			case LIST:
				for (int k = 0; k < __n; k++)
					list_bidirectional_iterator++;
				list_bidirectional_iterator_counter += __n;
				break;
			case VECTOR:
				vector_random_access_iterator += __n;
				vector_random_access_iterator_counter += __n;
				break;
			case DEQUE:
				deque_random_access_iterator += __n;
				deque_random_access_iterator_counter += __n;
				break;
			}
			currentSequence->log_operation(ACCESS_ELEMENT);
			return *this;
		}
		iterator& operator-=(const difference_type& __n) {
			if(iteratorVersion < currentSequence->iteratorVersion)
				currentSequence->syncIterator(this);
			switch (tag) {
			case LIST:
				for (int k = 0; k < __n; k++)
					--list_bidirectional_iterator;
				list_bidirectional_iterator_counter -= __n;
				break;
			case VECTOR:
				vector_random_access_iterator -= __n;
				vector_random_access_iterator_counter -= __n;
				break;
			case DEQUE:
				deque_random_access_iterator -= __n;
				deque_random_access_iterator_counter -= __n;
				break;
			}
			currentSequence->log_operation(ACCESS_ELEMENT);
			return *this;
		}
		iterator operator+(difference_type __n) {
			if(iteratorVersion < currentSequence->iteratorVersion)
				currentSequence->syncIterator(this);
			*this += __n;
			return *this;

		}
		iterator operator-(difference_type __n) {
			if(iteratorVersion < currentSequence->iteratorVersion)
				currentSequence->syncIterator(this);
			*this -= __n;
			return *this;
		}
	};
	std::deque<operation_t> operations;

	void syncIterator(iterator* it) {
		it->iteratorVersion = iteratorVersion;
		representation_t rep = it->currentSequence->internals->representation;
		if (rep != it->tag) {
			switch (rep) {
			case LIST:
				it->tag = LIST;
				it->list_bidirectional_iterator
						= it->currentSequence->internals->contents.list->begin();
				break;
			case VECTOR:
				it->tag = VECTOR;
				it->vector_random_access_iterator
						= it->currentSequence->internals->contents.vector->begin();
				break;
			case DEQUE:
				it->tag = DEQUE;
				it->deque_random_access_iterator
						= it->currentSequence->internals->contents.deque->begin();
				break;
			}
			switch (it->tag) {
			case LIST:
				it += it->list_bidirectional_iterator_counter;
				it->list_bidirectional_iterator_counter = 0;
				break;
			case VECTOR:
				it += it->vector_random_access_iterator_counter;
				it->vector_random_access_iterator_counter = 0;
				break;
			case DEQUE:
				it += it->deque_random_access_iterator_counter;
				it->deque_random_access_iterator_counter = 0;
				break;
			}
		}
	}
protected:

	void log_operation(operation_t op) {
		operations.push_front(op);
		if (operations.size() >= opsize) {
			attempt_adaptation();
			operations.clear();
		}
	}
	unsigned int represent_costs(representation_t rep) {
		unsigned int result = 0, length = size();
		for (std::deque<operation_t>::iterator i = operations.begin(); i
				!= operations.end(); ++i) {
			switch (internals->complexity(*i, rep)) {
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
		int operations_length = 1000;
		float list_cost = (float) (represent_costs(LIST) + length)
				/ operations_length;
		float vector_cost = (float) (represent_costs(VECTOR) + length)
				/ operations_length;
		float deque_cost = (float) (represent_costs(DEQUE) + length)
				/ operations_length;
		switch (internals->representation) {
		case LIST:
			if (vector_cost < list_cost || deque_cost < list_cost) {
				if (vector_cost < deque_cost) {
					ContentsADT* insides = new ContentsADT(VECTOR);
					std::cout << "ADAPT VECTOR" << std::endl;
					insides->contents.vector->resize(length);
					typename std::list<T, Allocator>::iterator iter =
							internals->contents.list->begin();
					for (int i = 0; i < length; i++) {
						insides->contents.vector->at(i) = *iter;
						iter++;
					}
					delete internals;
					internals = insides;
				} else {
					ContentsADT* insides = new ContentsADT(DEQUE);
					std::cout << "ADAPT DEQUE" << std::endl;
					insides->contents.deque->resize(length);
					typename std::list<T, Allocator>::iterator iter =
							internals->contents.list->begin();
					for (int i = 0; i < length; i++) {
						insides->contents.deque->at(i) = *iter;
						iter++;
					}
					delete internals;
					internals = insides;
				}
				iteratorVersion++;
				opsize = 100;
				operations.clear();
			} else
				opsize *= 2;
			break;
		case VECTOR:
			if (list_cost < vector_cost || deque_cost < vector_cost) {
				if (list_cost < deque_cost) {
					ContentsADT* insides = new ContentsADT(LIST);
					std::cout << "ADAPT LIST" << std::endl;
					insides->contents.list->resize(length);
					typename std::list<T, Allocator>::iterator iter =
							insides->contents.list->begin();
					for (int i = 0; i < length; i++) {
						*iter = internals->contents.vector->at(i);
						iter++;
					}
					delete internals;
					internals = insides;
				} else {
					ContentsADT* insides = new ContentsADT(DEQUE);
					std::cout << "ADAPT DEQUE" << std::endl;
					insides->contents.deque->resize(length);
					for (int i = 0; i < length; i++)
						insides->contents.deque->at(i)
								= internals->contents.vector->at(i);
					delete internals;
					internals = insides;
				}
				iteratorVersion++;
				opsize = 100;
				operations.clear();
			} else
				opsize *= 2;
			break;
		case DEQUE:
			if (list_cost < deque_cost || vector_cost < deque_cost) {
				if (list_cost < vector_cost) {
					ContentsADT* insides = new ContentsADT(LIST);
					std::cout << "ADAPT LIST" << std::endl;
					insides->contents.list->resize(length);
					typename std::list<T, Allocator>::iterator iter =
							insides->contents.list->begin();
					for (int i = 0; i < length; i++) {
						*iter = internals->contents.deque->at(i);
						iter++;
					}
					delete internals;
					internals = insides;
				} else {
					ContentsADT* insides = new ContentsADT(VECTOR);
					std::cout << "ADAPT VECTOR" << std::endl;
					insides->contents.vector->resize(length);
					for (int i = 0; i < length; i++)
						insides->contents.vector->at(i)
								= internals->contents.deque->at(i);
					delete internals;
					internals = insides;
				}
				iteratorVersion++;
				opsize = 100;
				operations.clear();
			} else
				opsize *= 2;
			break;
		}
	}
	ContentsADT* internals;
public:
	//	typedef Allocator allocator_type;
	//	typedef typename Allocator::value_type value_type;
	//	typedef typename Allocator::reference reference;
	//	typedef typename Allocator::const_reference const_reference;
	//	typedef typename Allocator::pointer pointer;
	//	typedef typename Allocator::const_pointer const_pointer;
	//	typedef typename Allocator::size_type size_type;
	//	typedef typename Allocator::difference_type difference_type;
	//typedef size_t size_type;
	//typedef ptrdiff_t difference_type;
	//Iterators
	//	struct iterator {
	//		typename std::list<T>::iterator list_bidirectional_iterator;
	//		typename std::vector<T>::iterator vector_random_access_iterator;
	//		typename std::deque<T>::iterator deque_random_access_iterator;
	//		AdaptiveSequence* currentSequence;
	//		int tag;
	//		iterator(AdaptiveSequence* sequence) {
	//			switch (sequence->internals->representation) {
	//			case LIST:
	//				tag = 1;
	//				break;
	//			case VECTOR:
	//				tag = 2;
	//				break;
	//			case DEQUE:
	//				tag = 3;
	//				break;
	//			}
	//
	//		}
	//		reference operator*() {
	//			switch (tag) {
	//			case 1:
	//				return *list_bidirectional_iterator;
	//				break;
	//			case 2:
	//				return *vector_random_access_iterator;
	//				break;
	//			case 3:
	//				return *deque_random_access_iterator;
	//				break;
	//			}
	//		}
	//
	//		iterator& operator++() {
	//			switch (tag) {
	//			case 1:
	//				list_bidirectional_iterator++;
	//				break;
	//			case 2:
	//				vector_random_access_iterator++;
	//				break;
	//			case 3:
	//				deque_random_access_iterator++;
	//				break;
	//			}
	//			return *this;
	//		}
	//		iterator& operator--() {
	//			switch (tag) {
	//			case 1:
	//				list_bidirectional_iterator--;
	//				break;
	//			case 2:
	//				vector_random_access_iterator--;
	//				break;
	//			case 3:
	//				deque_random_access_iterator--;
	//				break;
	//			}
	//			return *this;
	//
	//		}
	//		bool operator==(const iterator& __i) {
	//			switch (tag) {
	//			case 1:
	//				return list_bidirectional_iterator
	//						== __i.list_bidirectional_iterator;
	//				break;
	//			case 2:
	//				return vector_random_access_iterator
	//						== __i.vector_random_access_iterator;
	//				break;
	//			case 3:
	//				return deque_random_access_iterator
	//						== __i.deque_random_access_iterator;
	//				break;
	//			}
	//
	//		}
	//		bool operator!=(const iterator& __i) {
	//			switch (tag) {
	//			case 1:
	//				return list_bidirectional_iterator
	//						!= __i.list_bidirectional_iterator;
	//				break;
	//			case 2:
	//				return vector_random_access_iterator
	//						!= __i.vector_random_access_iterator;
	//				break;
	//			case 3:
	//				return deque_random_access_iterator
	//						!= __i.deque_random_access_iterator;
	//				break;
	//			}
	//		}
	//
	//	};
	//	typedef typename std::list<T>::iterator list_bidirectional_iterator;
	//	typedef typename std::list<T>::const_iterator
	//			const_list_bidirectonal_iterator;
	//	typedef typename std::vector<T>::iterator vector_random_access_iterator;
	//	typedef typename std::vector<T>::const_iterator
	//			const_vector_random_access_iterator;
	//	typedef typename std::deque<T>::iterator deque_random_access_iterator;
	//	typedef typename std::deque<T>::const_iterator
	//			const_deque_random_access_iterator;
	//	typedef std::reverse_iterator<const_vector_random_access_iterator>
	//			const_reverse_iterator;
	//	typedef std::reverse_iterator<vector_random_access_iterator>
	//			reverse_iterator;

	//	void syncIterator(iterator* it) {
	//		representation_t rep = it->currentSequence->internals->representation;
	//		int tag = it->tag;
	//		//		switch ();
	//
	//	}
	//>>>>>>> refs/remotes/origin/master

public:
	AdaptiveSequence() : iteratorVersion(0) {
		internals = new ContentsADT(VECTOR);
		opsize = 100;
	}
	template<typename _InputIterator> AdaptiveSequence(_InputIterator first,
			_InputIterator last) : iteratorVersion(0) {
		internals = new ContentsADT(VECTOR);
		delete internals->contents.vector;
		internals->contents.vector = new std::vector<T>(first, last);
		opsize = 100;

	}
	/* AdaptiveSequence() :
	 internals(VECTOR) {
	 //		internals = ContentsADT(LIST);
	 }*/
	virtual ~AdaptiveSequence() {
		delete internals;
	}

	iterator begin() {
		iterator itebegin(this);
		switch (internals->representation) {
		case LIST:
			itebegin.list_bidirectional_iterator
					= this->internals->contents.list->begin();
			break;
		case VECTOR:
			itebegin.vector_random_access_iterator
					= this->internals->contents.vector->begin();
			break;
		case DEQUE:
			itebegin.deque_random_access_iterator
					= this->internals->contents.deque->begin();
			break;
		}

		return itebegin;
	}
	iterator end() {
		iterator iteend(this);
		switch (internals->representation) {
		case LIST:
			iteend.list_bidirectional_iterator
					= this->internals->contents.list->end();
			break;
		case VECTOR:
			iteend.vector_random_access_iterator
					= this->internals->contents.vector->end();
			break;
		case DEQUE:
			iteend.deque_random_access_iterator
					= this->internals->contents.deque->end();
			break;
		}

		return iteend;
	}
	//	const_iterator begin() const;
	//	iterator end();
	//	const_iterator end() const;

	//iterator rbegin();
	//const_iterator rbegin() const;
	//iterator rend();
	//const_iterator rend() const;

	bool empty() const {
		log_operation(READ_FRONT);
		switch (internals->representation) {
		case LIST:
			return internals->contents.list->empty();
			break;
		case VECTOR:
			return internals->contents.vector->empty();
			break;
		case DEQUE:
			return internals->contents.deque->empty();
			break;
		}
	}

	size_type size() {
		switch (internals->representation) {
		case LIST:
			return internals->contents.list->size();
			break;
		case VECTOR:
			return internals->contents.vector->size();
			break;
		case DEQUE:
			return internals->contents.deque->size();
			break;
		}
	}
	size_type max_size() {
		switch (internals->representation) {
		case LIST:
			return internals->contents.list->max_size();
			break;
		case VECTOR:
			return internals->contents.vector->max_size();
			break;
		case DEQUE:
			return internals->contents.deque->max_size();
			break;
		}
	}
	void resize(size_type sz, T c = T()) {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->resize(sz, c);
			break;
		case VECTOR:
			internals->contents.vector->resize(sz, c);
			break;
		case DEQUE:
			internals->contents.deque->resize(sz, c);
			break;
		}
		log_operation(ITERATE_OVER);
	}

	reference front() {
		log_operation(READ_FRONT);
		switch (internals->representation) {
		case LIST:
			return internals->contents.list->front();
			break;
		case VECTOR:
			return internals->contents.vector->front();
			break;
		case DEQUE:
			return internals->contents.deque->front();
			break;
		}
	}

	const_reference front() const {
		log_operation(READ_FRONT);
		switch (internals->representation) {
		case LIST:
			return internals->contents.list->front();
			break;
		case VECTOR:
			return internals->contents.vector->front();
			break;
		case DEQUE:
			return internals->contents.deque->front();
			break;
		}
	}

	reference operator[](size_type n) {
		return at(n);
	}
	const_reference operator[](size_type n) const {
		return at(n);
	}

	const_reference at(size_type n) const {
		log_operation(ACCESS_ELEMENT);
		switch (internals->representation) {
		case LIST: {
			typename std::list<T, Allocator>::iterator iter =
					internals->contents.list->begin();
			for (int i = 0; i < n; i++)
				iter++;
			T& result = *iter;
			return result;
			break;
		}
		case VECTOR:
			return internals->contents.vector->at(n);
			break;
		case DEQUE:
			return internals->contents.deque->at(n);
			break;
		}
	}

	reference at(size_type n) {
		log_operation(ACCESS_ELEMENT);
		switch (internals->representation) {
		case LIST: {
			typename std::list<T, Allocator>::iterator iter =
					internals->contents.list->begin();
			for (int i = 0; i < n; i++)
				iter++;
			T& result = *iter;
			return result;
			break;
		}
		case VECTOR:
			return internals->contents.vector->at(n);
			break;
		case DEQUE:
			return internals->contents.deque->at(n);
			break;
		}
	}

	template<class InputIterator> void assign(InputIterator first,
			InputIterator last) {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->assign(first, last);
			break;
		case VECTOR:
			internals->contents.vector->assign(first, last);
			break;
		case DEQUE:
			internals->contents.deque->assign(first, last);
			break;
		}
		log_operation(ITERATE_OVER);
	}
	void assign(size_type n, const T& u) {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->assign(n, u);
			break;
		case VECTOR:
			internals->contents.vector->assign(n, u);
			iteratorVersion++;
			break;
		case DEQUE:
			internals->contents.deque->assign(n, u);
			break;
		}
		log_operation(ITERATE_OVER);
	}
	void push_front(const T& x) {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->push_front(x);
			break;
		case VECTOR: {
			int size = internals->contents.vector->size();
			internals->contents.vector->resize(size + 1);
			for (int i = 0; i < size; i++)
				internals->contents.vector->at(i + 1)
						= internals->contents.vector->at(i);
			internals->contents.vector->at(0) = x;
			iteratorVersion++;
			break;
		}
		case DEQUE:
			internals->contents.deque->push_front(x);
			break;
		}
		log_operation(WRITE_FRONT);
	}
	void pop_front() {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->pop_front();
			break;
		case VECTOR:
			internals->contents.vector->pop_front();
			iteratorVersion++;
			break;
		case DEQUE:
			internals->contents.deque->pop_front();
			break;
		}
		log_operation(READ_FRONT);
	}

	void push_back(const T& x) {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->push_back(x);
			break;
		case VECTOR:
			internals->contents.vector->push_back(x);
			iteratorVersion++;
			break;
		case DEQUE:
			internals->contents.deque->push_back(x);
			break;
		}
		log_operation(WRITE_BACK);
	}

	void pop_back() {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->pop_back();
			break;
		case VECTOR:
			internals->contents.vector->pop_back();
			iteratorVersion++;
			break;
		case DEQUE:
			internals->contents.deque->pop_back();
			break;
		}
		log_operation(READ_BACK);
	}
	iterator insert(iterator& position, const T& __x) {
		log_operation(INSERT);
		iterator insresult(this);
		switch (internals->representation) {
		case LIST:
			insresult.list_bidirectional_iterator
					= internals->contents.list->insert(
							position.list_bidirectional_iterator, __x);
			break;
		case VECTOR:
			assert(position.tag == VECTOR);
			insresult.vector_random_access_iterator
					= internals->contents.vector->insert(
							position.vector_random_access_iterator, __x);
			iteratorVersion++;
			break;
		case DEQUE:
			insresult.deque_random_access_iterator
					= internals->contents.deque->insert(
							position.deque_random_access_iterator, __x);
			break;
		}
		position++;
		return insresult;
	}
	void insert(iterator position, size_type n, const T& __x) {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->insert(
					position.list_bidirectional_iterator, n, __x);
			break;
		case VECTOR:
			internals->contents.vector->insert(
					position.vector_random_access_iterator, n, __x);
			iteratorVersion++;
			break;
		case DEQUE:
			internals->contents.deque->insert(
					position.deque_random_access_iterator, n, __x);
			break;
		}
		log_operation(INSERT);
	}
	template<class InputIterator> void insert(iterator position,
			InputIterator first, InputIterator last) {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->insert(
					position.list_bidirectional_iterator, first, last);
			break;
		case VECTOR:
			internals->contents.vector->insert(
					position.vector_random_access_iterator, first, last);
			break;
		case DEQUE:
			internals->contents.deque->insert(
					position.deque_random_access_iterator, first, last);
			break;
		}
		log_operation(INSERT);
	}
	iterator erase(iterator& position) {
		iterator result(this);
		switch (internals->representation) {
		case LIST:
			result.list_bidirectional_iterator
					= internals->contents.list->erase(
							position.list_bidirectional_iterator);
			break;
		case VECTOR:
			result.vector_random_access_iterator
					= internals->contents.vector->erase(
							position.vector_random_access_iterator);
			break;
		case DEQUE:
			result.deque_random_access_iterator
					= internals->contents.deque->erase(
							position.deque_random_access_iterator);
			break;
		}
		return result;
	}
	iterator erase(iterator first, iterator last) {
		iterator result(this);
		switch (internals->representation) {
		case LIST:
			result.list_bidirectional_iterator
					= internals->contents.list->erase(
							first.list_bidirectional_iterator,
							last.list_bidirectional_iterator);
			break;
		case VECTOR:
			result.vector_random_access_iterator
					= internals->contents.vector->erase(
							first.vector_random_access_iterator,
							last.vector_random_access_iterator);
			break;
		case DEQUE:
			result.deque_random_access_iterator
					= internals->contents.deque->erase(
							first.deque_random_access_iterator,
							last.deque_random_access_iterator);
			break;
		}
		return result;
	}
	void swap(AdaptiveSequence<T>& seq);
	void clear() {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->clear();
			break;
		case VECTOR:
			internals->contents.vector->clear();
			break;
		case DEQUE:
			internals->contents.deque->clear();
			break;
		}
		log_operation(READ_BACK);
	}

	//void splice(iterator position, AdaptiveSequence<T>& x);
	//void splice(iterator position, AdaptiveSequence<T>& x, iterator i);
	//void splice(iterator position, AdaptiveSequence<T>& x, iterator first,
	//		iterator last);
	void remove(const T& value) {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->remove(value);
			break;
		case VECTOR:
			internals->contents.vector->remove(value);
			break;
		case DEQUE:
			internals->contents.deque->remove(value);
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
		switch (internals->representation) {
		case LIST:
			internals->contents.list->sort();
			break;
		case VECTOR:
			std::sort(internals->contents.vector->begin(),
					internals->contents.vector->end());
			/* std::list<T> temp();
			 for(std::vector<T>::iterator i=internals->contents.vector->begin();i!=internals->contents.vector->end();i++)
			 temp.push_back(*i);
			 temp.sort();
			 for(int i=0;i<internals->contents.vector->size();i++)
			 internals->contents.vector->at(i) = temp[i];
			 log_operation(ITERATE);*/
			break;
		case DEQUE:
			/*std::list<T> temp();
			 for(std::vector<T>::iterator i=internals->contents.deque->begin();i!=internals->contents.deque->end();i++)
			 temp.push_back(*i);
			 temp.sort();
			 for(int i=0;i<internals->contents.deque->size();i++)
			 internals->contents.deque->at(i) = temp[i];
			 log_operation(ITERATE);*/
			std::sort(internals->contents.deque->begin(),
					internals->contents.deque->end());
			break;
		}
		log_operation(SORT);
	}
	template<class Compare> void sort(Compare comp);

	void reverse() {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->reverse();
			break;
		case VECTOR:
			internals->contents.vector->reverse();
			break;
		case DEQUE:
			internals->contents.deque->reverse();
			break;
		}
		log_operation(ITERATE_OVER);
	}
	void getInternals() {
		switch (internals->representation) {
		case LIST:
			std::cout << "LIST" << std::endl;
			break;
		case VECTOR:
			std::cout << "VECTOR" << std::endl;
			break;
		case DEQUE:
			std::cout << "DEQUE" << std::endl;
			break;
		}
	}
};

#endif /* ADAPTIVESEQUENCE_H_ */
