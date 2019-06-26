/*
 * Sem.h
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#ifndef H_SEM_H_
#define H_SEM_H_


typedef unsigned int Time;
class KernelSem;
class Semaphore {
public:
	Semaphore(int init = 1);
	virtual ~Semaphore();
	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n = 0);
	int val() const; // Returns the current value of the semaphore
private:
	KernelSem* myImpl;
};

#endif /* H_SEM_H_ */
