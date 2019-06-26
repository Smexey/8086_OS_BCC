/*
 * List.cpp
 *
 *  Created on: May 31, 2019
 *      Author: OS1
 */



#include "List.h"
#include "STDIO.H"

void List::push(unsigned int elem){
//	printf("PUSHEDTOLIST");
	Node* newN=new Node(elem);
	if(first==0)
		first=last=newN;
	else{
		last=last->next=newN;
	}
}

unsigned int List::pop(){
	Node* newN=first;
	if(first==0)
		return 0;
	first=newN->next;
	if(first==0)
		last=0;
	int elem=newN->elem;
	delete newN;
	return elem;
}

List::~List() {
	while (first!=0){
		last=first;
		first=first->next;
		delete last;
	}
}
