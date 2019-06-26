/*
 * IVTEntry.h
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_
#include "const.h"

class KernelEv;
typedef unsigned char IVTNo;

class IVTEntry{
public:
	// Vektor prekida
	static IVTEntry * vectorTable[256];

	IVTEntry(IVTNo, InterruptRoutine);
	virtual ~IVTEntry();

	InterruptRoutine oldInt;
	KernelEv * event;
	IVTNo intNum;

	void callOldInt(); // Zovi staru prekidnu rutinu
	void signal();
};


#define PREPAREENTRY(intNo, callOld) \
void interrupt newInt##intNo(...); \
IVTEntry newEntry##intNo(intNo, newInt##intNo); \
void interrupt newInt##intNo(...) { \
	if (callOld) { \
		newEntry##intNo.callOldInt(); \
	} \
	newEntry##intNo.signal(); \
}


#endif /* H_IVTENTRY_H_ */
