/*
 * main.c
 *
 *  Created on: Oct 26, 2018
 *      Author: linux
 */
#include "utilities/inputs.h"

//Common includes
#include "common_includes.h"

//User management functions
#include "device_management.h"

//Beneficiary  management functions
#include "beneficiary_transactions.h"

//Merchant management functions
#include "merchant_transactions.h"

//database Operations
#include "database.h"

int main(int argc, char *argv[])
{
	int selected = 0;
	int retval = 0;
	lcd_init(&argc, &argv);
	lcd_set_bk_color(COLOR_WITE);
	lcd_set_font_color(COLOR_BLACK);
	const char  main_menu[][100] = {"Beneficiary Transactions","Merchant  Transactions","POS/User Management" , "Exit"};

	create_all_tables();
	while(1)
	{
		switch(lcd_menu("Safaricom ", main_menu, 4,selected))
		{

		case 0:
			beneficiary_transactions();
			break;
		case 1:
			merchant_transactions();
			break;
		case 2:
			device_management();
			break;
		case 3:
			exit(0);
			break;
		}
	}
	return 0;
}

