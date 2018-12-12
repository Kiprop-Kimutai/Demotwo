/*
 * merchant_transactions.c
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */



#include <stdlib.h>
#include "common_includes.h"
#include "merchant_transactions.h"

void merchant_transactions(void )
{
	int selected = 0;
	const char  merchant_menu[][100] = {"Merchant Account Inquiry", "Withdrawal To Bank","Store to  Store transfer", "Roll Up  Transfer", "Exit"};
	while(1)
	{
		switch(lcd_menu("Merchant Transactions", merchant_menu, 5 ,selected))
		{
		case 0:
			//create payload to request balance and ministatement;
			printf("Merchant Account Inquiry");
			break;
		case 1:
			printf("Withdrawal To Bank\n");
			break;
		case 2:
			printf("Store to  Store transfer");
			break;
		case 3:
			printf("Roll Up  Transfer");
			break;
		case 4 :
			return;
			break;
		}
	}

}
