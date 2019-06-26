/*
 * SQueue.cpp
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#include"SQueue.h"


#include "iostream.h"
#include "PCB.h"
#include "SCHEDULE.h"

void SleepQueue::push(PCB* elem, Time wait){
//	printf("sleepblockpush\n");
	if(first==0)
		first=new SleepNode(elem,wait);
	else{
		if(first->wait>wait){
			SleepNode* newN=new SleepNode(elem,wait);
			newN->next=first;
			first->wait-=wait;
			first=newN;
		}
		else{
			SleepNode* tek=first;
			SleepNode* pr=0;
			while (tek!=0 && tek->wait<=wait){
				wait-=tek->wait;
				pr=tek;
				tek=tek->next;
			}
			SleepNode* newN=new SleepNode(elem,wait);
			pr->next=newN;
			newN->next=tek;
			if(tek!=0)
				tek->wait-=wait;
		}
	}
}

int SleepQueue::decschedule(){
	//dec prvog
	if(first!=0)
			first->wait--;
//	printf("decschedulestart time==%d\n",first->wait);
	//svaki koji treba da izadje izlazi
	SleepNode* tek=first;
	int cnt = 0;
	while (first!=0 && first->wait<=0){
//		lock();
//		cout<<"sleepovanjem removan id="<<tek->elem->id<<endl;
//		unlock();

		tek=first;
		tek->elem->wasunblocked = 1;//za wait povratnu vrednost
		tek->elem->isworking = 1;
		Scheduler::put(tek->elem);
		first=first->next;
		first->wait+=tek->wait;
		delete tek;
	}
	return cnt;
}

PCB* SleepQueue::pop(){
	//obican pop

	SleepNode* newN=first;
	if(first==0)
		return 0;
	first=newN->next;
	//prebaci ostatak vremena na sledeceg prvog
	first->wait += newN->wait;
	PCB* elem=newN->elem;
	delete newN;
	return elem;
}

SleepQueue::~SleepQueue() {
	SleepNode* tek;
	while (first!=0){
			tek=first;
			first=first->next;
			delete tek;
		}
}

