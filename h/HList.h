/*
 * HandlerList.h
 *
 *  Created on: May 31, 2019
 *      Author: OS1
 */

#ifndef H_HLIST_H_
#define H_HLIST_H_

typedef void (*SignalHandler)();

class HandlerList {
private:
	struct Node{
		SignalHandler elem;
		Node* next;
		Node(SignalHandler _elem):elem(_elem),next(0){}
	};

	Node* first, *last;
public:
	HandlerList():last(0),first(0){}
	void push(SignalHandler elem);
//	SignalHandler pop();
	int isEmpty(){
		return ((first==0) ? 1:0);
	}

	void copyfrom(HandlerList * from);

	void handle();
	void swap(SignalHandler hand1,SignalHandler hand2);
	void removeall();

	virtual ~HandlerList();
};



#endif /* H_HLIST_H_ */
