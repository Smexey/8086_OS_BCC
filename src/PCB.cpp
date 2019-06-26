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
ID PCB::idgen =  0;
PCB* PCB::pcbarr[500] = {0};//init na null
PCB* PCB::running = 0;
PCB* PCB::idle = 0;


PCB::PCB(StackSize stacksz,Time ts){//IDLE
	//IDLE THREAD
	timeslice = 1;
	stacksz/=2;
	stack = new unsigned[stacksz];

//	stacksz+=2;//jer nema threadptr da se vrati negde?
	stack[stacksz - PSWOff] = 0x200;//set i bit na 1
	#ifndef BCC_BLOCK_IGNORE
	stack[stacksz - SegOff] = FP_SEG(PCB::idlewrapper);
	stack[stacksz - OffOff] = FP_OFF(PCB::idlewrapper);
	ss = FP_SEG(&stack[stacksz - STOff]);
	sp = FP_OFF(&stack[stacksz - STOff]);
	bp = FP_OFF(&stack[stacksz - STOff]);
	#endif
	stack[stacksz-STOff]=bp;
	mythread = 0;//nema potrebe
	id = 501;//bas njegov
	wasunblocked = 0;
	isworking = 1;//da ne moze da se startuje
//	pcbarr[id] = this;
}

PCB::PCB(StackSize stacksz,Time ts,Thread * mythr){
	timeslice = ts;
	stacksz/=2;
	stack = new unsigned[stacksz];

	int myTreadSeg,myTreadOff;
	#ifndef BCC_BLOCK_IGNORE
	myTreadSeg=FP_SEG(mythr);
	myTreadOff=FP_OFF(mythr);
	#endif
	stack[stacksz - PtrSegOff]=myTreadSeg;
	stack[stacksz - PtrOffOff]=myTreadOff;
	stack[stacksz - PSWOff] = 0x200;//set i bit na 1
	#ifndef BCC_BLOCK_IGNORE
	stack[stacksz - SegOff] = FP_SEG(PCB::wrapper);
	stack[stacksz - OffOff] = FP_OFF(PCB::wrapper);
	ss = FP_SEG(&stack[stacksz - STOff]);
	sp = FP_OFF(&stack[stacksz - STOff]);
	bp = FP_OFF(&stack[stacksz - STOff]);
	#endif
	stack[stacksz-STOff]=bp;
	mythread = mythr;
	id = ++idgen;
	isworking = 0;
	wasunblocked = 0;
	pcbarr[id] = this;
}

PCB :: PCB () {
	isworking=0;
	wasunblocked = 0;
	stack = NULL;
	id=502;//main pcb
}

void PCB::wrapper(){
	running->mythread->run();
	while(!running->waiting.isEmpty()){
		PCB * next = running->waiting.pop();
		next->isworking = 1;
		Scheduler::put(next);
	}
	running->isworking = 0;
	dispatch();
	//running=0; dispatch!!!! nikad se ne zavrsi wrapper jer pcb nema callback
}
void PCB::idlewrapper(){
	while(1){
//		printf("idle\n");
	}
}


PCB::~PCB (){
	pcbarr[id] = 0;
	delete stack;
}

void PCB::startPCB(){
	if(isworking!=0) return;//nece moci dva puta start
	this->isworking=1;
	Scheduler::put(this);
}

void PCB::waitToComplete(){
	if (isworking == 1){
		running->isworking = 0;
		waiting.push(running);
		dispatch();
	}
}
