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

int print_receipt(char * type_of_receipt , cJSON * transaction ,  cJSON * personal_data , int * printflag ,  int * print_complete) {
	int reprint = 0;
	int ifd = -1;
	int key = 0;
	int retval = 0;
	char Recipt[100];
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

/*
	printf("\n\n\n\n\nI started printing\n\n\n\n");
	printf("charge :%s", charge);
	printf("pmode :%s", pmode);
	printf("p_ref  :%s", payment_ref_code);
	printf("Service :%s", serviceNameSelected);*/

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

		surface->DrawString(surface, "SAFARICOM LIMITED", -1, x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 8;

		surface->DrawString(surface, "_________________________________*", -1,
				x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;


	/*	if(strcmp(type_of_receipt, "TX RECEIPT")==0)
			surface->DrawString(surface, "TRANSACTION RECEIPT", -1, x, y, DSTF_TOPCENTER);
		else*/

		surface->DrawString(surface, type_of_receipt, -1, x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 8;

		surface->DrawString(surface, "_________________________________", -1,
				x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;


		surface1->SetFont(surface1, font_32);
		surface1->DrawString(surface1, "", -1,x, y, DSTF_TOPCENTER);
		surface1->DrawString(surface1, "", -1,x, y, DSTF_TOPCENTER);
		surface1->DrawString(surface1, "", -1,x, y, DSTF_TOPCENTER);
		surface1->DrawString(surface1, "", -1,x, y, DSTF_TOPCENTER);
		//y += height + 1;


/*		strcat(rootServiceName, " Receipt");
		surface->SetFont(surface, font_32);
		surface->DrawString(surface, rootServiceName, -1, x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height ;*/

/*		surface->SetFont(surface, font_32);
		surface->DrawString(surface, "Gate", -1, 10, y,DSTF_TOPLEFT);
		surface->DrawString(surface, CurrentUser.market, -1, 100, y,DSTF_TOPLEFT);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 8;*/

/*		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height;*/

/*		surface->SetFont(surface, font_32);
		surface->DrawString(surface, serviceNameSelected, -1, x, y,DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;*/



		surface->SetFont(surface, font_24);
		font_24->GetHeight(font_24, &height);
		surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
		surface->DrawString(surface, "Beneficiary  ", -1, 5, y,DSTF_TOPLEFT);
		surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
		sprintf( myholder ,  "%s %s %s", get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(personal_data,"firstname"))) ,get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(personal_data,"lastname"))) , get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(personal_data,"middlename"))) );
		surface->DrawString(surface,myholder , -1, 140, y,DSTF_TOPLEFT);
		y += height + 1;
		surface->DrawString(surface, "Merchant ID ", -1, 5, y,DSTF_TOPLEFT);
		//surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
		surface->DrawString(surface, "Merchant ID", -1, 140, y,DSTF_TOPLEFT);
		y += height + 1;


		/*surface->DrawString(surface, "Subservice", -1, 5, y,DSTF_TOPLEFT);
				//surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
				surface->DrawString(surface, serviceNameSelected1, -1, 140, y,DSTF_TOPLEFT);
				y += height + 1;*/
		/*
				{
					"benTxn":	[{
							"amount":	"808",
							"transOperation":	"111",
							"debiticcid":	"8cg4382017583718",
							"crediticcid":	"Merchant_ID",
							"walletId":	"100",
							"transId":	"BT-82461843-181213000133",
							"terminalId":	"82461843",
							"date":	"2018-12-13 00:01:33",
							"authMode":	"101"
						}]
				}
		*/
		surface->DrawString(surface, "Wallet Name", -1, 5, y,DSTF_TOPLEFT);
		surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
		surface->DrawString(surface, get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction,"walletname"))), -1, 140, y,DSTF_TOPLEFT);
		y += height + 1;


		surface->DrawString(surface, "Wallet Bal", -1, 5, y,DSTF_TOPLEFT);
		surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
		surface->DrawString(surface, cJSON_Print(cJSON_GetObjectItem(transaction,"walletbalance")), -1, 140, y,DSTF_TOPLEFT);
		y += height + 1;
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height + 1;


		/*sprintf(Recipt,"Receipt No: %s",recptnum);
	    //surface->DrawString(surface,"Receipt No.", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, Recipt, -1, rectangle_x+150, y, DSTF_TOPLEFT);
*/

		surface->DrawString(surface,"Receipt No: ", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction,"transId"))), -1,rectangle_x+135, y, DSTF_TOPLEFT);
		y += height + 1;


		surface->DrawString(surface, "Amount ", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface,get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction,"Amount"))), -1, rectangle_x+135, y, DSTF_TOPLEFT);
		y += height + 1;

/*		if(strcmp(type_of_receipt,"VOIDED RECEIPT")==0 || strcmp(type_of_receipt,"REPRINTED RECEIPT")==0)
		{
			surface->DrawString(surface, "Orignal Date", -1, rectangle_x, y, DSTF_TOPLEFT);
			surface->DrawString(surface, voided_tx_time, -1, rectangle_x+135, y, DSTF_TOPLEFT);
			y += height + 1;

		}*/

		surface->DrawString(surface, "Time & Date", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, "Date Time ", -1, rectangle_x+135, y, DSTF_TOPLEFT);
		y += height + 1;

/*		if(number_of_trans > 0){
			surface->DrawString(surface, "-----------------------------------", -1,x, y, DSTF_TOPCENTER);
					y += height + 1;

					surface->SetFont(surface, font_32);
					surface->DrawString(surface, "TRANSACTION DETAILS", -1, x, y,DSTF_TOPCENTER);
					font_32->GetHeight(font_32, &height);
					y += height + 1;

					surface->DrawString(surface, "-----------------------------------", -1,x, y, DSTF_TOPCENTER);
					font_24->GetHeight(font_24, &height);
					surface->SetFont(surface, font_24);
					y += height + 1;

					for (numb = 0; numb <= number_of_trans - 1; numb++) {

						surface->DrawString(surface, transactin_name[numb], -1, rectangle_x + 30, y, DSTF_TOPLEFT);
						surface->DrawString(surface, transactin_val[numb] , -1, rectangle_x+260, y, DSTF_TOPLEFT);
						y += height + 1;
					}
//R
		}*/




		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 8;

		snprintf(myholder, 34, "Served By %s","Merchant Name" );
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, myholder , -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;


		surface->DrawString(surface, "Thank You, welcome again", -1, x, y, DSTF_TOPCENTER);
		y += height + 1;

		surface->SetFont(surface, font_32);
		surface->DrawString(surface, "-----------------------------------", -1,x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		y += height -8;

			y += height + 1;

		y += height -8;


/*
		surface->SetFont(surface, font_32);
		surface->DrawString(surface, "-----------------------------------", -1,x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		y += height -8;

		surface->SetFont(surface, font_24);
		font_24->GetHeight(font_24, &height);
		surface->DrawString(surface, "POWERED BY COMPULYNX", -1, x, y,DSTF_TOPCENTER);
		y += height+1;
		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
*/

		//strcpy(recptnum, "");



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
		 print_receipt(type_of_receipt ,  transaction ,  personal_data ,  printflag , print_complete);
		print_complete=1;
	}
	//number_of_trans=0;
/*	if(transactin_name){
		free(transactin_name);
	}*/
	//PrintHeader();

}
