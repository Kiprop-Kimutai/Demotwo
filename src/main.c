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

#include "../src/utilities/common_functions.h"

//database Operations
//#include "database.h"

int main(int argc, char *argv[])
{
	int selected = 0;
	int retval = 0;
	char * date_t;
	char * receipt_t;
	lcd_init(&argc, &argv);
	lcd_set_bk_color(COLOR_WITE);
	lcd_set_font_color(COLOR_BLACK);

	printf("%s %s\n",date_t, receipt_t  );
	sys_get_sn(pos_serial_number,40);
	const char  main_menu[][100] = {"Beneficiary Transactions","Merchant  Transactions","POS/User Management" , "Exit"};

	//strcpy(pos_serial_number ,  get_)
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

