/*
 * KEvent.h
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#ifndef H_KEVENT_H_
#define H_KEVENT_H_


typedef unsigned char IVTNo;

class PCB;
class Event;

class KernelEv{

public:
	KernelEv(IVTNo);
	virtual ~KernelEv();
	void signal();
	void wait();

	IVTNo intNumber;
	PCB * caller;
	int sem;

};


#endif /* H_KEVENT_H_ */
