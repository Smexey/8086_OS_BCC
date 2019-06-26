/*
 * SysCall.cpp
 *
 *  Created on: May 16, 2019
 *      Author: OS1
 */

#include "SysCall.h"
#include "PCB.h"
#include "KSem.h"
#include "lock.h"
#include <IOSTREAM.H>

extern int lockFlag;
volatile int requestedDispatch = 0;
volatile unsigned spReg, bpReg, ssReg;
extern int tick();

Time TimeCount;
extern InterruptRoutine oldTimerRoutine;

void interrupt Timer(...) {

	// Ako nije pozvana zovemo stari timer
	if (!requestedDispatch) {
		tick();
		KernelSem::semsleeplist.decschedule(); //dekrementira sve
		TimeCount++;
		(*oldTimerRoutine)(); /// call old Timer
//		lock();
//		printf("TIMER %d\n",TimeCount);
//		unlock();
	}

	// Promena konteksta ako je nit gotova ili ako je zatrazena
	if (((PCB::running->timeslice <= TimeCount)&&(PCB::running->timeslice)) || requestedDispatch) {
		if (!lockFlag) {

#ifndef BCC_BLOCK_IGNORE
			asm {
				mov spReg,sp
				mov bpReg,bp
				mov ssReg,ss
			}
#endif
			//cuva stare
			PCB::running->sp = spReg;
			PCB::running->bp = bpReg;
			PCB::running->ss = ssReg;

			//stavlja nazad u scheduler ako nije idle i ako treba da radi
			if (PCB::running != PCB::idle && PCB::running->isworking == 1) {
				Scheduler::put(PCB::running);
			}

			PCB::running = Scheduler::get();

			//ako je prazan onda je idle
			if (PCB::running == 0){
				PCB::running = PCB::idle;
//				printf("idle ");
			}
//			locki();
//			cout << "running swap u:" << PCB::running->id << endl;
//			unlocki();

			//kod za signale ide ovde???
			spReg = PCB::running->sp;
			bpReg = PCB::running->bp;
			ssReg = PCB::running->ss;
			//novi kontekst
#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp,spReg
				mov bp,bpReg
				mov ss,ssReg
			}
#endif

			if (requestedDispatch) {
				requestedDispatch = 0;
			} else
				TimeCount = 0;

			//ako se promeni kontekst handle signale
			if (PCB::running != PCB::idle && PCB::running != PCB::mainpcb) {
				PCB::running->handlesignals();
//				printf("vratio se iz handlesignals ");
			}


		} else {
			requestedDispatch = 1;
		}

	}

}
