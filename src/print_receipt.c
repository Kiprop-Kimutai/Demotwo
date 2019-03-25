/*
 * print_receipt.c
 *
 *  Created on: Dec 13, 2018
 *      Author: linux
 */

#include <posapi.h>
#include <printer.h>
#include <directfb.h>

#include "../src/utilities/lcd.h"
#include "../src/utilities/string_convert.h"
#define printer_device_name "/dev/printer0"
#define font_file_xx "/usr/share/fonts/arialnb.ttf"
#include "../src/utilities/cJSON.h"
#include "../src/utilities/common_functions.h"

#include "usersManagement.h"
//#include "jsonread.h"
//#include "receipt.h"
//#include "main_old.h"
//static IDirectFBSurface *load_image(IDirectFB *dfb, const char *filename);
//void load_image_for_screen(IDirectFB *dfb, const char *filename ,IDirectFBSurface *main_surface );
void get_printstatus(int ifd, printer_status_t status);
//void PrintHeader(void);

extern IDirectFBSurface *main_surface;
const char *logo = "logo1.png";
//const char *filenames = "out.png";
#define DFBCHECK(x...)                                         \
		{                                                            \
	DFBResult err = x;                                         \
	\
	if (err != DFB_OK)                                         \
	{                                                        \
		fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ ); \
		DirectFBErrorFatal( #x, err );                         \
	}                                                        \
		}


static IDirectFBSurface *load_image(IDirectFB *dfb, const char *filename) {
	IDirectFBImageProvider *provider;
	IDirectFBSurface *imgsurf = NULL;
	DFBSurfaceDescription dsc;
	DFBResult err;
	//	IDirectFBFont *font_32 = NULL;


	dsc.height = 32;
	/*	dfb->CreateFont(dfb, font_file_xx, &dsc, &font_32);
	if (NULL == font_32) {
		lcd_printf(ALG_LEFT, "craete font16 failed.");
		lcd_flip();
		retval = -1;
	}*/


	err = dfb->CreateImageProvider(dfb, filename, &provider);
	if (err != DFB_OK) {
		fprintf( stderr, "Couldn't load image from file '%s': %s\n", filename,DirectFBErrorString(err));
		return NULL;
	}

	provider->GetSurfaceDescription(provider, &dsc);
	dsc.flags = DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT;
	dsc.pixelformat = DSPF_ARGB;
	if (dfb->CreateSurface(dfb, &dsc, &imgsurf) == DFB_OK)
		provider->RenderTo(provider, imgsurf, NULL);


	provider->Release(provider);

	return imgsurf;
}

int print_receipt(char * type_of_receipt , cJSON * transaction ,   int * printflag ,  int * print_complete) {
	int reprint = 0;
	int ifd = -1;
	int key = 0;
	char  dates[30];
	int retval = 0;
	char Recipt[100];
	int i;
	printer_param_t param;
	printer_status_t status;
	DFBSurfaceDescription surfdesc;
	IDirectFBSurface * surface = NULL;
	IDirectFBSurface * surface1 = NULL;
	IDirectFBFont *font_16 = NULL;
	IDirectFBFont *font_24 = NULL;
	//IDirectFBFont *font_48 = NULL;
	IDirectFBFont *font_32 = NULL;
	DFBFontDescription fdesc;
	//const char * disp_text = "HELLO WORLD";
	int x, y, height;
	int rectangle_x, rectangle_y, rectangle_width;
	int numb;
	char myholder[100];
	//const char * utf8text = string_covert(disp_text, strlen(disp_text));





	lcd_clean();
	screen_header();
	lcd_printf(ALG_LEFT, "Prepare for print...");
	lcd_flip();
	kb_hit();

	memset(&fdesc, 0, sizeof(fdesc));
	fdesc.flags = DFDESC_HEIGHT;

	fdesc.height = 16;
	dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_16);
	if (NULL == font_16) {
		lcd_printf(ALG_LEFT, "craete font16 failed.");
		lcd_flip();
		retval = -1;
	}

	if (0 == retval) {
		fdesc.height = 24;
		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_24);
		if (NULL == font_24) {
			lcd_printf(ALG_LEFT, "craete font16 failed");
			lcd_flip();
			retval = -1;
		}
	}

	if (0 == retval) {
		fdesc.height = 32;
		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_32);
		if (NULL == font_32) {
			lcd_printf(ALG_LEFT, "craete font32 failed");
			lcd_flip();
			retval = -1;
		}
	}
	if (0 == retval) {
		ifd = printer_open(printer_device_name, O_WRONLY | O_NONBLOCK);
		if (ifd < 0) {
			lcd_printf(ALG_LEFT, "Device Open failed.");
			lcd_flip();
		}
	}
	if (0 == retval) {
		printer_get_param(ifd, &param);
		surfdesc.flags = DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT;
		surfdesc.caps = DSCAPS_NONE;
		surfdesc.width = param.ro_width;
		surfdesc.height = 4000;
		dfb->CreateSurface(dfb, &surfdesc, &surface);
		surface->Clear(surface, 0xFF, 0xFF, 0xFF, 0xFF);
		surface->SetColor(surface, 0x00, 0x00, 0x00, 0xFF);
		dfb->CreateSurface(dfb, &surfdesc, &surface1);
		surface1->Clear(surface1, 0xFF, 0xFF, 0xFF, 0xFF);

		surface1->SetColor(surface1, 0x00, 0x00, 0x00, 0xFF);
		x = param.ro_width / 2;
		y = 20;

		rectangle_x = 5;
		rectangle_y = y - 10;
		rectangle_width = param.ro_width - 5 * 2,
				//Header:	surface->SetFont(surface, font_48);

				surface->DrawString(surface, "_________________________________*", -1,
						x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;

		surface->DrawString(surface, type_of_receipt, -1, x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 8;




		surface1->SetFont(surface1, font_32);
		surface1->DrawString(surface1, "", -1,x, y, DSTF_TOPCENTER);
		surface1->DrawString(surface1, "", -1,x, y, DSTF_TOPCENTER);
		surface1->DrawString(surface1, "", -1,x, y, DSTF_TOPCENTER);
		surface1->DrawString(surface1, "", -1,x, y, DSTF_TOPCENTER);
		//y += height + 1;




		surface->SetFont(surface, font_24);
		font_24->GetHeight(font_24, &height);

		{
			/*	Header
			 * {
					"Agent Name":"Laitoriat  Agencies",
					"Agent No.":"3423423423",
					"Location":"Moyale County ",
					"Terminal ""800000 ",
					"Operator":"Alexandar Kibii ",

				}*/
			surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
			surface->DrawString(surface, "Agent Name", -1, 5, y,DSTF_TOPLEFT);
			surface->DrawString(surface, ":", -1, 120, y,DSTF_TOPLEFT);
			surface->DrawString(surface,myMerchantUser->agentName , -1, 125, y,DSTF_TOPLEFT);
			y += height + 1;

			surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
			surface->DrawString(surface, "Agent No.", -1, 5, y,DSTF_TOPLEFT);
			surface->DrawString(surface, ":", -1, 120, y,DSTF_TOPLEFT);
			surface->DrawString(surface, myMerchantUser->agentid , -1, 125, y,DSTF_TOPLEFT);
			y += height + 1;

/*			surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
			surface->DrawString(surface, "Location", -1, 5, y,DSTF_TOPLEFT);
			surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
			surface->DrawString(surface,get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(Header,"Location"))) , -1, 140, y,DSTF_TOPLEFT);
			y += height + 1;*/

			surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
			surface->DrawString(surface, "Terminal", -1, 5, y,DSTF_TOPLEFT);
			surface->DrawString(surface, ":", -1, 120, y,DSTF_TOPLEFT);
			surface->DrawString(surface,pos_serial_number, -1, 125, y,DSTF_TOPLEFT);
			y += height + 1;

			surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
			surface->DrawString(surface, "Operator", -1, 5, y,DSTF_TOPLEFT);
			surface->DrawString(surface, ":", -1, 120, y,DSTF_TOPLEFT);
			surface->DrawString(surface,myLoginPosUser->username , -1, 125, y,DSTF_TOPLEFT);
			y += height + 1;

			surface->SetFont(surface, font_32);
					surface->DrawString(surface, "-----------------------------------", -1,x, y, DSTF_TOPCENTER);
					font_32->GetHeight(font_32, &height);
					y += height -8;


		}


		if(strcmp(type_of_receipt ,"CARD BALANCE RECEIPT") == 0)
		{
			/*			[{
						"wallet":	"M-PESA Account",
						"currency":	"SSP",
						"balance":	2330
					}, {
						"wallet":	"WFP Beneficiary Account",
						"currency":	"SSP",
						"balance":	4000
					}]*/
			int i;
			cJSON *balance_array;
			char  tmp_str[100];

			surface->SetFont(surface, font_24);
			font_24->GetHeight(font_24, &height);

			for (i  = 0; i<cJSON_GetArraySize(transaction); i++){
				balance_array = cJSON_GetArrayItem(transaction,i);
/*
				strcpy( wallet_currency[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "currency"))));
				strcpy( wallet_id[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "wallet"))));
				wallet_amount[i] = atof(cJSON_Print(cJSON_GetObjectItem(balance_array , "balance")));

*/


				surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
				surface->DrawString(surface, "wallet", -1, 5, y,DSTF_TOPLEFT);
				surface->DrawString(surface, ":", -1, 130, y,DSTF_TOPLEFT);
				y += height + 1;
				surface->DrawString(surface,get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array,"wallet"))) , -1, 70, y,DSTF_TOPLEFT);
				y += height + 1;


				surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
				surface->DrawString(surface, "Balance", -1, 5, y,DSTF_TOPLEFT);
				surface->DrawString(surface, ":", -1, 130, y,DSTF_TOPLEFT);
				y += height + 1;
				sprintf (tmp_str , "%s . %s" , get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array,"currency"))) , cJSON_Print(cJSON_GetObjectItem(balance_array , "balance")) );
				surface->DrawString(surface, tmp_str, -1, 70, y,DSTF_TOPLEFT);
				y += height + 1;
				surface->DrawString(surface, "______________________________", -1,x, y, DSTF_TOPCENTER);
				y += height + 1;
			}



		}
		if(strcmp(type_of_receipt ,"TRANSACTION RECEIPT") == 0)
		{
			char tmp_str[200];
			surface->SetFont(surface, font_24);
			font_24->GetHeight(font_24, &height);

			surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
			surface->DrawString(surface, "wallet", -1, 5, y,DSTF_TOPLEFT);
			surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
			y += height + 1;
			surface->DrawString(surface,get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction,"walletName"))) , -1, 70, y,DSTF_TOPLEFT);
			y += height + 1;

			surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
			surface->DrawString(surface, "Transaction Code", -1, 5, y,DSTF_TOPLEFT);
			surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
			y += height + 1;
			surface->DrawString(surface,get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction,"transId"))) , -1, 70, y,DSTF_TOPLEFT);
			y += height + 1;


			surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
			surface->DrawString(surface, "Transaction Amount", -1, 5, y,DSTF_TOPLEFT);
			surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
			y += height + 1;
			sprintf (tmp_str , "%s . %s" , get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction,"currency"))) , get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction,"amount"))) );

			surface->DrawString(surface,tmp_str, -1, 70, y,DSTF_TOPLEFT);
			y += height + 1;

/*			surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
			surface->DrawString(surface, "Currency", -1, 5, y,DSTF_TOPLEFT);
			surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
			surface->DrawString(surface,get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction,"Currency"))) , -1, 140, y,DSTF_TOPLEFT);
			y += height + 1;*/


			surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
			surface->DrawString(surface, "Date", -1, 5, y,DSTF_TOPLEFT);
			surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
			y += height + 1;
			surface->DrawString(surface,get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction,"Date"))) , -1, 70, y,DSTF_TOPLEFT);
			y += height + 1;




		}
		else if(strcmp(type_of_receipt ,"BALANCE ENQUIRY RECEIPT") == 0)
		{
			/*			[{
						"wallet":	"M-PESA Account",
						"currency":	"SSP",
						"balance":	2330
					}, {
						"wallet":	"WFP Beneficiary Account",
						"currency":	"SSP",
						"balance":	4000
					}]*/
			int i;
			cJSON *balance_array;
			char  tmp_str[100];

			surface->SetFont(surface, font_24);
			font_24->GetHeight(font_24, &height);

			for (i  = 0; i<cJSON_GetArraySize(transaction); i++){
				balance_array = cJSON_GetArrayItem(transaction,i);
/*
				strcpy( wallet_currency[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "currency"))));
				strcpy( wallet_id[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "wallet"))));
				wallet_amount[i] = atof(cJSON_Print(cJSON_GetObjectItem(balance_array , "balance")));

*/


				surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
				surface->DrawString(surface, "wallet", -1, 5, y,DSTF_TOPLEFT);
				surface->DrawString(surface, ":", -1, 130, y,DSTF_TOPLEFT);
				y += height + 1;
				surface->DrawString(surface,get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array,"wallet"))) , -1, 70, y,DSTF_TOPLEFT);
				y += height + 1;


				surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
				surface->DrawString(surface, "Balance", -1, 5, y,DSTF_TOPLEFT);
				surface->DrawString(surface, ":", -1, 130, y,DSTF_TOPLEFT);
				y += height + 1;
				sprintf (tmp_str , "%s . %s" , get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array,"currency"))) , cJSON_Print(cJSON_GetObjectItem(balance_array , "balance")) );
				surface->DrawString(surface, tmp_str, -1, 70, y,DSTF_TOPLEFT);
				y += height + 1;
				surface->DrawString(surface, "______________________________", -1,x, y, DSTF_TOPCENTER);
				y += height + 1;
			}

		}
		if(strcmp(type_of_receipt ,"CARD MINI STATEMENT") == 0)
		{
			/*			[{
						"wallet":	"M-PESA Account",
						"currency":	"SSP",
						"balance":	2330
					}, {
						"wallet":	"WFP Beneficiary Account",
						"currency":	"SSP",
						"balance":	4000
					}]*/
			int i;
			char  tmp_str[100];

			surface->SetFont(surface, font_24);
			font_24->GetHeight(font_24, &height);

			for (i  = 0; i<cJSON_GetArraySize(transaction); i++){


				cJSON * balance_array = cJSON_GetArrayItem(transaction,i);

				char * receiptNumber = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "ReceiptNumber")));
				char * transactionStatus = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "TransactionStatus")));
				char * currency = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "Currency")));
				char * amount = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "Amount")));
				char * initiatedTime = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "InitiatedTime")));
				char * details = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "Details")));

				strcpy(dates,get_date_from_string(initiatedTime));
				//printf("XXXXXXXXXXXXXXXXXXXXXXX : %s\n", dates);

				printf ("%d: receiptNumber : %s transactionStatus : %s currency : %s amount: %s initiatedTime: %s details : %s\n" , i+1 , receiptNumber, transactionStatus, currency, amount,initiatedTime, details );


				surface->DrawString(surface,"--------------------------------------------------------", -1, rectangle_x, y, DSTF_TOPLEFT);
																y += height + 1;	//surface->DrawString(surface,"==========================", -1,rectangle_x+135, y, DSTF_TOPLEFT);
					y += height + 1;
					surface->DrawString(surface,"ReceiptNo: ", -1, rectangle_x, y, DSTF_TOPLEFT);
					surface->DrawString(surface, receiptNumber, -1,rectangle_x+135, y, DSTF_TOPLEFT);
					y += height + 1;
				/*	char  balance1[50];
					sprintf(balance1,"%.2f" ,balance);*/
					surface->DrawString(surface, "TxnStatus ", -1, rectangle_x, y, DSTF_TOPLEFT);
					surface->DrawString(surface,transactionStatus, -1, rectangle_x+135, y, DSTF_TOPLEFT);
					y += height + 1;

					surface->DrawString(surface, "Currency ", -1, rectangle_x, y, DSTF_TOPLEFT);
					surface->DrawString(surface,currency, -1, rectangle_x+135, y, DSTF_TOPLEFT);
					y += height + 1;
					surface->DrawString(surface,"Amount: ", -1, rectangle_x, y, DSTF_TOPLEFT);
					surface->DrawString(surface, amount, -1,rectangle_x+135, y, DSTF_TOPLEFT);
					y += height + 1;

					surface->DrawString(surface, "InitiatedTime ", -1, rectangle_x, y, DSTF_TOPLEFT);
					surface->DrawString(surface,dates, -1, rectangle_x+135, y, DSTF_TOPLEFT);
					y += height + 1;

					surface->DrawString(surface, "details ", -1, rectangle_x, y, DSTF_TOPLEFT);
					surface->DrawString(surface,details, -1, rectangle_x+135, y, DSTF_TOPLEFT);
					y += height + 1;

  }



		}

		/*** Merchant Print outs **/


		if(strcmp(type_of_receipt, "Balance")==0){
			char * date[100];
			surface->SetFont(surface, font_24);
			font_24->GetHeight(font_24, &height);
		for (i  = 0; i<cJSON_GetArraySize(transaction); i++)
					{

						double balance;
						cJSON * balance_array;
						char * card_wallet_id ;
						char * currency;
						 balance_array = cJSON_GetArrayItem(transaction,i);
						balance = atof(cJSON_Print(cJSON_GetObjectItem(balance_array , "balance")));
						card_wallet_id = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "wallet")));
						currency = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "currency")));

						printf ("%d: card_wallet : %s currency : %s balance : %1f\n" , i+1 , card_wallet_id, currency, balance);
						surface->DrawString(surface,"--------------------------------------------------------", -1, rectangle_x, y, DSTF_TOPLEFT);
														y += height + 1;
							surface->DrawString(surface,"wallet_name: ", -1, rectangle_x, y, DSTF_TOPLEFT);
							surface->DrawString(surface, card_wallet_id, -1,rectangle_x+135, y, DSTF_TOPLEFT);
							y += height + 1;
							char  balance1[50];
							sprintf(balance1,"%.2f" ,balance);
							surface->DrawString(surface, "Amount ", -1, rectangle_x, y, DSTF_TOPLEFT);
							surface->DrawString(surface,balance1, -1, rectangle_x+135, y, DSTF_TOPLEFT);
							y += height + 1;

							surface->DrawString(surface, "currency ", -1, rectangle_x, y, DSTF_TOPLEFT);
							surface->DrawString(surface,currency, -1, rectangle_x+135, y, DSTF_TOPLEFT);
							y += height + 1;

		  }
		}
		if(strcmp(type_of_receipt, "Mini statement")==0){

			surface->SetFont(surface, font_24);
			font_24->GetHeight(font_24, &height);
			for (i  = 0; i<cJSON_GetArraySize(transaction); i++)
							{


								cJSON * balance_array = cJSON_GetArrayItem(transaction,i);

								char * receiptNumber = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "ReceiptNumber")));
								char * transactionStatus = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "TransactionStatus")));
								char * currency = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "Currency")));
								char * amount = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "Amount")));
								char * initiatedTime = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "InitiatedTime")));
								char * details = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "Details")));

								 strcpy(dates,get_date_from_string(initiatedTime));
								// printf("XXXXXXXXXXXXXXXXXXXXXXX : %s\n", dates);
								//printf ("%d: receiptNumber : %s transactionStatus : %s currency : %s amount: %s initiatedTime: %s details : %s\n" , i+1 , receiptNumber, transactionStatus, currency, amount, dates, details );


								surface->DrawString(surface,"--------------------------------------------------------", -1, rectangle_x, y, DSTF_TOPLEFT);
																				y += height + 1;	//surface->DrawString(surface,"==========================", -1,rectangle_x+135, y, DSTF_TOPLEFT);
									y += height + 1;
									surface->DrawString(surface,"ReceiptNo: ", -1, rectangle_x, y, DSTF_TOPLEFT);
									surface->DrawString(surface, receiptNumber, -1,rectangle_x+125, y, DSTF_TOPLEFT);
									y += height + 1;
								/*	char  balance1[50];
									sprintf(balance1,"%.2f" ,balance);*/
									surface->DrawString(surface, "Status ", -1, rectangle_x, y, DSTF_TOPLEFT);
									surface->DrawString(surface,transactionStatus, -1, rectangle_x+125, y, DSTF_TOPLEFT);
									y += height + 1;

									surface->DrawString(surface, "Currency ", -1, rectangle_x, y, DSTF_TOPLEFT);
									surface->DrawString(surface,currency, -1, rectangle_x+125, y, DSTF_TOPLEFT);
									y += height + 1;
									surface->DrawString(surface,"Amount: ", -1, rectangle_x, y, DSTF_TOPLEFT);
									surface->DrawString(surface, amount, -1,rectangle_x+125, y, DSTF_TOPLEFT);
									y += height + 1;

									surface->DrawString(surface, "Time ", -1, rectangle_x, y, DSTF_TOPLEFT);
									surface->DrawString(surface,dates, -1, rectangle_x+125, y, DSTF_TOPLEFT);
									y += height + 1;

									surface->DrawString(surface, "Details ", -1, rectangle_x, y, DSTF_TOPLEFT);
									surface->DrawString(surface,details, -1, rectangle_x+125, y, DSTF_TOPLEFT);
									y += height + 1;

				  }

		}
		if(strcmp(type_of_receipt, "Roll Up Transfer")==0){
			char * transId;
			char ammount[10];
			 int amount;
			char * debitPartyName;
			char * creditPartyName;
			char * debitAccountBalance;
			char * creditAccountCurrentBalance ;
			char * finalizedTime;
			char * transactionStatus;
			surface->SetFont(surface, font_24);
			font_24->GetHeight(font_24, &height);

			 transId = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction , "transId")));
			 debitPartyName = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction , "debitPartyName")));
			 creditPartyName = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction , "creditPartyName")));
			 debitAccountBalance = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction , "debitAccountBalance")));
			 creditAccountCurrentBalance = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction , "creditAccountCurrentBalance")));
			 finalizedTime = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction , "finalizedTime")));
			 transactionStatus = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction , "transactionStatus")));
			 amount = cJSON_GetObjectItem(transaction, "ammount")->valueint;
			 // convert ammount to string [buf]
			 sprintf(ammount, "%d", amount);
			 strcpy(dates,get_date_from_string(finalizedTime));
			 surface->DrawString(surface,"--------------------------------------------------------", -1, rectangle_x, y, DSTF_TOPLEFT);
															y += height + 1;
				surface->DrawString(surface,"transId:", -1, rectangle_x, y, DSTF_TOPLEFT);
				surface->DrawString(surface, transId, -1,rectangle_x+170, y, DSTF_TOPLEFT);
				y += height + 1;

				surface->DrawString(surface, "dbtPartyName", -1, rectangle_x, y, DSTF_TOPLEFT);
				surface->DrawString(surface,debitPartyName, -1, rectangle_x+170, y, DSTF_TOPLEFT);
				y += height + 1;

				surface->DrawString(surface, "dbtAccBal: ", -1, rectangle_x, y, DSTF_TOPLEFT);
				surface->DrawString(surface,debitAccountBalance, -1, rectangle_x+170, y, DSTF_TOPLEFT);
				y += height + 1;

				surface->DrawString(surface,"crdAccBal:", -1, rectangle_x, y, DSTF_TOPLEFT);
				surface->DrawString(surface, creditAccountCurrentBalance, -1,rectangle_x+170, y, DSTF_TOPLEFT);
				y += height + 1;

				surface->DrawString(surface, "finalizedTime", -1, rectangle_x, y, DSTF_TOPLEFT);
				surface->DrawString(surface,dates, -1, rectangle_x+170, y, DSTF_TOPLEFT);
				y += height + 1;

				surface->DrawString(surface, "ammount", -1, rectangle_x, y, DSTF_TOPLEFT);
				surface->DrawString(surface,ammount, -1, rectangle_x+170, y, DSTF_TOPLEFT);
				y += height + 1;

		}



		surface->SetFont(surface, font_32);
		surface->DrawString(surface, "-----------------------------------", -1,x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		y += height -8;


		{

			char  tmp_str[100];
			surface->SetFont(surface, font_24);
			surface->DrawString(surface,"Thank you for shopping with us "  , -1, x, y, DSTF_TOPCENTER);
			font_24->GetHeight(font_24, &height);
			y += height + 1;
			sprintf (tmp_str , "Served by : %s" , myLoginPosUser->username  );



			surface->DrawString(surface,tmp_str, -1, x, y, DSTF_TOPCENTER);
			y += height + 1;


			y += height + 1;
			surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
			font_32->GetHeight(font_32, &height);
			surface->SetFont(surface, font_32);
			y += height - 8;
			y += height -8;
		}








	}

	if (0 == retval) {

		lcd_printf(ALG_LEFT, "Printing ....");
		lcd_flip();
		kb_hit();
		print_surface(ifd, load_image(dfb, logo), y);

		do {
			usleep(100000);
			printer_get_status(ifd, &status);
			if (((status.status >> PRINTER_STATUS_BUSY) & 0x01) == 0x01)
				//lcd_printf(ALG_LEFT, "printer status = busy");
				continue;
			else if (((status.status >> PRINTER_STATUS_HIGHT_TEMP) & 0x01)
					== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = tempreture high");
				lcd_printf(ALG_LEFT,
						"Insert and press enter to reprint or cancel to terminate");
				key = kb_getkey();

				//Printer_Demo();

				break;
			} else if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)== 0x01) {
				int wx= 1;
				while(wx)
				{
					printer_get_status(ifd, &status);
					if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)== 0x01)
					{
						lcd_clean();
						screen_header();
						lcd_printf(ALG_CENTER, "Printer Paper is Empty");
						lcd_printf(ALG_CENTER, "Insert and press enter \nto reprint");
						printflag = 1;
						lcd_flip();
						kb_getkey();
						reprint = 1;
						goto END;
					}
					else
						wx=0;
				}
				break;
			} else if (((status.status >> PRINTER_STATUS_FEED) & 0x01)
					== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = feed paper");
				break;
			} else if (((status.status >> PRINTER_STATUS_PRINT) & 0x01)== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = printing");
				break;
			} else if (((status.status >> PRINTER_STATUS_FORCE_FEED) & 0x01)== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = force feed paper");
				break;
			} else if (((status.status >> PRINTER_STATUS_POWER_ON) & 0x01)== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = power on");
				break;
			}
			lcd_flip();

		} while (status.status != 0);

		print_surface(ifd, surface, y);
		do {
			usleep(100000);
			printer_get_status(ifd, &status);
			if (((status.status >> PRINTER_STATUS_BUSY) & 0x01) == 0x01)
				//lcd_printf(ALG_LEFT, "printer status = busy");
				continue;
			else if (((status.status >> PRINTER_STATUS_HIGHT_TEMP) & 0x01)
					== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = tempreture high");
				break;
			} else if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)
					== 0x01) {
				int wx= 1;
				while(wx)
				{
					printer_get_status(ifd, &status);
					if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)
							== 0x01)
					{
						lcd_clean();
						screen_header();
						lcd_printf(ALG_CENTER, "Printer Paper is Empty");
						lcd_printf(ALG_CENTER, "Insert and press enter \nto reprint");
						printflag = 1;
						lcd_flip();
						kb_getkey();
						reprint = 1;
						goto END;
					}
					else
						wx=0;
				}
				break;

			} else if (((status.status >> PRINTER_STATUS_FEED) & 0x01)
					== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = feed paper");
				break;
			} else if (((status.status >> PRINTER_STATUS_PRINT) & 0x01)
					== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = printing");
				break;
			} else if (((status.status >> PRINTER_STATUS_FORCE_FEED) & 0x01)
					== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = force feed paper");
				break;
			} else if (((status.status >> PRINTER_STATUS_POWER_ON) & 0x01)
					== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = power on");
				break;
			}
			lcd_flip();

		} while (status.status != 0);
		/*		print_surface(ifd, load_image(dfb, filenames), y + 250);
		//lcd_printf(ALG_LEFT, "Printing ........");
		do {
			usleep(100000);
			printer_get_status(ifd, &status);
			if (((status.status >> PRINTER_STATUS_BUSY) & 0x01) == 0x01)
				//lcd_printf(ALG_LEFT, "printer status = busy");
				continue;
			else if (((status.status >> PRINTER_STATUS_HIGHT_TEMP) & 0x01)
					== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = tempreture high");
				break;
			} else if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)
					== 0x01) {
				int wx= 1;
				while(wx)
					{
					printer_get_status(ifd, &status);
				if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)
									== 0x01)
				{
					lcd_clean();
					screen_header();
					lcd_printf(ALG_CENTER, "Printer Paper is Empty");
					lcd_printf(ALG_CENTER, "Insert and press enter \nto reprint");
					printflag = 1;
					lcd_flip();
					kb_getkey();
					reprint = 1;
					goto  END;

				}
				else
				wx=0;
					}
				break;
			} else if (((status.status >> PRINTER_STATUS_FEED) & 0x01)
					== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = feed paper");
				break;
			} else if (((status.status >> PRINTER_STATUS_PRINT) & 0x01)
					== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = printing");
				break;
			} else if (((status.status >> PRINTER_STATUS_FORCE_FEED) & 0x01)
					== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = force feed paper");
				break;
			} else if (((status.status >> PRINTER_STATUS_POWER_ON) & 0x01)
					== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = power on");
				break;
			}
			lcd_flip();

		} while (status.status != 0);*/

		lcd_printf(ALG_LEFT, "Finished.");
		lcd_flip();
	}

	print_surface(ifd, surface1, 100);
	do {
		usleep(100000);
		printer_get_status(ifd, &status);
		if (((status.status >> PRINTER_STATUS_BUSY) & 0x01) == 0x01)
			//lcd_printf(ALG_LEFT, "printer status = busy");
			continue;
		else if (((status.status >> PRINTER_STATUS_HIGHT_TEMP) & 0x01)
				== 0x01) {
			lcd_printf(ALG_LEFT, "printer status = tempreture high");
			break;
		} else if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)
				== 0x01) {
			int wx= 1;
			while(wx)
			{
				printer_get_status(ifd, &status);
				if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)
						== 0x01)
				{
					lcd_clean();
					screen_header();
					lcd_printf(ALG_CENTER, "Printer Paper is Empty");
					lcd_printf(ALG_CENTER, "Insert and press enter \nto reprint");
					printflag = 1;
					lcd_flip();
					kb_getkey();
					reprint = 1;
					goto END;
				}
				else
					wx=0;
			}
			break;

		} else if (((status.status >> PRINTER_STATUS_FEED) & 0x01)
				== 0x01) {
			lcd_printf(ALG_LEFT, "printer status = feed paper");
			break;
		} else if (((status.status >> PRINTER_STATUS_PRINT) & 0x01)
				== 0x01) {
			lcd_printf(ALG_LEFT, "printer status = printing");
			break;
		} else if (((status.status >> PRINTER_STATUS_FORCE_FEED) & 0x01)
				== 0x01) {
			lcd_printf(ALG_LEFT, "printer status = force feed paper");
			break;
		} else if (((status.status >> PRINTER_STATUS_POWER_ON) & 0x01)
				== 0x01) {
			lcd_printf(ALG_LEFT, "printer status = power on");
			break;
		}
		lcd_flip();

	} while (status.status != 0);


	surface->SetFont(surface, font_24);
	surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
	font_24->GetHeight(font_24, &height);
	y += height + 1;

	if (NULL != surface)
		surface->Release(surface);

	if (NULL != surface1)
		surface1->Release(surface1);

	if (NULL != font_16)
		font_16->Release(font_16);

	if (NULL != font_24)
		font_24->Release(font_24);

	if (NULL != font_32)
		font_32->Release(font_32);

	if (ifd >= 0) {
		printer_close(ifd);
		ifd = -1;
	}
	END :
	if(reprint){
		//Printer_Demo(type_of_receipt);
		print_receipt(type_of_receipt ,  transaction ,  printflag , print_complete);
		print_complete=1;
	}
	//number_of_trans=0;
	/*	if(transactin_name){
		free(transactin_name);
	}*/
	//PrintHeader();

}


