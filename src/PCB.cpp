/*
 * PCB.cpp
 *
 *  Created on: May 15, 2019
 *      Author: OS1
 */

#include "PCB.h"
#include "const.h"
#include <dos.h>
#include "stdio.h"
ID PCB::idgen = 0;
PCB* PCB::pcbarr[500] = { 0 }; //init na null
PCB* PCB::running = 0;
PCB* PCB::idle = 0;
PCB * PCB::mainpcb = 0;
int PCB::kernelprivileges = 0;
#include "IOSTREAM.H"

PCB::PCB(StackSize stacksz, Time ts) { //IDLE
	//IDLE THREAD
	timeslice = 1;
	stacksz /= 2;
	stack = new unsigned[stacksz];

	stack[stacksz - PSWOff] = 0x200;	//set i bit na 1
#ifndef BCC_BLOCK_IGNORE
			stack[stacksz - SegOff] = FP_SEG(PCB::idlewrapper);
			stack[stacksz - OffOff] = FP_OFF(PCB::idlewrapper);
			ss = FP_SEG(&stack[stacksz - STOff]);
			sp = FP_OFF(&stack[stacksz - STOff]);
			bp = FP_OFF(&stack[stacksz - STOff]);
#endif
	stack[stacksz - STOff] = bp;
	mythread = 0;	//nema potrebe
	id = 501;	//bas njegov
	wasunblocked = 0;
	isworking = 1;	//da ne moze da se startuje
	mythread->myowner = 0;
//	pcbarr[id] = this;
}

PCB::PCB(StackSize stacksz, Time ts, Thread * mythr) {
	timeslice = ts;
	stacksz /= 2;
	stack = new unsigned[stacksz];

	int myThreadSeg, myThreadOff;
#ifndef BCC_BLOCK_IGNORE
	myThreadSeg=FP_SEG(mythr);
	myThreadOff=FP_OFF(mythr);
#endif
	stack[stacksz - PtrSegOff] = myThreadSeg;
	stack[stacksz - PtrOffOff] = myThreadOff;
	stack[stacksz - PSWOff] = 0x200;	//set i bit na 1
#ifndef BCC_BLOCK_IGNORE
			stack[stacksz - SegOff] = FP_SEG(PCB::wrapper);
			stack[stacksz - OffOff] = FP_OFF(PCB::wrapper);
			ss = FP_SEG(&stack[stacksz - STOff]);
			sp = FP_OFF(&stack[stacksz - STOff]);
			bp = FP_OFF(&stack[stacksz - STOff]);
#endif
	stack[stacksz - STOff] = bp;
	mythread = mythr;
	id = ++idgen;
	isworking = 0;
	wasunblocked = 0;
	pcbarr[id] = this;
}

PCB::PCB() {	//main pcb
	isworking = 0;
	wasunblocked = 0;
	stack = NULL;
	id = 502;
	mythread = 0;
}

extern int dispatchret;
extern int dispatchretseg;

void PCB::wrapper(Thread * my) {

//	printf("%p   %p\n",my,PCB::running->mythread);
	running->mythread->run();
	while (!running->waiting.isEmpty()) {
		PCB * next = running->waiting.pop();
//		cout<<"ubacen u sched: "<<next->id<<endl;
		if (next->isworking >= 0) {
			next->isworking = 1;
			Scheduler::put(next);
		}
	}

	lock();
	PCB::kernelprivileges = 1;
	if (PCB::running->mythread != 0) {
		if (PCB::running->mythread->myowner != 0)
			PCB::running->mythread->myowner->signal(1);
		PCB::running->mythread->signal(2);
	}
	PCB::kernelprivileges = 1;
	unlock();

	PCB::running->mythread->handlesignals();
	PCB::running->isworking = -1;

	dispatch();

//	#ifndef BCC_BLOCK_IGNORE
//		asm{
//			push ax
//			push bx
//			mov bx,bp
//			mov ax,dispatchret
//			add bx,2
//			mov [bx],ax
//			mov ax,dispatchretseg
//			add bx,2
//			mov [bx],ax
//			pop bx
//			pop ax
//		}
//	#endif
//
//	lock();
//	printf("");
//	cout <<""<< flush;
//	unlock();
}
void PCB::idlewrapper() {
	while (1) {
//		printf("idle");
	}
}

PCB::~PCB() {
	pcbarr[id] = 0;
	delete stack;
}

void PCB::startPCB() {
	if (isworking !=0)//moze zablokirani start
		return;	//nece moci dva puta start
	this->isworking = 1;
	Scheduler::put(this);
}

void PCB::waitToComplete() {
//	cout<<("waittocompl ")<<isworking<<" "<<"pushovan u: "<<id<<endl;
	if (isworking ==1 || isworking==0) { //blokirana ili radi
		running->isworking = 0;
		waiting.push(running);
		dispatch();
	}
}

void PCB::handlesignals() {
	mythread->handlesignals();
}
