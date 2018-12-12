/*
 * desfire_test.h
 *
 *  Created on: Oct 18, 2018
 *      Author: pos
 */

#ifndef SRC_DESFIRE_TEST_H_
#define SRC_DESFIRE_TEST_H_
#include "../src/utilities/keyboart.h"
#include "sgfplib.h"
//#include "seos.h"

enum apps{
	personal=0,
	biodata
};

char dataout[3000];
char *serial_number;

int createApplicationAndFileAndWriteData(char *filename,char *data);
//int createApplicationAndFileAndWriteData(char *applicationName,char *filename,char *data);
char *readDataFromMifareCard(char *applicationName,char *filename);
#endif /* SRC_DESFIRE_TEST_H_ */
