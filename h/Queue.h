/*
 * Queue.h
 *
 *  Created on: May 15, 2019
 *      Author: OS1
 */

#ifndef OS_H_QUEUE_H_
#define OS_H_QUEUE_H_

class PCB;

class Queue {
private:
	struct Node{
		PCB* elem;
		Node* next;
		Node(PCB* _elem):elem(_elem),next(0){}
	};

	Node* first, *last;
public:
	Queue():last(0),first(0){}
	void push(PCB* elem);
	PCB* pop();
	int isEmpty(){
		return ((first==0) ? 1:0);
	}
	virtual ~Queue();
};


#endif /* OS_H_QUEUE_H_ */
