/*
 * List.h
 *
 *  Created on: May 31, 2019
 *      Author: OS1
 */

#ifndef H_LIST_H_
#define H_LIST_H_


class List {
private:
	struct Node{
		unsigned int elem;
		Node* next;
		Node(unsigned int _elem):elem(_elem),next(0){}
	};

	Node* first, *last;
public:
	List():last(0),first(0){}
	void push(unsigned int elem);
	unsigned int pop();
	int isEmpty(){
		return ((first==0) ? 1:0);
	}
	virtual ~List();
};


#endif /* H_LIST_H_ */
