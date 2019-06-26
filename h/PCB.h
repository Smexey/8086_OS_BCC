/*
 * PCB.h
 *
 *  Created on: May 15, 2019
 *      Author: OS1
 */

#ifndef OS_H_PCB_H_
#define OS_H_PCB_H_

#include "Thread.h"
#include "SCHEDULE.h"
#include "Queue.h"
#include "stdio.h"
#include "lock.h"

class PCB{
	//sve private jer je friend class od thread i idle
public:
	unsigned * stack;
	unsigned bp;
	unsigned sp;
	unsigned ss;

	ID id;
	Thread * mythread;
	int isworking;
	//int timedef;
	int timeslice;
	Queue waiting;
	int wasunblocked;//za onaj wait return boze sacuvaj

	//functions
	PCB(StackSize stacksz,Time ts,Thread * mythr);
	PCB(StackSize stacksz,Time ts);//za idle thread
	PCB();
	void startPCB();
	void waitToComplete();

	//statics

	static PCB* pcbarr[500];
	static ID idgen;
	virtual ~PCB();

	static PCB* running;
	static PCB* idle;

	static void wrapper();
	static void idlewrapper();
};



#endif /* OS_H_PCB_H_ */
