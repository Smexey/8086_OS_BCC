#include "lock.h"
#include "Thread.h"

volatile int lockFlag = 0;

extern int requestedDispatch;

void lock(){
	lockFlag++;
}

void unlock(){

	--lockFlag;
	if(requestedDispatch && !lockFlag) dispatch();
}
