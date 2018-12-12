/*
 * beneficiary_transactions.c
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */

#include <stdlib.h>
#include "common_includes.h"
#include "beneficiary_transactions.h"
#include "../src/utilities/lcd.h"
#include "utilities/keyboart.h"

/*
 * Function to  register beneficiary
 */
void register_beneficiary(void );

void beneficiary_transactions(void )
{
	int selected = 0;
	const char  beneficiary_menu[][100] = {"Beneficiary Transaction","Beneficiary Registration","Balance Inquiry/Update","Mini-Statement" , "Exit"};
	while(1)
	{
		switch(lcd_menu("Beneficiary Transactions", beneficiary_menu, 5 ,selected))
		{
		case 0:
			//beneficiary_transaction_operations();
			printf("Beneficiary Transaction\n");
			break;
		case 1:
			register_beneficiary();
			break;
		case 2:
			printf("Balance Inquiry/Update\n");
			break;
		case 3:
			printf("Mini-Statement");
			break;
		case 4 :
			return;
			break;
		}
	}
}

void register_beneficiary(void ){
	int key=kb_getkey();
	message_display_function(1, "","Beneficiary Registration" ,  "Please tap the NFC card and press enter key");

	if(key==DIKS_ENTER){

	}
	else
		return;
}
