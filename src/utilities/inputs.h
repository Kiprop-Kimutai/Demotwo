/*
 * inputs.h
 *
 *  Created on: Oct 26, 2018
 *      Author: linux
 */

#ifndef INPUTS_H_
#define INPUTS_H_

int menu_terminal(char * title  , const char menus[][100], int menu_size );
char ** getData_terminal(char * title  ,const char  data_requested[][100] , int menu_size);
void display_info_terminal(int title_print , char * title ,  char * message  , int clear );


#endif /* INPUTS_H_ */
