/*
 * KEvent.cpp
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#include "KEvent.h"
#include "IVTEntry.h"
#include "PCB.h"

KernelEv::KernelEv(IVTNo intNo) {
	caller = PCB::running;

	intNumber = intNo;
	sem = 0;

	// Povezujemo sa tabelom
	IVTEntry * newEntry = IVTEntry::vectorTable[intNumber];

	if (newEntry && newEntry->event == 0) //da li je preparovan?
		newEntry->event = this;

}

KernelEv::~KernelEv() {
	// Zovemo signal da odblokiramo
	signal(); //smem li ovo da radim ili samo da putujem u sched?

	// Brisemo ulaz
	if (IVTEntry::vectorTable[intNumber]
			&& IVTEntry::vectorTable[intNumber]->event == this) // ako je ovaj ulaz odgovarajuc za ovu
		IVTEntry::vectorTable[intNumber]->event = 0; // raskidamo vezu
}

// Signal binarnom semaforu
void KernelEv::signal() {
	if (sem == 0) {
		sem = 1;
		caller->isworking = 1;
		Scheduler::put(caller); //ubaci se u scheduler

		dispatch(); //dispatch na neku novu nit
	}
}

void KernelEv::wait() {

	// ako je running caller moze uopste da se waituj
	if (PCB::running == caller) {
		sem = 0;
		caller->isworking = 0; //nece uci u scheduler
		dispatch();
	}

}

