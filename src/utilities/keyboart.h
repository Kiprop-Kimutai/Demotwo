

#ifndef KEYBOART_H_
#define KEYBOART_H_

//Defines the label word during request for input
char * label_one;
char * label_two;
int flag_getting_pwd;
int flag_getting_pwd1;
int flag_getting_pwd2;
int login_successful;
int su_login_successful;
char * key_board_title;
#include <stdint.h>
#include "cJSON.h"
int add_message_below_input;
//Functions and methods
//int kb_getString(uint8_t ucMode, uint8_t ucMinlen, uint8_t ucMaxlen,char *pszOut, const char *ch, char *charact);
int kb_getStringtwo(uint8_t ucMode, uint8_t ucMode1,  uint8_t ucMinlen, uint8_t ucMaxlen,char *pszOut,char *pszOut1 ,  const char *ch, char *charact, char *charact2, char * title , int display_string_below);
void message_display_function(int image_present , char * imageurl , char * title ,char * message , ...);
void message_display_function_extended(int image_present , char * imageurl , char * title ,const char* format, ...);
int message_display_getdate( char * day , char * month);
int confirm_screen(char  * header ,  cJSON * object);
#endif
