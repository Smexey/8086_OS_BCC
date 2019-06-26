/*
 * Main.cpp
 *
 *  Created on: May 16, 2019
 *      Author: OS1
 */

#include "PCB.h"
#include "Thread.h"
#include "SysCall.h"
#include "SysInit.h"
#include "stdio.h"
#include "Sem.h"
#include "STDIO.H"

extern int userMain(int argc, char* argv[]);

class UserThread: public Thread {
private:
	int result;
	int argc;
	char** argv;
public:
	UserThread(int _argc, char** _argv, StackSize ss = defaultStackSize,
			Time ts = defaultTimeSlice) :
			Thread(ss, ts) {
		argc = _argc;
		argv = _argv;
	}
	void run() {
		result = userMain(argc, argv);
	}
	int retResult() {
		waitToComplete();
		return result;
	}
	~UserThread() {
		waitToComplete();
	}
};

int main(int argc,char * argv[]) {
	SysInit();

	UserThread * user = new UserThread(argc,argv);
	user->start();
	int result = user->retResult();
	delete user;
	SysRestore();
	printf("REZULTAT %d",result);
	return result;
}
