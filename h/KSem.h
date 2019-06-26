/*
 * KSem.h
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#ifndef H_KSEM_H_
#define H_KSEM_H_

#include "Queue.h"
#include "PCB.h"
#include "const.h"
#include "SQueue.h"
#include "SQList.h"

class KernelSem {
public:
	int val;
	Queue blocked;
	SleepQueue sleepblocked;

	KernelSem(int _value);
	virtual ~KernelSem();

	int wait(Time t);
	int signal(int n);
	int getVal() const;

	static SQList semsleeplist;
};

#endif /* H_KSEM_H_ */
