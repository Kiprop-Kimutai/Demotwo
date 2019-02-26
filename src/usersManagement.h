/*
 * usersManagement.h
 *
 *  Created on: Oct 28, 2018
 *      Author: linux
 */

#ifndef USERSMANAGEMENT_H_
#define USERSMANAGEMENT_H_

//Defines a user;
struct  pos_users
{
	char  username[30];
	char  firstname[30];
	char  lastname[30];
	char  pin[6];
	int  level;
	int  status;
};

struct pos_users active_pos_user;
int create_pos_users(void);
//	username, pin, merchantid, idnumber, agentid, userlevel
typedef struct posuser
{
	char username[50];
	char name[50];
	char pin[50];
	char idnumber[50];
	char status[50];
	char userlevel[50];
}
PosUserFile;
PosUserFile *myPosUser;
typedef struct merchant
{
	char MerchantName[50];
	char agentName[50];
	char merchantid[50];
	char agentid[50];
	char status[50];
}
MerchantFile;
MerchantFile *myMerchantUser;

typedef struct logindata
{
	char username[50];
	char  name[100];
	char pin[50];
	char idnumber[50];
	char status[50];
	char userlevel[50];
}
LoginUserFile;
LoginUserFile *myLoginPosUser;
/*
 * Function that  has all the details to  manage the POS users
 */
void manage_users_menu(void );

/*
 * Function that  is used to  load POS user parameters during login or upon request
 */
void initialize_user_params(void);

int request_operators(void);

void login(void) ;
int entered_pin;

 int user_found_true;

#endif /* USERSMANAGEMENT_H_ */
