/*
 * Event.h
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#ifndef H_EVENT_H_
#define H_EVENT_H_

// File: event.h
#include "const.h"

typedef unsigned char IVTNo;
class KernelEv;

class Event {
public:
	Event(IVTNo ivtNo);
	~Event();
	void wait();
protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
private:
	KernelEv* myImpl;
};



#endif /* H_EVENT_H_ */
