/*
 * fingerprint_reader.h
 *
 *  Created on: Dec 6, 2018
 *      Author: linux
 */

#ifndef SRC_FINGERPRINT_READER_H_
#define SRC_FINGERPRINT_READER_H_
#include "utilities/cJSON.h"

//unsigned char   stored_fingerprint[600];
int fplib_test(int option , cJSON * Biodata);
#include "utilities/common_functions.h"

#endif /* SRC_FINGERPRINT_READER_H_ */
