/*
 * ThrKill.cpp
 *
 *  Created on: May 31, 2019
 *      Author: OS1
 */

#include "ThrKill.h"
#include "lock.h"
#include "IOSTREAM.H"
ThreadKiller ThreadKiller::threadkiller;

ThreadKiller::ThreadKiller() {
}
void ThreadKiller::run() {
	while (1) {
//		cout << "inthreadkill " << flush;
		while (1) {
			lock();
			if (tokill.isEmpty()) {
				break;
			}
			PCB * todel = tokill.pop();
//			cout << "threadkiller del: " << flush;
//			cout << todel->id << endl;
			delete todel->stack;

			while (!todel->waiting.isEmpty()) {
				PCB * next = todel->waiting.pop();
				if (next->isworking >= 0) {
					next->isworking = 1;
					Scheduler::put(next);
				}
			}
			todel->isworking = -1; //zavrsen stanje
			unlock();
		}
//		cout<<"threadkilleridvaljda = "<<PCB::running->id<<" ";
		unlock();
		PCB::running->isworking = 0; //gasi sam sebe
		dispatch();
	}
}
void ThreadKiller::insert(PCB* elem) {
	lock();
	tokill.push(elem);
	unlock();
}

ThreadKiller::~ThreadKiller() {
	//nema waittocomplete jer nema smisla
}

