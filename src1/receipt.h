/*
 * receipt.h
 *
 *  Created on: Oct 24, 2014
 *      Author: tarus
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jsonread.h"
int receipt_num;

char *get_receipt_number( void );
void revert_receipt_number(void );
void set_get_z_number(int increament_operation);
int get_voided_number() ;
