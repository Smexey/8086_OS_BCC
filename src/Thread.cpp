/*
 * Thread.cpp
 *
 *  Created on: May 15, 2019
 *      Author: OS1
 */

#include "Thread.h"
#include "PCB.h"
#include "lock.h"
#include <IOSTREAM.H>
#include "dos.h"
#include "ASSERT.H"

#include "const.h"
#include "ThrKill.h"

extern int requestedDispatch;

Thread::Thread(StackSize stackSize, Time timeSlice) {
	myowner = PCB::running->mythread;
	for (short i = 0; i < NUMSIGNALS; ++i) {
		if (myowner != 0){
			signalblockflag[i] = myowner->signalblockflag[i]; //nasledjuje blokiranje
			handlerlists[i].copyfrom(&(myowner->handlerlists[i]));
		}
		else{
			signalblockflag[i] = 0;
			//nema kopije handlera
		}

	}

	myPCB = new PCB(stackSize, timeSlice, this);
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

Thread * Thread::getThreadById(ID id) {
	return PCB::pcbarr[id]->mythread;
}

void Thread::waitToComplete() {

	myPCB->waitToComplete();
}

Thread::~Thread() {
//	waitToComplete();
	delete myPCB;
}

int dispatchret = 0;
int dispatchretseg = 0;
void setdispatchret() {
#ifndef BCC_BLOCK_IGNORE
	asm {
		push ax
		mov ax, [bp]+2
		mov dispatchret,ax
		mov ax, [bp]+4
		mov dispatchretseg,ax
		pop ax
	}
//	lock();
//	printf("setdispret seg: %X\n",dispatchret);
//	printf("setdispret seg: %X\n",dispatchretseg);
//	printf("real disp adr: %X\n", FP_OFF(dispatch));
//	printf("real disp seg: %X\n", FP_SEG(dispatch));
//	unlock();
#endif
}

void dispatch() {
//		if (dispatchret == 0) { //prvi dispatch cuva
//			setdispatchret();
//		}

	requestedDispatch = 1;
#ifndef BCC_BLOCK_IGNORE
	asm int 8h; // call new Timer
#endif
}

//=======SIGNALI========

short Thread::globalsignalblockflag[NUMSIGNALS] = { 0 };

void Thread::blockSignalGlobally(SignalId signal) {
	lock();
	assert(signal<NUMSIGNALS);
	globalsignalblockflag[signal]++;
	unlock();
}

void Thread::unblockSignalGlobally(SignalId signal) {
	lock();
	assert(signal<NUMSIGNALS);
	globalsignalblockflag[signal]--;
	if (globalsignalblockflag[signal] < 0)
		globalsignalblockflag[signal] = 0;
	unlock();
}

void Thread::blockSignal(SignalId signal) {
	lock();
	assert(signal<NUMSIGNALS);
	signalblockflag[signal]++;
	unlock();
}

void Thread::unblockSignal(SignalId signal) {
	lock();
	assert(signal<NUMSIGNALS);
	signalblockflag[signal]--;
	if (signalblockflag[signal] < 0)
		signalblockflag[signal] = 0;
	unlock();
}

void Thread::signal(SignalId signal) {
	lock();
	//ne dozvoljava user signal 1
	if ((signal == 1 || signal == 2) && !PCB::kernelprivileges) {
//		printf("illegal signal");
		unlock();
		return;
	}

	//ako nije prazna registruje se signal
	signallist.push(signal);
//	printf("SIGNAL PUSH idthr: %d sig: %d \n", myPCB->id, signal);
	unlock();
	//dispatch(); nema smisla
}

void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	lock();
	assert(signal<NUMSIGNALS);
	handlerlists[signal].push(handler);
//	printf("handler registered for %d\n", signal);
	unlock();
}

void Thread::unregisterAllHandlers(SignalId id) {
	lock();
	assert(id<NUMSIGNALS);
	handlerlists[id].removeall();
	unlock();
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	lock();
	handlerlists[id].swap(hand1, hand2);
	unlock();
}

void Thread::handlesignals() {
	if(signallist.isEmpty()) return;
	lock();
	unlockintinsideint();
	int shouldbekilled = 0;
	while (!signallist.isEmpty()) {
		unsigned int id = signallist.pop();
//		printf("signallist search id %d ", id);
		if (!globalsignalblockflag[id] && !signalblockflag[id]) {
//			printf("prolaziblock %d\n",id);
//			printf("idthrcurr: %d\n",Thread::getRunningId());
			handlerlists[id].handle();
			if (id == 0) {
				//zove se samoubilacka
				//ubaci se u threadkiller nit da se posle ubije jednog dana
//				printf("samoubistvo idthr %d ", PCB::running->id);
				shouldbekilled = 1;
			}
		}
	}
	if (shouldbekilled) {
		//pokrece gc ako je trebala da se ubije
		PCB::running->isworking = -1;
		ThreadKiller::threadkiller.insert(PCB::running);
		ThreadKiller::threadkiller.start();//samo ubaci u pcb
//		printf("SHOULDBEKILLEDINVOKED");
		lockintinsideint();
		unlock();
		dispatch();
	}
	lockintinsideint();
	unlock();
}

