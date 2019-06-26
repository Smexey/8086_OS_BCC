/*
 * ThrKill.h
 *
 *  Created on: May 31, 2019
 *      Author: OS1
 */

#ifndef SRC_THRKILL_H_
#define SRC_THRKILL_H_

#include "PCB.h"

class ThreadKiller: public Thread {

	friend class PCB;
public:
	ThreadKiller();

	virtual void run();
	void insert(PCB* elem);

	~ThreadKiller();

	Queue tokill;
	static ThreadKiller threadkiller;
};



#endif /* SRC_THRKILL_H_ */
