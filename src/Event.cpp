/*
 * Event.cpp
 *
 *  Created on: May 27, 2019
 *      Author: OS1
 */

#include "Event.h"
#include "Const.h"
#include "KEvent.h"


// Konstruktor
Event::Event(IVTNo ivtNo){
	locki();
	myImpl = new KernelEv(ivtNo);
	unlocki();
}

// Destruktor
Event::~Event(){
	locki();
	delete myImpl;
	unlocki();
}

// Sacekaj
void Event::wait(){
	locki();
	myImpl -> wait();
	unlocki();

}

void Event::signal(){
	locki();
	myImpl -> signal();
	unlocki();
}

