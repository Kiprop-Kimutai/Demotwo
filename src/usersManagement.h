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
/*
 * Function that  has all the details to  manage the POS users
 */
void manage_users_menu(void );

/*
 * Function that  is used to  load POS user parameters during login or upon request
 */
void initialize_user_params(void);

#endif /* USERSMANAGEMENT_H_ */
