/*
 * AdaptiveSequence.cpp
 *
 *  Created on: Apr 11, 2011
 *      Author: Xiang Zhao
 */

#include "AdaptiveSequence.h"

AdaptiveSequence::AdaptiveSequence() {
	internals = ContentsADT(LIST);
}

AdaptiveSequence::~AdaptiveSequence() {
	~internals;
}

bool AdaptiveSequence::empty() const {
	switch(internals.representation) {
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

size_type AdaptiveSequence::size() const {
	switch(internals.representation) {
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

size_type AdaptiveSequence::max_size() const {
	switch(internals.representation) {
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

void AdaptiveSequence::resize (size_type sz, T c = T()) {
	switch(internals.representation) {
		case LIST:
			internals.contents.list->resize(sz,c);
			break;
		case VECTOR:
			internals.contents.vector->resize(sz,c);
			break;
		case DEQUEUE:
			internals.contents.dequeue->resize(sz,c);
			break;
	}
	log_operation(ITERATE_OVER);
}

reference AdaptiveSequence::front() {
	switch(internals.representation) {
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

const_reference AdaptiveSequence::front() const {
	switch(internals.representation) {
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

const_reference AdaptiveSequence::at(size_type n) const {
	switch(internals.representation) {
		case LIST:
			return internals.contents.list->at(n);
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

reference AdaptiveSequence::at(size_type n) {
	switch(internals.representation) {
		case LIST:
			return internals.contents.list->at(n);
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

void AdaptiveSequence::assign (size_type n,const T& u) {
	switch(internals.representation) {
		case LIST:
			internals.contents.list->assign(n,u);
			break;
		case VECTOR:
			internals.contents.vector->assign(n,u);
			break;
		case DEQUEUE:
			internals.contents.dequeue->assign(n,u);
			break;
	}
	log_operation(ITERATE_OVER);
}

void AdaptiveSequence::push_front (const T& x) {
	switch(internals.representation) {
		case LIST:
			internals.contents.list->push_front(x);
			break;
		case VECTOR:
			internals.contents.vector->push_front(x);
			break;
		case DEQUEUE:
			internals.contents.dequeue->push_front(x);
			break;
	}
	log_operation(ACCESS_FRONT);
}

void AdaptiveSequence::pop_front() {
	switch(internals.representation) {
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

void AdaptiveSequence::push_back (const T& x) {
	switch(internals.representation) {
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

void AdaptiveSequence::pop_back() {
	switch(internals.representation) {
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

void AdaptiveSequence::clear() {
	switch(internals.representation) {
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
	log_operation(LENGTH);
}

void AdaptiveSequence::remove(const T& value) {
	switch(internals.representation) {
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

void AdaptiveSequence::sort() {
	switch(internals.representation) {
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

void AdaptiveSequence::reverse() {
	switch(internals.representation) {
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

unsigned int AdaptiveSequence::represent_costs(representation_t rep) {
	unsigned int result = 0,length = size();
	for(std::list<operation_t>::iterator i = operations.begin(); i != operations.end(); ++i) {
		switch(internals.complexity(*i,rep)) {
			case CONSTANT:
				result++;
				break;
			case LINEAR:
				result += length;
				break;
			case NLOGN:
				result += length*log(length);
				break;
		}
	}
	return result;
}

void AdaptiveSequence::attempt_adaptation() {
	unsigned int length = size();
	float list_cost = (represent_costs(LIST) + length) / operations.size();
	float vector_cost = (represent_costs(VECTOR) + length) / operations.size();
	float dequeue_cost = (represent_costs(DEQUEUE) + length) / operations.size();
	switch(internals.representation) {
		case LIST:
			if(vector_cost < list_cost || dequeue_cost < list_cost) {
				if(vector_cost < dequeue_cost) {
					ContentsADT insides(VECTOR);
					insides.contents.vector->resize(length);
					for(int i=0;i<length;i++)
						insides.contents.vector->at(i) = internals.contents.list->at(i);
					~internals;
					internals = insides;
				}
				else {
					ContentsADT insides(DEQUEUE);
					insides.contents.dequeue->resize(length);
					for(int i=0;i<length;i++)
						insides.contents.dequeue->at(i) = internals.contents.list->at(i);
					~internals;
					internals = insides;
				}
				operations.clear();
			}
			break;
		case VECTOR:
			if(list_cost < vector_cost || dequeue_cost < vector_cost) {
				if(list_cost < dequeue_cost) {
					ContentsADT insides(LIST);
					insides.contents.list->resize(length);
					for(int i=0;i<length;i++)
						insides.contents.list->at(i) = internals.contents.vector->at(i);
					~internals;
					internals = insides;
				}
				else {
					ContentsADT insides(DEQUEUE);
					insides.contents.dequeue->resize(length);
					for(int i=0;i<length;i++)
						insides.contents.dequeue->at(i) = internals.contents.vector->at(i);
					~internals;
					internals = insides;
				}
				operations.clear();
			}
			break;
		case DEQUEUE:
			if(list_cost < dequeue_cost || vector_cost < dequeue_cost) {
				if(list_cost < vector_cost) {
					ContentsADT insides(LIST);
					insides.contents.list->resize(length);
					for(int i=0;i<length;i++)
						insides.contents.list->at(i) = internals.contents.dequeue->at(i);
					~internals;
					internals = insides;
				}	
				else {
					ContentsADT insides(VECTOR);
					insides.contents.vector->resize(length);
					for(int i=0;i<length;i++)
						insides.contents.vector->at(i) = internals.contents.dequeue->at(i);
					~internals;
					internals = insides;
				}
				operations.clear();
			}
			break;
	}
}
