//============================================================================
// Name        : dspi.cpp
// Author      : Margarita E. Della Sera
// Version     :
// Copyright   : @copy righted
// Description : dspi in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <cstdio>
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
#include <thread>
#include "spi.h"
#include "SystemLog.h"
#include "daemonize.h"
#include "Logger.h"
#include "ClientSocket.h"
#include "ConfigFile.h"
#include "MenuComposition.h"
#include "MenuPrimitive.h"




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
    	cerr << "Cannot Load symbol create: " << msg.c_str() << '\n';
    	myAppSysLog.writeErrLog(msg.c_str());
    	msg.clear();
    	dlerror();
    	return 1;
    }

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

    // cast



    //creates an instance of the class

    spi* spiDev = createSpiDev();



    //castedSpiDev()


   // spiDev->openDevice(pConfigFileParams->_spiParams._device.c_str());
    int fOpen = spiDev->openDevice(pConfigFileParams->_spiParams._device.c_str());

    printf("File open is .. [%i]\n", fOpen);

    if(fOpen >= 0) {

       spiDev->setLog(&myAppSysLog);

    //Initialize spi values

      spiDev->setSpiSpeed((uint32_t)atoi(pConfigFileParams->_spiParams._spiSpeed.c_str()));
      spiDev->setSpiSpeed((uint16_t)atoi(pConfigFileParams->_spiParams._spiDelay.c_str()));

      spiDev->spiInit();  //TODO: check for successful initialisation

     // spiDev->Run();

      printf("\n\n\n\n");

      enum {v_dc_offset = 1, i_dc_offset, iv_dc_offset, v_ac_offset, i_ac_offset,
    	  v_dc_gain}; //TODO .. add remaining calibration types

           MenuComposition* pRootMenu = new MenuComposition("CS5463 Menu", 0);

           //Level 1 Menu
           MenuComposition* pCalibrate = new MenuComposition("Calibration", 1);
           MenuComposition* pMeasure = new MenuComposition("Measure", 1);

           pRootMenu->Add(pCalibrate);
           pRootMenu->Add(pMeasure);

           pMeasure->Add(new MenuPrimitive("Measure", 4, 0,
                       		          "...About to perform Measurements .. ", spiDev));

           MenuComposition* pOffsetCal = new MenuComposition("Offset Calibration", 2);
           MenuComposition* pGainCal = new MenuComposition("Gain Calibration", 2);

           pCalibrate->Add(pOffsetCal);
           pCalibrate->Add(pGainCal);

           MenuComposition* pDCOffset = new MenuComposition("DC Offset", 3);
           MenuComposition* pACOffset = new MenuComposition("AC Offset", 3);

           pOffsetCal->Add(pDCOffset);
           pOffsetCal->Add(pACOffset);

           // DC Offset calibrations: 3 in total
           pDCOffset->Add(new MenuPrimitive("Voltage DC Offset", 4, v_dc_offset,
        		          "...About to perform Voltage DC Offset Calibration .. ", spiDev));

           pDCOffset->Add(new MenuPrimitive("Current DC Offset", 4, i_dc_offset,
        		   "...About to perform Current Channel DC Offset Calibration", spiDev));

           // AC Offset Calibration: 3 in total
           pACOffset->Add(new MenuPrimitive("Voltage AC Offset", 4, v_ac_offset,
 		          "...About to perform Voltage AC Offset Calibration .. ", spiDev));

           pACOffset->Add(new MenuPrimitive("Current AC Offset", 4, i_ac_offset,
            		          "...About to perform Current AC Offset Calibration .. ", spiDev));

           //Gain Calibration
           cout <<endl << endl;
           MenuComposition* pDCGainCalibration = new MenuComposition("DC Gain Calibration", 3);
           MenuComposition* pACGainCalibration = new MenuComposition("AC Gain Calibration", 3);

           pGainCal->Add(pDCGainCalibration);
           pGainCal->Add(pACGainCalibration);

           pDCGainCalibration->Add(new MenuPrimitive("Voltage DC Gain Calibration", 4, v_dc_gain,
  		          "...About to perform Voltage DC Gain Calibration .. ", spiDev));

           pRootMenu->Display();
    }

   // destroy the spi device
    destroySpiDev(spiDev);

   // unload the spiHandler library
    dlclose(spiDevHandler);

	return 0;
}



