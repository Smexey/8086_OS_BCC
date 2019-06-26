/*
 * Queue.cpp
 *
 *  Created on: May 15, 2019
 *      Author: OS1
 */

#include "Queue.h"
#include "PCB.h"

void Queue::push(PCB* elem){
	Node* newN=new Node(elem);
	if(first==0)
		first=last=newN;
	else{
		last=last->next=newN;
	}
}

PCB* Queue::pop(){
	Node* newN=first;
	if(first==0)
		return 0;
	first=newN->next;
	if(first==0)
		last=0;
	PCB* elem=newN->elem;
	delete newN;
	return elem;
}

Queue::~Queue() {
	while (first!=0){
		last=first;
		first=first->next;
		delete last;
	}
}
