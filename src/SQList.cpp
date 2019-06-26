/*
 * SQList.cpp
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#include "SQList.h"

void SQList::push(SleepQueue* elem) {
	Node* newN = new Node(elem);
	if (first == 0)
		first = last = newN;
	else {
		last = last->next = newN;
	}
}

void SQList::remove(SleepQueue* e) {
//	printf("REMOVE SQLIST \n");
	Node * tek = first;
	Node * prev = 0;
	while (tek != 0) {
		if (tek->elem == e) {
			if (prev != 0) {
				prev->next = tek->next;
				tek->next = 0;
				delete tek;
				return;
			} else {
				first = tek->next;
				tek->next = 0;
				if (first == 0)
					last = 0;
			}
		}
		prev = tek;
		tek = tek->next;
	}
}

SQList::~SQList() {
	while (first != 0) {
		last = first;
		first = first->next;
		delete last;
	}
}

int SQList::decschedule() {

	for (Node * tek = first; tek != 0; tek = tek->next) {
		if (!tek->elem->isEmpty())
			tek->elem->decschedule();
	}
	return 0;
}
