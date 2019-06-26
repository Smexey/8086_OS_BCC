/*
 * SQList.h
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#ifndef H_SQLIST_H_
#define H_SQLIST_H_

#include "SQueue.h"

class SQList {
private:
	struct Node{
		SleepQueue* elem;
		Node* next;
		Node(SleepQueue* _elem):elem(_elem),next(0){}
	};

	Node* first, *last;
public:
	SQList():last(0),first(0){}
	void push(SleepQueue* elem);

	int decschedule();

	void remove(SleepQueue* elem);

	int isEmpty(){
		return ((first==0) ? 1:0);
	}
	virtual ~SQList();
};


#endif /* H_SQLIST_H_ */
