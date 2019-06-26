/*
 * Thread.h
 *
 *  Created on: May 15, 2019
 *      Author: OS1
 */

#ifndef OS_H_THREAD_H_
#define OS_H_THREAD_H_

#include "HList.h"
#include "List.h"

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

class PCB;

#define NUMSIGNALS 16

typedef void (*SignalHandler)();
typedef unsigned SignalId;

// Kernel's implementation of a user's thread
class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

	//signalci
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);

	void signal(SignalId signal);

	void blockSignal(SignalId signal);
	void unblockSignal(SignalId signal);

	static void blockSignalGlobally(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

protected:
	friend class PCB;
	Thread(StackSize stackSize = defaultStackSize, Time timeSlice =
			defaultTimeSlice);
	virtual void run() {
	}


private:
	PCB* myPCB;
	Thread* myowner;

	void handlesignals();

	static short globalsignalblockflag[NUMSIGNALS];
	short signalblockflag[NUMSIGNALS];
	List signallist;
	HandlerList handlerlists[NUMSIGNALS];
};
void dispatch();




#endif /* OS_H_THREAD_H_ */
