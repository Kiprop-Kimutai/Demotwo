#include "main_old.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../src/utilities/lcd.h"



const char menu[][100] = {
		"Display",
		"Key Board",
		"Touch Screen",
		"Printer",
		"Contact Smartcard",
		"Contactless Smartcard",
		"Magnetic Strip Card",
		"Uart",
		"Modem",
		"GPRS",
		"Socket Server",
		"Socket Client",
		"SSL Server",
		"SSL Client"
};
const char menu1[][100] = {
		"Register Beneficiary",
		"Transaction",
		"My Account",
		"Card Ops"
};

extern void Display_Demo(void);
extern void key_board_demo(void);
extern void transaction1(void);
extern void register_ben(void);
extern void my_account(void);
extern void create_all_table();
extern void Printer_Demo(void);
extern int read_config();
extern void set_innitial_configuratio();
extern void ContactSmartCard_Demo(void);
extern void ContactlessSmartCard_Demo(void);
extern void magneticstrip_demo(void);
extern void uart_demo(void);

extern void socket_server_demo(void);
extern void socket_client_demo(void);
extern void ssl_server_demo(void);
extern void ssl_client_demo(void);
extern void modem_demo(void);
extern void gprs_demo(void);
extern void touch_demo(void);

#define CL_BACK_GROUND_PIC_FILE "cl_bk_pic.jpg"
void design_url(char * parenttype, char * parent_id)
{}
void get_offline_menu(char * parenttype, char * parent_id)
{

	/*char final[1000];
	char str[100];

	printf("PID		: %s\n", parent_id);
	printf("parentT	: %s\n", parenttype);


	rootMenuIdent = 0;
	if (subservice_id != 0) {
		free(subservice_id);
		subservice_id = 0;
	}
	if (service_level != 0) {
		free(service_level);
		service_level = 0;
	}
	if (parentType != 0) {
		free(parentType);
		parentType = 0;
	}
	if (params != 0) {
		free(params);
		params = 0;
	}

	if (charges != 0) {
		free(charges);
		charges = 0;
	}
	if (parentId != 0) {
		free(parentId);
		parentId = 0;
	}
	if (subservice_name != 0) {
		free(subservice_name);
		subservice_name = 0;
	}
	strcpy(charge, "");
	strcpy(response, "");



	if (strcmp(parenttype, "SERVICEDTL") == 0)

	{
		sprintf(final,"select * from SERVICEDTLS where id = '%s';", parent_id);
		printf("\n%s",final);
		read_database(final, "services");
		if(my_offline_params!=0)
			free(my_offline_params);
		my_offline_params = malloc(100);
		strcpy(my_offline_params,"");
		if(params!=0)
		{

			if(strcmp(params[0],"-1")==0)
			{
				strcpy(my_offline_params,"-1");
				printf("Trying 23to get a value\n");

			}

			else
			get_params_for_offline();
		}
		else{

		}

		if(strlen(my_offline_params)==0){
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "The provided parameters ");
			lcd_printf(ALG_CENTER, "are not found. Please");
			lcd_printf(ALG_CENTER, "ask admin if the service");
			lcd_printf(ALG_CENTER, "is available or valid");
			lcd_flip();
			kb_getkey();
		}
	}

	else if (strcmp(parenttype, "SERVICEDATA") == 0)
	{
		strcpy(final,"select * from SERVICES;");
		read_database(final, "services");

		rootMenuIdent = 1;
	}

	else if (strcmp(parenttype, "SUBSERVICE") == 0)
	{

		sprintf(final,"select * from SUBSERVICES where parentId = '%s' ;", parent_id,main_service_level);
		printf("\n%s",final);
		read_database(final, "services");
	}*/

}

int main_old(int argc, char *argv[]){
	int retval = 0;
	int i, selected;
	lcd_init(&argc, &argv);
	lcd_set_bk_color(COLOR_WITE);
	lcd_set_font_color(COLOR_BLACK);
	i = 0;
	create_all_table();
	selected = 0;
	while (selected>=0){
		selected = lcd_menu("POC Application", menu1, sizeof(menu1)/100, selected);
		switch (selected){
			case 0:
//				Display_Demo();
				register_ben();
				//read_config();

				break;
			case 1:
				 transaction1();
				//key_board_demo();
				break;
			case 2:
				my_account();
				break;
			case 3:
				peformCardOperations();
				break;
			default:
				break;
		}
	}

	//lcd_uninit();
	return retval;
}
