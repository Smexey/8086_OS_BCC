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
}

void SysRestore() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(TimerRoutine, oldTimerRoutine);
	asm sti;
#endif
}
