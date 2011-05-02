/*
 * AdaptiveSequence.h
 *
 *  Created on: Apr 11, 2011
 *      Authors: Xiang Zhao, Eli Gottlieb
 */

#ifndef ADAPTIVESEQUENCE_H_
#define ADAPTIVESEQUENCE_H_

#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <list>
#include <vector>
#include <deque>
#include <memory>
#include "adaptivity.h"

template<typename T, class Allocator = std::allocator<T> > class AdaptiveSequence {
protected:
	enum representation_t {
		LIST, VECTOR, deque
	};
	typedef union {
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
			case deque:
				contents.deque = new std::deque<T>();
				representation = deque;
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
			case deque:
				delete contents.deque;
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
				case deque:
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
				case deque:
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
				case deque:
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
					return LINEAR;
					break;
				case deque:
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
	std::list<iterator*> iteratorList;
public:
	//typedef size_t size_type;
	//typedef ptrdiff_t difference_type;
	//Iterators
	struct iterator {
		typename std::list<T, Allocator>::iterator list_bidirectional_iterator;
		typename std::vector<T, Allocator>::iterator
				vector_random_access_iterator;
		typename std::deque<T, Allocator>::iterator
				deque_random_access_iterator;
		int list_bidirectional_iterator_counter;
		int vector_random_access_iterator_counter;
		int deque_random_access_iterator_counter;
		AdaptiveSequence* currentSequence;
		int tag;
		iterator(AdaptiveSequence* sequence) {
			list_bidirectional_iterator_counter = 0;
			vector_random_access_iterator_counter = 0;
			deque_random_access_iterator_counter = 0;
			switch (sequence->internals->representation) {
			case LIST:
				tag = 1;
				break;
			case VECTOR:
				tag = 2;
				break;
			case deque:
				tag = 3;
				break;
			}
			sequence->iteratorList.push_back(this);
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

		iterator& operator++() {
			switch (tag) {
			case 1:
				list_bidirectional_iterator++;
				list_bidirectional_iterator_counter++;
				break;
			case 2:
				vector_random_access_iterator++;
				vector_random_access_iterator_counter++;
				break;
			case 3:
				deque_random_access_iterator++;
				deque_random_access_iterator_counter++;
				break;
			}
			return *this;
		}
		iterator& operator--() {
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
			return *this;

		}
		bool operator==(const iterator& __i) {
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
		bool operator!=(const iterator& __i) {
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

	};
	std::list<operation_t> operations;
	//<<<<<<< HEAD
	void syncIterator(iterator* it) {
		std::cout << "sync iterator" << std::endl;
		representation_t rep = it->currentSequence->internals->representation;
		int tag = it->tag;
	}
	//=======
	//	void syncIterator(iterator* it);
	//>>>>>>> refs/remotes/origin/master
	void syncIterators() {
		for (typename std::list<iterator*>::iterator it = iteratorList.begin(); it
				!= iteratorList.end(); ++it) {
			syncIterator(*it);
		}
	}
protected:

	void log_operation(operation_t op) {
		operations.push_front(op);
		attempt_adaptation();
	}
	unsigned int represent_costs(representation_t rep) {
		unsigned int result = 0, length = size();
		for (std::list<operation_t>::iterator i = operations.begin(); i
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
		syncIterators();
		unsigned int length = size();
		float list_cost = (float) (represent_costs(LIST) + length)
				/ operations.size();
		float vector_cost = (float) (represent_costs(VECTOR) + length)
				/ operations.size();
		float deque_cost = (float) (represent_costs(deque) + length)
				/ operations.size();
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
					ContentsADT* insides = new ContentsADT(deque);
					std::cout << "ADAPT deque" << std::endl;
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
				syncIterators();
				operations.clear();
			}
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
					ContentsADT* insides = new ContentsADT(deque);
					std::cout << "ADAPT deque" << std::endl;
					insides->contents.deque->resize(length);
					for (int i = 0; i < length; i++)
						insides->contents.deque->at(i)
								= internals->contents.vector->at(i);
					delete internals;
					internals = insides;
				}
				operations.clear();
			}
			//			syncIterators(internals->representation);
			break;
		case deque:
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
				operations.clear();
			}
			break;
		}
	}
	ContentsADT* internals;
	//<<<<<<< HEAD
	//=======
	//<<<<<<< HEAD
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
	//			case deque:
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

	//	void syncIterator(iterator* it) {
	//		representation_t rep = it->currentSequence->internals->representation;
	//		int tag = it->tag;
	//		//		switch ();
	//
	//	}
	//>>>>>>> refs/remotes/origin/master

public:
	typedef iterator iterator;

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

	AdaptiveSequence() {
		internals = new ContentsADT(VECTOR);
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
		case deque:
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
		case deque:
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
		switch (internals->representation) {
		case LIST:
			return internals->contents.list->empty();
			break;
		case VECTOR:
			return internals->contents.vector->empty();
			break;
		case deque:
			return internals->contents.deque->empty();
			break;
		}
		log_operation(READ_FRONT);
	}

	size_type size() {
		switch (internals->representation) {
		case LIST:
			return internals->contents.list->size();
			break;
		case VECTOR:
			return internals->contents.vector->size();
			break;
		case deque:
			return internals->contents.deque->size();
			break;
		}
		//Computing the size of the sequence requires iterating over it.		
		log_operation(ITERATE_OVER);
	}
	size_type max_size() {
		switch (internals->representation) {
		case LIST:
			return internals->contents.list->max_size();
			break;
		case VECTOR:
			return internals->contents.vector->max_size();
			break;
		case deque:
			return internals->contents.deque->max_size();
			break;
		}
		log_operation(ITERATE_OVER);
	}
	void resize(size_type sz, T c = T()) {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->resize(sz, c);
			break;
		case VECTOR:
			internals->contents.vector->resize(sz, c);
			break;
		case deque:
			internals->contents.deque->resize(sz, c);
			break;
		}
		log_operation(ITERATE_OVER);
	}

	reference front() {
		switch (internals->representation) {
		case LIST:
			return internals->contents.list->front();
			break;
		case VECTOR:
			return internals->contents.vector->front();
			break;
		case deque:
			return internals->contents.deque->front();
			break;
		}
		log_operation(READ_FRONT);
	}

	const_reference front() const {
		switch (internals->representation) {
		case LIST:
			return internals->contents.list->front();
			break;
		case VECTOR:
			return internals->contents.vector->front();
			break;
		case deque:
			return internals->contents.deque->front();
			break;
		}
		log_operation(READ_FRONT);
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
			break;
		}
		case deque:
			internals->contents.deque->push_front(x);
			break;
		}
		log_operation(WRITE_FRONT);
	}

	reference operator[](size_type n) {
		return at(n);
	}
	const_reference operator[](size_type n) const {
		return at(n);
	}

	const_reference at(size_type n) const {
		switch (internals->representation) {
		case LIST:
			typename std::list<T, Allocator>::iterator iter =
					internals->contents.list->begin();
			for (int i = 0; i < n; i++)
				iter++;
			T& result = *iter;
			return result;
			break;
		case VECTOR:
			return internals->contents.vector->at(n);
			break;
		case deque:
			return internals->contents.deque->at(n);
			break;
		}
		log_operation(ACCESS_ELEMENT);
	}

	reference at(size_type n) {
		switch (internals->representation) {
		case LIST:
			typename std::list<T, Allocator>::iterator iter =
					internals->contents.list->begin();
			for (int i = 0; i < n; i++)
				iter++;
			T& result = *iter;
			return result;
			break;
		case VECTOR:
			return internals->contents.vector->at(n);
			break;
		case deque:
			return internals->contents.deque->at(n);
			break;
		}
		log_operation(ACCESS_ELEMENT);
	}

	template<class InputIterator> void assign(InputIterator first,
			InputIterator last);
	void assign(size_type n, const T& u) {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->assign(n, u);
			break;
		case VECTOR:
			internals->contents.vector->assign(n, u);
			break;
		case deque:
			internals->contents.deque->assign(n, u);
			break;
		}
		log_operation(ITERATE_OVER);
	}
	//<<<<<<< HEAD
	//
	//	void push_front(const T& x) {
	//		switch (internals->representation) {
	//		case LIST:
	//			internals->contents.list->push_front(x);
	//			break;
	//		case VECTOR:
	//			internals->contents.vector->push_front(x);
	//			break;
	//		case deque:
	//			internals->contents.deque->push_front(x);
	//			break;
	//		}
	//		log_operation(ACCESS_FRONT);
	//	}
	//
	//=======
	//
	//>>>>>>> refs/remotes/origin/master
	void pop_front() {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->pop_front();
			break;
		case VECTOR:
			internals->contents.vector->pop_front();
			break;
		case deque:
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
			break;
		case deque:
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
			break;
		case deque:
			internals->contents.deque->pop_back();
			break;
		}
		log_operation(READ_BACK);
	}
	//iterator insert(iterator position, const T& x);
	//void insert(iterator position, size_type n, const T& x);
	//template<class InputIterator> void insert(iterator position,
	//		InputIterator first, InputIterator last);
	//iterator erase(iterator position);
	//iterator erase(iterator first, iterator last);
	void swap(AdaptiveSequence<T>& seq);
	void clear() {
		switch (internals->representation) {
		case LIST:
			internals->contents.list->clear();
			break;
		case VECTOR:
			internals->contents.vector->clear();
			break;
		case deque:
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
		case deque:
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
			internals->contents.vector->sort();
			break;
		case deque:
			internals->contents.deque->sort();
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
		case deque:
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
		case deque:
			std::cout << "deque" << std::endl;
			break;
		}
	}
};

#endif /* ADAPTIVESEQUENCE_H_ */
