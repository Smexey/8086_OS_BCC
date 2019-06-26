/*
 * KSem.cpp
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#include "KSem.h"
#include <iostream.h>

SQList KernelSem::semsleeplist;

KernelSem::KernelSem(int _value) {
	val = _value;
	if (val < 0)
		val = 0;

	semsleeplist.push(&sleepblocked);
}

KernelSem::~KernelSem() {
	PCB* ptr;
	//na brisanje ubaci sve blokirane
	while ((ptr = blocked.pop()) != 0)
		Scheduler::put(ptr);

	//i sve sleep blokovane
	while ((ptr = sleepblocked.pop()) != 0)
		Scheduler::put(ptr);

	KernelSem::semsleeplist.remove(&sleepblocked);

}

int KernelSem::wait(Time t) {
	lock();
	if (--val < 0) {
		if (t == 0) {
			PCB::running->isworking = 0;
			blocked.push(PCB::running);
//			lock();
//			cout << "usao u block" << PCB::running->id << endl;
//			unlock();

		} else {
			PCB::running->isworking = 0;
			sleepblocked.push(PCB::running, t);
//			lock();
//			cout << "usao u sleepa" << PCB::running->id << endl;
//			unlock();
		}

		unlock();
		dispatch();
		lock();

//		lock();
//		cout << "izasao iz sleepa" << PCB::running->id << endl;
//		unlock();
		if ((PCB::running->wasunblocked) == 1) {
			PCB::running->wasunblocked = 0;
			unlock();
			return 0;
		}
	}
	unlock();
	return 1;
}

int KernelSem::signal(int n) {
	lock();
	if (n < 0)
		return n;

	int cnt = 0;
	int delta;
	if (n == 0)
		delta = 1;
	else
		delta = n;

	if (val < 0) {
		val += delta;
		for (int i = 0; i < delta && !blocked.isEmpty(); i++) {
			PCB* ptr = blocked.pop();
			ptr->isworking = 1;
			Scheduler::put(ptr);
			cnt++;
		}

		for (int j = cnt; j < delta && !sleepblocked.isEmpty(); j++) {
			PCB* ptr = sleepblocked.pop();
			ptr->isworking = 1;
			lock();
			cout << "nevaljali signal" << endl;
			unlock();
			Scheduler::put(ptr);
			cnt++;
		}
		unlock();
		return cnt;
	}
	val += delta;
	unlock();
	return 0;
}

int KernelSem::getVal() const {
	return val;
}
