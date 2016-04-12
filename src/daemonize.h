/*
 * daemonize.h
 *
 *  Created on: Apr 18, 2014
 *      Author: victoria
 */

#ifndef DAEMONIZE_H_
#define DAEMONIZE_H_

#include <string.h>
#include <stdio.h>
#include <errno.h>  //strerror()
#include <fcntl.h> //open()
#include <syslog.h>
#include <unistd.h>  //dup()
#include <stdlib.h>  // exit, EXIT_FAILURE
#include <stdexcept>
#include <sys/file.h> //lockf()
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

class myDaemon {
public:
	myDaemon(const char* rundir, const char* pidfileName);
	~myDaemon();

	int getPidNo(){return pidNo;}

	void daemonize();
private:
	void pidFileCreate();
	void stdFileDescriptorRoute();

private:
	int   m_childPid;
	int   m_sid;
	int   pidNo;
	int   m_pidfileHandler;
	const char* m_pRunDir;
	const char* m_pPidFileName;
};



#endif /* DAEMONIZE_H_ */
