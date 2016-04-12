/*
 * ConfigFile.h
 *
 *  Created on: Apr 11, 2016
 *      Author: victoria
 */

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include <string>
#include <iostream>
#include <fstream>
#include "spi.h"

typedef struct {
    const std::string FILENAME = "/etc/dspi/dspi.conf";
    std::string DELIM          =  "=";
    const int MAX_BUFFER       = 1024;
    int NUM_LINES              = 4;
}fileProperties;

typedef struct {
		std::string _device   = "";
		std::string _spiMode  = "";
		std::string _spiBPW   = "";
		std::string _spiSpeed = "";
		std::string _spiDelay = "";
}spiParamsStr;

typedef struct {

	std::string _servername = "";
	std::string _portNo     = "";
}serverParams;

typedef struct {
        const std::string  _daemonDir      = "/var/run";
        const std::string  _daemonPidFile  = "/var/run/dspi.pid";
        std::string  _appName;
        spiParamsStr _spiParams;
        serverParams _serverParams;

    }config;

class ConfigFile {
public:
    ConfigFile();
    ConfigFile(const ConfigFile& orig);
    virtual ~ConfigFile();

    void ReadConfigParams();
    config* getConfigParams(){return &_config;}
private:
    fileProperties _fileProperties;
    config         _config;
    enum{appName = 0, device, spiMode,spiBPW, spiSpeed, spiDelay, serverName, portNo};

};





#endif /* CONFIGFILE_H_ */
