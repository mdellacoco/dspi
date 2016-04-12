/*
 * ConfigFile.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: victoria
 */

#include "ConfigFile.h"

/*
 * File:   ConfigFile.cpp
 * Author: Margarita E. Della Sera
 *
 * Created on April 6, 2016, 11:36 AM
 */

#include "ConfigFile.h"

/**
 * Constructor
 */

ConfigFile::ConfigFile() {
}

/**
 * Copy constructor
 * @param orig
 */

ConfigFile::ConfigFile(const ConfigFile& orig) {
}
/**
 * Destructor
 */

ConfigFile::~ConfigFile() {
}

/**
 * This function reads the conf parameters written in a given config file and
 * stores them locally in variables.
 */

void ConfigFile::ReadConfigParams(){

    int fd;
    std::filebuf fb;
    std::string configParams[9] = {"", "", "", "", "", "", "", "", ""};

    char part1[256]; //part 1 of line with the written parameters
    char part2[256]; // part 2 of line.

    int i = 0;

    if (fb.open (_fileProperties.FILENAME.c_str() ,std::ios::in)) {
        std::istream is(&fb);

        while (!is.eof()) {

            is.getline(part1, 256, ' ');
            is.getline(part2, 256);

            configParams[i] = part2;
            i++;
        }


      _config._appName = configParams[appName];
      _config._spiParams._device = configParams[device];
      _config._spiParams._spiMode = configParams[spiMode];
      _config._spiParams._spiBPW = configParams[spiBPW];
      _config._spiParams._spiSpeed = configParams[spiSpeed];
      _config._spiParams._spiDelay = configParams[spiDelay];
      _config._serverParams._servername = configParams[serverName];
      _config._serverParams._portNo = configParams[portNo];

      printf("here is fine 1 ..\n");

    }
}

