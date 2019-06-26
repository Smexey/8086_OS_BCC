/*
 * SysInit.cpp
 *
 *  Created on: May 16, 2019
 *      Author: OS1
 */

#include "const.h"
#include <dos.h>
#include "PCB.h"
#include "SysCall.h"
InterruptRoutine oldTimerRoutine;

void SysInit() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	oldTimerRoutine = getvect(TimerRoutine);
	setvect(TimerRoutine,Timer);
	asm sti;
#endif
	PCB::idle = new PCB(500, 1);
	PCB* mainPCB = new PCB();
	PCB::running = mainPCB;
	PCB::mainpcb = mainPCB;
}

void SysRestore() {
#ifndef BCC_BLOCK_IGNORE
	asm{
			cli
			in      al, 61h         //; Send acknowledgment without
			or      al, 10000000b   //;   modifying the other bits.
			out     61h, al         //;
			and     al, 01111111b   //;
			out     61h, al         //;
			mov     al, 20h         //; Send End-of-Interrupt signal
			out     20h, al         //;
		}
	setvect(TimerRoutine, oldTimerRoutine);
	asm sti;
#endif
}
