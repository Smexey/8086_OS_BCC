/*
 * Thread.cpp
 *
 *  Created on: May 15, 2019
 *      Author: OS1
 */

#include "Thread.h"
#include "PCB.h"
#include "lock.h"
extern int requestedDispatch;
extern int lockFlag;

Thread::Thread(StackSize stackSize, Time timeSlice) {
	myPCB = new PCB(stackSize, timeSlice, this);
	myPCB->startPCB();
}

void Thread::start() {
	myPCB->startPCB();
}

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId() {
	return PCB::running->id;
}

Thread * getThreadById(ID id) {
	return PCB::pcbarr[id]->mythread;
}

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}

Thread::~Thread() {
//	waitToComplete();
	delete myPCB;
}

void dispatch() {
	//lock i unlock zbog promene kont izmddju ova dva?
	requestedDispatch = 1;
	asm int 8h; // call new Timer
}
