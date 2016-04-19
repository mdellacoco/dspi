//============================================================================
// Name        : dspi.cpp
// Author      : Margarita E. Della Sera
// Version     :
// Copyright   : @copy righted
// Description : dspi in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdexcept>
#include <string.h>
#include <string>
#include <errno.h>
#include <linux/types.h>
#include <dlfcn.h>
#include <sys/unistd.h>
#include "spi.h"
#include "SystemLog.h"
#include "daemonize.h"
#include "Logger.h"
#include "ClientSocket.h"
#include "ConfigFile.h"



using namespace std;

int main(int argc, char** argv) {

	char* libName = argv[1];

	//Read config File

	ConfigFile _configFile;
	config* pConfigFileParams;

	_configFile.ReadConfigParams();

	pConfigFileParams = _configFile.getConfigParams();


	//Creates system daemon

	  myDaemon myDaemonObj(pConfigFileParams->_daemonDir.c_str(),
			               pConfigFileParams->_daemonPidFile.c_str());
	  myDaemonObj.daemonize();

      char pidNo[28];

      sprintf(pidNo, "%d", myDaemonObj.getPidNo());
      std::string pidNoStr(pidNo);

	// Creates a syslog to store information and errors about Notus app.

	   SystemLog myAppSysLog(pConfigFileParams->_appName.c_str());

	   char hostname[128];

	   int hn = gethostname(hostname, sizeof hostname);

	   loggerInfo myLoggerInfo;

	   myLoggerInfo.appName = pConfigFileParams->_appName;
	   myLoggerInfo.clientName = hostname;  //client
	   myLoggerInfo.pidNo = pidNo;

	   ClientSocket* pSocket;

	   pSocket = ClientSocket::Instance(pConfigFileParams->_serverParams._servername,
			                            pConfigFileParams->_serverParams._portNo);

	   Logger* pLogHandler;

	   pLogHandler = Logger::Instance("", pSocket, &myLoggerInfo);

	  std::string msg;
	  std::string Errormsg;

	// Loads the spi lib

    void* spiDevHandler = dlopen(libName, RTLD_NOW);
    std::string errorStr;

    if(!spiDevHandler) {
    	errorStr = dlerror();
    	msg = "Cannot Load Library: " + errorStr;

    	myAppSysLog.writeErrLog(msg.c_str());
    	msg.clear();
    	dlerror();
    	return 1;
    }

    // load the symbols


    create_t* createSpiDev = (create_t*) dlsym(spiDevHandler, "create");

    const char* dlsymError = dlerror();


    if(dlsymError) {

    	//cerr << "Cannot Load symbol create: " << dlerror() << '\n';
    	errorStr = dlsymError;
    	msg = "Cannot Load symbol create: " + errorStr;
    	myAppSysLog.writeErrLog(msg.c_str());
    	//reset errors
    	dlerror();
    	msg.clear();

    	return 1;
    }


    destroy_t* destroySpiDev = (destroy_t*)dlsym(spiDevHandler, "destroy");
    dlsymError = dlerror();

    if(dlsymError) {

        errorStr = dlsymError;
        msg = "Cannot load symbol destroy: " + errorStr;

        myAppSysLog.writeErrLog(msg.c_str());
        msg.clear();
        //reset errors
        dlerror();
        return 1;
        }

    //creates an instance of the class

    spi* spiDev = createSpiDev();

    spiDev->setLog(&myAppSysLog);

    spiDev->openDevice(pConfigFileParams->_spiParams._device.c_str());

    //Initialize spi values

    spiDev->setSpiSpeed((uint32_t)atoi(pConfigFileParams->_spiParams._spiSpeed.c_str()));
    spiDev->setSpiSpeed((uint16_t)atoi(pConfigFileParams->_spiParams._spiDelay.c_str()));

    spiDev->spiInit();

    spiDev->Run();

    // destroy the spi device
    destroySpiDev(spiDev);

    // unload the spiHandler library
    dlclose(spiDevHandler);

	return 0;
}
