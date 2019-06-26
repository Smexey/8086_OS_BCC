/*
 * IVTEntry.cpp
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */



#include "IVTEntry.h"
#include <dos.h>

#include "KEvent.h"


IVTEntry * IVTEntry::vectorTable[256] = { 0 };


IVTEntry::IVTEntry(IVTNo intNo, InterruptRoutine newInt){
	locki();
	event = 0;

	// Dodeljujemo broj
	intNum = intNo;
	vectorTable[intNo] = this;


	// Preusmeravamo prekid
#ifndef BCC_BLOCK_IGNORE
	oldInt = getvect(intNo);
	setvect(intNo, newInt);
#endif;


	unlocki();
}

// Destrutkor
IVTEntry::~IVTEntry(){
	locki();
	// Ukloni iz liste prekida
	vectorTable[intNum] = 0;
	// Postavi stari prekid
#ifndef BCC_BLOCK_IGNORE
	setvect(intNum, oldInt);
#endif
	// Pozovi staru prekidnu rutinu
	callOldInt();

	unlocki();
}

// Zovemo staru prekidnu rutinu
void IVTEntry::callOldInt(){
	locki();

	if(oldInt)
		(*oldInt)();
	unlocki();
}

// pozivamo signal
void IVTEntry::signal(){
	locki();
	if(event){
		event->signal();
	}
	unlocki();
}
