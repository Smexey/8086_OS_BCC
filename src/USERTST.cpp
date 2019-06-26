/*
 * USER.cpp
 *
 *  Created on: May 25, 2019
 *      Author: OS1
 */

#include "stdio.h"
#include "Sem.h"
#include "Thread.h"
#include "lock.h"

class TestThread: public Thread {
	const char * s;
	Semaphore * mysem;
public:
	TestThread(const char * s, Semaphore * mys) {
		this->s = s;
		mysem = mys;
	}
	virtual void run() {

		for (int i = 0; i < 10; i++) {

			mysem->wait(5);
			for (unsigned int k = 0; k < 30000; k++) {
				for (unsigned int x = 0; x < 30000; x++)
					;
			}

			mysem->signal(0);
		}
	}
	int retResult() {
		waitToComplete();
		return 0;
	}

	~TestThread() {
		waitToComplete();
	}
};

int userMain2(int argc, char* argv[]) {
	Semaphore s1;
	Semaphore s2;
	s1.wait(0);
	s2.wait(0);

	TestThread * tt1 = new TestThread("thr 1", &s1);
	TestThread * tt2 = new TestThread("thr 2", &s1);
	TestThread * tt3 = new TestThread("thr 3", &s2);
	TestThread * tt4 = new TestThread("thr 4", &s2);

	TestThread * tt5 = new TestThread("thr 5", &s1);
	TestThread * tt6 = new TestThread("thr 6", &s1);
	TestThread * tt7 = new TestThread("thr 7", &s2);
	TestThread * tt8 = new TestThread("thr 8", &s2);

	tt1->start();
	tt2->start();
	tt3->start();
	tt4->start();

	tt5->start();
	tt6->start();
	tt7->start();
	tt8->start();

	delete tt1;
	delete tt2;
	delete tt3;
	delete tt4;

	delete tt5;
	delete tt6;
	delete tt7;
	delete tt8;

	return 0;
}
