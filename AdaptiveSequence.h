/*
 * AdaptiveSequence.h
 *
 *  Created on: Apr 11, 2011
 *      Authors: Xiang Zhao, Eli Gottlieb
 */

#ifndef ADAPTIVESEQUENCE_H_
#define ADAPTIVESEQUENCE_H_

#include "stdlib.h"
#include <list>
#include <vector>
#include <deque>
#include "adaptivity.h"

template<typename T, typename Allocator = std::allocator<T> > class AdaptiveSequence {
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
		representation_t tag;
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
	}
	std::list<operation_t, Allocator> operations;
	void log_operation(operation_t op) {
		operations.push_front(op);
		attempt_adaptation();
	}
	unsigned int represent_costs(representation_t rep);
	void attempt_adaptation();
	ContentsADT internals;
public:
	typedef Allocator::reference reference;
	typedef Allocator::const_reference const_reference;
	//typedef iterator
	//Bidirectional iterator
	//const_iterator	Constant bidirectional iterator
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T value_type;
	typedef Allocator allocator_type;
	typedef Allocator::pointer pointer;
	typedef Allocator::const_pointer const_pointer;
	typedef reverse_iterator<iterator> iterator;
	typedef reverse_iterator<const_iterator> const_reverse_iterator;

	AdaptiveSequence();
	virtual ~AdaptiveSequence();

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	iterator rbegin();
	const_iterator rbegin() const;
	iterator rend();
	const_iterator rend() const;

	bool empty() const;
	size_type size() const;
	size_type max_size() const;
	void resize(size_type sz, T c = T());

	reference front();
	const_reference front() const;

	reference operator[](size_type n) {
		return at(n);
	}
	const_reference operator[](size_type n) const {
		return at(n);
	}
	const_reference at(size_type n) const;
	reference at(size_type n);

	template<class InputIterator> void assign(InputIterator first,
			InputIterator last);
	void assign(size_type n, const T& u);
	void push_front(const T& x);
	void pop_front();
	void push_back(const T& x);
	void pop_back();
	iterator insert(iterator position, const T& x);
	void insert(iterator position, size_type n, const T& x);
	template<class InputIterator> void insert(iterator position,
			InputIterator first, InputIterator last);
	iterator erase(iterator position);
	iterator erase(iterator first, iterator last);
	void swap(AdaptiveSequence<T>& seq);
	void clear();

	void splice(iterator position, AdaptiveSequence<T>& x);
	void splice(iterator position, AdaptiveSequence<T>& x, iterator i);
	void splice(iterator position, AdaptiveSequence<T>& x, iterator first,
			iterator last);
	void remove(const T& value);
	template<class Predicate> void remove_if(Predicate pred);
	void unique();
	template<class BinaryPredicate> void unique(BinaryPredicate binary_pred);

	void merge(AdaptiveSequence<T>& x);
	template<class Compare> void merge(AdaptiveSequence<T>& x, Compare comp);

	void sort();
	template<class Compare> void sort(Compare comp);

	void reverse();
};

#endif /* ADAPTIVESEQUENCE_H_ */
