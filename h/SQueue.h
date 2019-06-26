/*
 * SQueue.h
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#ifndef SRC_SQUEUE_H_
#define SRC_SQUEUE_H_

#include "PCB.h"

class SleepQueue {
private:
	struct SleepNode{
		PCB* elem;
		Time wait;
		SleepNode* next;
		SleepNode(PCB* _elem, Time _wait):elem(_elem),next(0), wait(_wait){}
	};

	SleepNode* first;
public:
	SleepQueue():first(0){}
	void push(PCB* elem, Time wait);
	int decschedule();
	int isEmpty(){
		return ((first==0) ? 1:0);
	}

	PCB* pop();

	~SleepQueue();
};



#endif /* SRC_SQUEUE_H_ */
