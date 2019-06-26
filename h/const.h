/*
 * const.h
 *
 *  Created on: May 15, 2019
 *      Author: OS1
 */

#ifndef OS_H_CONST_H_
#define OS_H_CONST_H_


#define unlockintinsideint() { asm pushf; asm sti;}
#define lockintinsideint(){ asm popf;}

#define TimerRoutine 0x0008

typedef void interrupt (*InterruptRoutine)(...);

#define PtrSegOff 1
#define PtrOffOff 2
#define PSWOff 5
#define SegOff 6
#define OffOff 7
#define STOff 16

#define locki() { asm pushf; asm cli; }
#define unlocki() { asm popf; }

//makro prepentry

#endif /* OS_H_CONST_H_ */
