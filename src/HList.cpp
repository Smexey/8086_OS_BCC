/*
 * HandlerList.cpp
 *
 *  Created on: May 31, 2019
 *      Author: OS1
 */

#include "../h/HList.h"

#include "STDIO.H"

void HandlerList::push(SignalHandler elem) {
	Node* newN = new Node(elem);
//	printf("pushdedintohandlerlist! \n");
	if (first == 0)
		first = last = newN;
	else {
		last = last->next = newN;
	}
//	printf("this == %p\n",this);
//	printf("first == %p\n",first);
}


void HandlerList::removeall() {
	while (first != 0) {
		last = first;
		first = first->next;
		delete last;
	}
	first = last = 0;
}

void HandlerList::swap(SignalHandler hand1,SignalHandler hand2){
	Node * curr1 = first;
	Node * curr2 = first;
	while(curr1!=0 || curr2!=0){
		if(curr1->elem!=hand1) curr1 = curr1->next;
		if(curr2->elem!=hand2) curr2 = curr2->next;
	}
	if(curr1!=0 && curr2!=0){
		//nasli oba swap
		SignalHandler temp  = curr1->elem;
		curr1->elem = curr2->elem;
		curr2->elem = temp;
	}
	//svakako return nema efekta ako nema jedne od njih
	return;
}

void HandlerList::copyfrom(HandlerList * from){
	Node * curr = from->first;
	while(curr!=0){
		push(curr->elem);
		curr = curr->next;
	}
}

void HandlerList::handle(){
	Node *curr = first;
//	printf("this == %p\n",this);
//	printf("handlovanje! \n");
//	printf("first == %p\n",first);
	while(curr!=0){
//		printf("unutarloop! \n");
		SignalHandler foo =(curr->elem);
		foo();
		curr = curr->next;
	}
//	printf("endofhandle ");

}

HandlerList::~HandlerList() {
	removeall();
}
