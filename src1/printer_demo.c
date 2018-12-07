#include <posapi.h>
#include <printer.h>
#include <directfb.h>

#include "../src/utilities/lcd.h"
#include "../src/utilities/string_convert.h"
#define printer_device_name "/dev/printer0"
#define font_file_xx "/usr/share/fonts/arialnb.ttf"
#include "jsonread.h"
#include "receipt.h"
#include "main_old.h"
static IDirectFBSurface *load_image(IDirectFB *dfb, const char *filename);
void load_image_for_screen(IDirectFB *dfb, const char *filename ,IDirectFBSurface *main_surface );
void get_printstatus(int ifd, printer_status_t status);
void PrintHeader(void);

extern IDirectFBSurface *main_surface;
const char *filename = "logo1.png";
const char *filenames = "out.png";
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

char *replacer(char *str) {
	int len = strlen(str);
	int i, count = 0, k = 0;
	for (i = 0; i < len; i++)
		if (str[i] == ':')
			count++;

	char newstr[len + 2 * count];
	for (i = 0; i < len; i++) {
		if (str[i] == ':') {
			newstr[k++] = "";

		} else
			newstr[k++] = str[i];
	}
	newstr[k] = '\0';
	printf("before segfault %s\n", newstr);
	return newstr;
}
//void Printer_Fake_receipt(void) {
//
//	int ifd = -1;
//	int retval = 0;
//	printer_param_t param;
//	printer_status_t status;
//	DFBSurfaceDescription surfdesc;
//	IDirectFBSurface * surface = NULL;
//	IDirectFBFont *font_16 = NULL;
//	IDirectFBFont *font_24 = NULL;
//	IDirectFBFont *font_48 = NULL;
//	IDirectFBFont *font_32 = NULL;
//	DFBFontDescription fdesc;
//	//const char * disp_text = "HELLO WORLD";
//	int x, y, height;
//	int rectangle_x, rectangle_y, rectangle_width;
//	char date1[50];
//	char myholder[100];
//	//const char * utf8text = string_covert(disp_text, strlen(disp_text));
//
//	lcd_clean();
//	screen_header();
//	lcd_printf(ALG_LEFT, "Prepare for print...");
//	lcd_flip();
//	kb_hit();
//
//	memset(&fdesc, 0, sizeof(fdesc));
//	fdesc.flags = DFDESC_HEIGHT;
//
//	if (0 == retval) {
//		fdesc.height = 48;
//		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_48);
//		if (NULL == font_48) {
//			lcd_printf(ALG_LEFT, "craete font16 failed");
//			lcd_flip();
//			retval = -1;
//		}
//	}
//
//	fdesc.height = 16;
//	dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_16);
//	if (NULL == font_16) {
//		lcd_printf(ALG_LEFT, "craete font16 failed.");
//		lcd_flip();
//		retval = -1;
//	}
//
//	if (0 == retval) {
//		fdesc.height = 24;
//		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_24);
//		if (NULL == font_24) {
//			lcd_printf(ALG_LEFT, "craete font16 failed");
//			lcd_flip();
//			retval = -1;
//		}
//	}
//
//	if (0 == retval) {
//		fdesc.height = 32;
//		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_32);
//		if (NULL == font_32) {
//			lcd_printf(ALG_LEFT, "craete font32 failed");
//			lcd_flip();
//			retval = -1;
//		}
//	}
//
//	if (0 == retval) {
//		ifd = printer_open(printer_device_name, O_WRONLY | O_NONBLOCK);
//		if (ifd < 0) {
//			lcd_printf(ALG_LEFT, "Device Open failed.");
//			lcd_flip();
//		}
//	}
//
//	if (0 == retval) {
//
//		printer_get_param(ifd, &param);
//
//		surfdesc.flags = DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT;
//		surfdesc.caps = DSCAPS_NONE;
//		surfdesc.width = param.ro_width;
//		surfdesc.height = 4000;
//		dfb->CreateSurface(dfb, &surfdesc, &surface);
//		surface->Clear(surface, 0xFF, 0xFF, 0xFF, 0xFF);
//
//		surface->SetColor(surface, 0x00, 0x00, 0x00, 0xFF);
//
//		x = param.ro_width / 2;
//		y = 20;
//
//		rectangle_x = 5;
//		rectangle_y = y - 10;
//		rectangle_width = param.ro_width - 5 * 2,
//
//		//Header:	surface->SetFont(surface, font_48);
//
//		surface->DrawString(surface, "CARTE BANCAIRE EMV", -1, 5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//		surface->DrawString(surface, "CARTE BANCAIRE EMV", -1, 5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//				surface->SetFont(surface, font_24);
//				y += height + 1;
//
//		surface->DrawString(surface, "A0000000031010", -1, 5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//
//		surface->DrawString(surface, "VISA DEBIT", -1,5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//		surface->DrawString(surface, "LE 30/07/18 A 08:25:30", -1,
//				5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//		surface->DrawString(surface, "SHERATON HOTEL", -1,
//				5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//		surface->DrawString(surface, "00DJIBOUTI", -1,
//				5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//		surface->DrawString(surface, "4140012 00000004140012", -1,
//				5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//		surface->DrawString(surface, "10003", -1,
//				5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//		surface->DrawString(surface, "XXXXXXXXXXXX7941", -1, 5, y, DSTF_TOPLEFT);
//		surface->DrawString(surface, "Gate", -1, 5, y,DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//		surface->DrawString(surface, "6A1F46137C617022", -1,
//				5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//		surface->DrawString(surface, "002 000001  15  C @", -1,
//				5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//		surface->DrawString(surface, "MONTANT:", -1,
//				5, y, DSTF_TOPLEFT);
//		font_32->GetHeight(font_32, &height);
//		surface->SetFont(surface, font_32);
//		y += height + 1;
//		surface->DrawString(surface, "96158DJF", -1,
//				x, y, DSTF_TOPCENTER);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//		surface->DrawString(surface, "DEBIT", -1,
//				5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//		surface->DrawString(surface, "TICKET CLIENT", -1,
//				5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//		surface->DrawString(surface, "A CONSERVER", -1,
//				5, y, DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//
//		/*				rectangle_height = y - rectangle_y;
//		 surface->DrawRectangle(surface, rectangle_x, rectangle_y, rectangle_width, rectangle_height );*/
//
//		/*				surface->SetFont(surface, font_24);
//		 surface->DrawString(surface, "Logo Comes Here", -1, x, y, DSTF_TOPCENTER);
//		 font_24->GetHeight(font_24, &height);
//		 y+= height - 10;
//
//
//
//		 surface->DrawString(surface,"_________________________________", -1, x, y, DSTF_TOPCENTER);
//		 font_48->GetHeight(font_48, &height);
//		 surface->SetFont(surface, font_48);
//		 y+= height + 1;*/
//
//		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
//		font_32->GetHeight(font_32, &height);
//		surface->SetFont(surface, font_32);
//		y += height - 5;
//
//		/*				surface->SetFont(surface, font_24);
//		 surface->DrawString(surface," ", -1, x, y, DSTF_TOPCENTER);
//		 font_24->GetHeight(font_24, &height);
//		 surface->SetFont(surface, font_24);
//		 y+= height -5;*/
//
//		surface->SetFont(surface, font_24);
//		surface->DrawString(surface, "Gate", -1, 10, y,DSTF_TOPLEFT);
//		surface->DrawString(surface, CurrentUser.market, -1, 100, y,DSTF_TOPLEFT);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//
//		surface->SetFont(surface, font_24);
//		surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//
//		snprintf(myholder, 34, "Time & Date: %-19s.",rcpt_date);
//		surface->SetFont(surface, font_24);
//		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
//		font_24->GetHeight(font_24, &height);
//		y += height + 1;
//
//		sprintf(date1, "On :%s", rcpt_date);
//
//		surface->DrawString(surface, "-----------------------------------", -1,
//				x, y, DSTF_TOPCENTER);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//
//		surface->SetFont(surface, font_32);
//		surface->DrawString(surface, "VOID RECEIPT", -1, x, y, DSTF_TOPCENTER);
//		font_32->GetHeight(font_32, &height);
//		y += height + 1;
//
//		surface->DrawString(surface, "-----------------------------------", -1,
//				x, y, DSTF_TOPCENTER);
//		font_24->GetHeight(font_24, &height);
//		surface->SetFont(surface, font_24);
//		y += height + 1;
//
//		//sprintf(str,"%d",receipt_num);
//		surface->SetFont(surface, font_32);
//		snprintf(myholder, 34, "No : %s.", recptnum);
//		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
//		font_24->GetHeight(font_32, &height);
//		y += height + 1;
//
//		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
//		font_48->GetHeight(font_48, &height);
//		surface->SetFont(surface, font_48);
//		y += height - 8;
//
//
//		snprintf(myholder, 34, "Served By %s",CurrentUser.name);
//		surface->SetFont(surface, font_24);
//		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
//		font_24->GetHeight(font_24, &height);
//		y += height + 1;
//
//
//		surface->SetFont(surface, font_24);
//		surface->DrawString(surface, "Thank you, welcome again", -1, x, y, DSTF_TOPCENTER);
//		font_24->GetHeight(font_24, &height);
//		y += height + 1;
//
//
//		surface->SetFont(surface, font_32);
//		surface->DrawString(surface, "-----------------------------------", -1,x, y, DSTF_TOPCENTER);
//		font_32->GetHeight(font_32, &height);
//		y += height + 1;
//
//		surface->SetFont(surface, font_24);
//		surface->DrawString(surface, "POWERED BY COMPULYNX", -1, x, y,DSTF_TOPCENTER);
//		font_24->GetHeight(font_24, &height);
//		y += height + 1;
//
//	}
//
//	if (0 == retval) {
//
//		lcd_printf(ALG_LEFT, "Printing....");
//		lcd_flip();
//		kb_hit();
//		print_surface(ifd, load_image(dfb, filename), y);
//		//lcd_printf(ALG_LEFT, "Printing ........");
//		do {
//			usleep(100000);
//			printer_get_status(ifd, &status);
//			if (((status.status >> PRINTER_STATUS_BUSY) & 0x01) == 0x01)
//				//lcd_printf(ALG_LEFT, "printer status = busy");
//				continue;
//			else if (((status.status >> PRINTER_STATUS_HIGHT_TEMP) & 0x01)
//					== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = tempreture high");
//				break;
//			} else if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)
//					== 0x01) {
//				int wx= 1;
//				while(wx)
//					{
//					printer_get_status(ifd, &status);
//				if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)
//									== 0x01)
//				{
//					lcd_clean();
//					screen_header();
//					lcd_printf(ALG_CENTER, "Printer Paper is Empty");
//					lcd_printf(ALG_CENTER, "Insert and press enter \nto reprint");
//					lcd_flip();
//					kb_getkey();
//				}
//				else
//				wx=0;
//					}
//				//Printer_Void_receipt();
//				break;
//			} else if (((status.status >> PRINTER_STATUS_FEED) & 0x01)== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = feed paper");
//				break;
//			} else if (((status.status >> PRINTER_STATUS_PRINT) & 0x01)== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = printing");
//				break;
//			} else if (((status.status >> PRINTER_STATUS_FORCE_FEED) & 0x01)== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = force feed paper");
//				break;
//			} else if (((status.status >> PRINTER_STATUS_POWER_ON) & 0x01)
//					== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = power on");
//				break;
//			}
//			lcd_flip();
//
//		} while (status.status != 0);
//
//		print_surface(ifd, surface, y);
//		do {
//			usleep(100000);
//			printer_get_status(ifd, &status);
//			if (((status.status >> PRINTER_STATUS_BUSY) & 0x01) == 0x01)
//				//lcd_printf(ALG_LEFT, "printer status = busy");
//				continue;
//			else if (((status.status >> PRINTER_STATUS_HIGHT_TEMP) & 0x01)== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = tempreture high");
//				break;
//			} else if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)== 0x01) {
//				int wx= 1;
//				while(wx)
//					{
//					printer_get_status(ifd, &status);
//				if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)
//									== 0x01)
//				{
//					lcd_clean();
//					screen_header();
//					lcd_printf(ALG_CENTER, "Printer Paper is Empty");
//					lcd_printf(ALG_CENTER, "Insert and press enter \nto reprint");
//					lcd_flip();
//					kb_getkey();
//				}
//				else
//				wx=0;
//					}
//				//Printer_Void_receipt();
//				break;
//			} else if (((status.status >> PRINTER_STATUS_FEED) & 0x01)== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = feed paper");
//				break;
//			} else if (((status.status >> PRINTER_STATUS_PRINT) & 0x01)== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = printing");
//				break;
//			} else if (((status.status >> PRINTER_STATUS_FORCE_FEED) & 0x01)== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = force feed paper");
//				break;
//			} else if (((status.status >> PRINTER_STATUS_POWER_ON) & 0x01)== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = power on");
//				break;
//			}
//			lcd_flip();
//
//		} while (status.status != 0);
//		print_surface(ifd, load_image(dfb, filenames), y + 150);
//		lcd_printf(ALG_LEFT, "Printing ........");
//		do {
//			usleep(100000);
//			printer_get_status(ifd, &status);
//			if (((status.status >> PRINTER_STATUS_BUSY) & 0x01) == 0x01)
//				//lcd_printf(ALG_LEFT, "printer status = busy");
//				continue;
//			else if (((status.status >> PRINTER_STATUS_HIGHT_TEMP) & 0x01)
//					== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = tempreture high");
//				break;
//			} else if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)== 0x01) {
//				int wx= 1;
//				while(wx)
//					{
//					printer_get_status(ifd, &status);
//				if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)== 0x01)
//				{
//					lcd_clean();
//					screen_header();
//					lcd_printf(ALG_CENTER, "Printer Paper is Empty");
//					lcd_printf(ALG_CENTER, "Insert and press enter \nto reprint");
//					lcd_flip();
//					kb_getkey();
//				}
//				else
//				wx=0;
//					}
//				break;
//			} else if (((status.status >> PRINTER_STATUS_FEED) & 0x01)== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = feed paper");
//				break;
//			} else if (((status.status >> PRINTER_STATUS_PRINT) & 0x01)== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = printing");
//				break;
//			} else if (((status.status >> PRINTER_STATUS_FORCE_FEED) & 0x01)== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = force feed paper");
//				break;
//			} else if (((status.status >> PRINTER_STATUS_POWER_ON) & 0x01)== 0x01) {
//				lcd_printf(ALG_LEFT, "printer status = power on");
//				break;
//			}
//			lcd_flip();
//
//		} while (status.status != 0);
//
//		lcd_printf(ALG_LEFT, "Finished.");
//		lcd_flip();
//	}
//
///*	surface->SetFont(surface, font_24);
//	surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
//	font_24->GetHeight(font_24, &height);
//	y += height + 1;*/
//
//	if (NULL != surface)
//		surface->Release(surface);
///*	if (NULL != dfb)
//		dfb->Release(dfb);*/
//	if (NULL != font_16)
//		font_16->Release(font_16);
//
//	if (NULL != font_24)
//		font_24->Release(font_24);
//
//	if (NULL != font_32)
//		font_32->Release(font_32);
//	if (NULL != font_48)
//		font_48->Release(font_48);
//
//	if (ifd >= 0) {
//		printer_close(ifd);
//		ifd = -1;
//	}
//	//PrintHeader();
//
//}

void Printer_Void_receipt(void) {

	int ifd = -1;
	int retval = 0;
	printer_param_t param;
	printer_status_t status;
	DFBSurfaceDescription surfdesc;
	IDirectFBSurface * surface = NULL;
	IDirectFBFont *font_16 = NULL;
	IDirectFBFont *font_24 = NULL;
	IDirectFBFont *font_48 = NULL;
	IDirectFBFont *font_32 = NULL;
	DFBFontDescription fdesc;
	//const char * disp_text = "HELLO WORLD";
	int x, y, height;
	int rectangle_x, rectangle_y, rectangle_width;
	char date1[50];
	char myholder[100];
	//const char * utf8text = string_covert(disp_text, strlen(disp_text));

	lcd_clean();
	screen_header();
	lcd_printf(ALG_LEFT, "Prepare for print...");
	lcd_flip();
	kb_hit();

	memset(&fdesc, 0, sizeof(fdesc));
	fdesc.flags = DFDESC_HEIGHT;

	if (0 == retval) {
		fdesc.height = 48;
		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_48);
		if (NULL == font_48) {
			lcd_printf(ALG_LEFT, "craete font16 failed");
			lcd_flip();
			retval = -1;
		}
	}

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

		surface->DrawString(surface, "KARURA FOREST ", -1, x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;

		surface->DrawString(surface, "_________________________________*", -1,
				x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;

		/*				rectangle_height = y - rectangle_y;
		 surface->DrawRectangle(surface, rectangle_x, rectangle_y, rectangle_width, rectangle_height );*/

		/*				surface->SetFont(surface, font_24);
		 surface->DrawString(surface, "Logo Comes Here", -1, x, y, DSTF_TOPCENTER);
		 font_24->GetHeight(font_24, &height);
		 y+= height - 10;



		 surface->DrawString(surface,"_________________________________", -1, x, y, DSTF_TOPCENTER);
		 font_48->GetHeight(font_48, &height);
		 surface->SetFont(surface, font_48);
		 y+= height + 1;*/

		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 5;

		/*				surface->SetFont(surface, font_24);
		 surface->DrawString(surface," ", -1, x, y, DSTF_TOPCENTER);
		 font_24->GetHeight(font_24, &height);
		 surface->SetFont(surface, font_24);
		 y+= height -5;*/

		surface->SetFont(surface, font_24);
		surface->DrawString(surface, "Gate", -1, 10, y,DSTF_TOPLEFT);
		surface->DrawString(surface, CurrentUser.market, -1, 100, y,DSTF_TOPLEFT);
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height + 1;

		surface->SetFont(surface, font_24);
		surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height + 1;

		snprintf(myholder, 34, "Time & Date: %-19s.",rcpt_date);
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;

		sprintf(date1, "On :%s", rcpt_date);

		surface->DrawString(surface, "-----------------------------------", -1,
				x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height + 1;

		surface->SetFont(surface, font_32);
		surface->DrawString(surface, "VOID RECEIPT", -1, x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		y += height + 1;

		surface->DrawString(surface, "-----------------------------------", -1,
				x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height + 1;

		//sprintf(str,"%d",receipt_num);
		surface->SetFont(surface, font_32);
		snprintf(myholder, 34, "No : %s.", recptnum);
		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_32, &height);
		y += height + 1;

		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
		font_48->GetHeight(font_48, &height);
		surface->SetFont(surface, font_48);
		y += height - 8;


		snprintf(myholder, 34, "Served By %s",CurrentUser.name);
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;


		surface->SetFont(surface, font_24);
		surface->DrawString(surface, "Thank you, welcome again", -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;


		surface->SetFont(surface, font_32);
		surface->DrawString(surface, "-----------------------------------", -1,x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		y += height + 1;

		surface->SetFont(surface, font_24);
		surface->DrawString(surface, "POWERED BY COMPULYNX", -1, x, y,DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;

	}

	if (0 == retval) {

		lcd_printf(ALG_LEFT, "Printing....");
		lcd_flip();
		kb_hit();
		print_surface(ifd, load_image(dfb, filename), y);
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
					lcd_flip();
					kb_getkey();
				}
				else
				wx=0;
					}
				//Printer_Void_receipt();
				break;
			} else if (((status.status >> PRINTER_STATUS_FEED) & 0x01)== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = feed paper");
				break;
			} else if (((status.status >> PRINTER_STATUS_PRINT) & 0x01)== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = printing");
				break;
			} else if (((status.status >> PRINTER_STATUS_FORCE_FEED) & 0x01)== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = force feed paper");
				break;
			} else if (((status.status >> PRINTER_STATUS_POWER_ON) & 0x01)
					== 0x01) {
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
			else if (((status.status >> PRINTER_STATUS_HIGHT_TEMP) & 0x01)== 0x01) {
				lcd_printf(ALG_LEFT, "printer status = tempreture high");
				break;
			} else if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01)== 0x01) {
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
					lcd_flip();
					kb_getkey();
				}
				else
				wx=0;
					}
				//Printer_Void_receipt();
				break;
			} else if (((status.status >> PRINTER_STATUS_FEED) & 0x01)== 0x01) {
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
		print_surface(ifd, load_image(dfb, filenames), y + 150);
		lcd_printf(ALG_LEFT, "Printing ........");
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
					lcd_flip();
					kb_getkey();
				}
				else
				wx=0;
					}
				break;
			} else if (((status.status >> PRINTER_STATUS_FEED) & 0x01)== 0x01) {
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

		lcd_printf(ALG_LEFT, "Finished.");
		lcd_flip();
	}

/*	surface->SetFont(surface, font_24);
	surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
	font_24->GetHeight(font_24, &height);
	y += height + 1;*/

	if (NULL != surface)
		surface->Release(surface);
/*	if (NULL != dfb)
		dfb->Release(dfb);*/
	if (NULL != font_16)
		font_16->Release(font_16);

	if (NULL != font_24)
		font_24->Release(font_24);

	if (NULL != font_32)
		font_32->Release(font_32);
	if (NULL != font_48)
		font_48->Release(font_48);

	if (ifd >= 0) {
		printer_close(ifd);
		ifd = -1;
	}
	//PrintHeader();

}








//Printer_Demo

void SBP_printer(char * type_of_receipt) {

	int reprint = 0;
	int ifd = -1;
	int key = 0;
	int retval = 0;
	char Recipt[100];
	printer_param_t param;
	printer_status_t status;
	DFBSurfaceDescription surfdesc;
	IDirectFBSurface * surface = NULL;
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

		surface->DrawString(surface, "KARURA FOREST", -1, x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 8;

		surface->DrawString(surface, "_________________________________*", -1,
				x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;

		if(strcmp(type_of_receipt,"VOIDED RECEIPT")==0)
		surface->DrawString(surface, "VOIDED RECEIPT", -1, x, y, DSTF_TOPCENTER);
		else
		surface->DrawString(surface, "ORIGINAL RECEIPT", -1, x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 8;

		surface->DrawString(surface, "_________________________________", -1,
				x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;



		//strcat(rootServiceName, " Receipt");
		surface->SetFont(surface, font_32);
		surface->DrawString(surface, "SBP Receipt", -1, x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height ;


		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height;



		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height + 1;

		//sprintf(Recipt,"Receipt No: %s",recptnum);
		surface->DrawString(surface,"Receipt No:", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, recptnum, -1, rectangle_x+150, y, DSTF_TOPLEFT);
		y += height + 1;

		surface->DrawString(surface, "Pay Mode", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, pmode, -1, rectangle_x +150, y, DSTF_TOPLEFT);
		y += height + 1;

		if(strcmp(pmode,"Cash") != 0)
		{
			surface->DrawString(surface, "Payment Ref", -1, rectangle_x, y, DSTF_TOPLEFT);
			surface->DrawString(surface, payment_ref_code, -1, rectangle_x+150, y, DSTF_TOPLEFT);
			y += height + 1;
		}

		surface->DrawString(surface, "Amount Paid :", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, chargesbp, -1, rectangle_x+150, y , DSTF_TOPLEFT);
		y += height + 1;


		if(strcmp(type_of_receipt,"VOIDED RECEIPT")==0)
		{
			surface->DrawString(surface, "Orignal Date", -1, rectangle_x, y, DSTF_TOPLEFT);
			surface->DrawString(surface, voided_tx_time, -1, rectangle_x + 150, y, DSTF_TOPLEFT);
			y += height + 1;
		}

		surface->DrawString(surface, "Time & Date", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, getdate_mine(), -1, rectangle_x+150, y, DSTF_TOPLEFT);
		y += height + 1;

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

/*		for (numb = 0; numb <= number_of_trans - 1; numb++) {

			surface->DrawString(surface, transactin_name[numb], -1, rectangle_x + 30, y, DSTF_TOPLEFT);
			surface->DrawString(surface, transactin_val[numb] , -1, rectangle_x+260, y, DSTF_TOPLEFT);
			y += height + 1;
		}*/

/*		strcpy(response, "");
		strcpy(bname, "");
		strcpy(bid, "");
		strcpy(bcode, "");
		strcpy(bdep, "");*/

		surface->DrawString(surface, "Name:", -1, rectangle_x + 30, y, DSTF_TOPLEFT);
		surface->DrawString(surface, bname , -1, rectangle_x+100, y, DSTF_TOPLEFT);
		y += height + 1;

		surface->DrawString(surface, "Id", -1, rectangle_x + 30, y, DSTF_TOPLEFT);
		surface->DrawString(surface, bid , -1, rectangle_x+100, y, DSTF_TOPLEFT);
		y += height + 1;

		surface->DrawString(surface, "Code", -1, rectangle_x + 30, y, DSTF_TOPLEFT);
		surface->DrawString(surface, bcode , -1, rectangle_x+100, y, DSTF_TOPLEFT);
		y += height + 1;

		surface->DrawString(surface, "Desc", -1, rectangle_x + 30, y, DSTF_TOPLEFT);
		surface->DrawString(surface, bdep , -1, rectangle_x+100, y, DSTF_TOPLEFT);
		y += height + 1;

		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 8;

		snprintf(myholder, 34, "Served By %s",CurrentUser.name );
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

		surface->SetFont(surface, font_24);
		font_24->GetHeight(font_24, &height);
		surface->DrawString(surface, "POWERED BY COMPULYNX", -1, x, y,DSTF_TOPCENTER);
		y += height+1;
		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);

	}

	if (0 == retval) {

		lcd_flip();
		kb_hit();
		print_surface(ifd, load_image(dfb, filename), y);
		lcd_printf(ALG_LEFT, "Printing ........");
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
		print_surface(ifd, load_image(dfb, filenames), y + 150);
		lcd_printf(ALG_LEFT, "Printing ........");
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

		} while (status.status != 0);

		lcd_printf(ALG_LEFT, "Finished.");
		lcd_flip();
	}


	//dfb->CreateSurface(dfb, &surfdesc, &surface);
	surface->Clear(surface, 0xFF, 0xFF, 0xFF, 0xFF);

	surface->SetColor(surface, 0x00, 0x00, 0x00, 0xFF);
	surface->SetFont(surface, font_24);
	surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
	surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
	surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
	font_24->GetHeight(font_24, &height);
	y += height + 20;

	if (NULL != surface)
		surface->Release(surface);

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
		SBP_printer(type_of_receipt);
	}
	strcpy(bname, "");
	strcpy(bid, "");
	strcpy(bcode, "");
	strcpy(bdep, "");
	//PrintHeader();

}












void Printer_Demo(char * type_of_receipt) {

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


	printf("\n\n\n\n\nI started printing\n\n\n\n");
	printf("charge :%s", charge);
	printf("pmode :%s", pmode);
	printf("p_ref  :%s", payment_ref_code);
	printf("Service :%s", serviceNameSelected);

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

		surface->DrawString(surface, "KARURA FOREST", -1, x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 8;

		surface->DrawString(surface, "_________________________________*", -1,
				x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;


		if(strcmp(type_of_receipt, "TX RECEIPT")==0)
			surface->DrawString(surface, "TRANSACTION RECEIPT", -1, x, y, DSTF_TOPCENTER);
		else

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
		surface->DrawString(surface, "Gate", -1, 5, y,DSTF_TOPLEFT);
	//	surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
		surface->DrawString(surface, CurrentUser.market, -1, 140, y,DSTF_TOPLEFT);
		y += height + 1;
		surface->DrawString(surface, "Service ", -1, 5, y,DSTF_TOPLEFT);
		//surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
		surface->DrawString(surface, rootServiceName, -1, 140, y,DSTF_TOPLEFT);
		y += height + 1;
		/*surface->DrawString(surface, "Subservice", -1, 5, y,DSTF_TOPLEFT);
				//surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
				surface->DrawString(surface, serviceNameSelected1, -1, 140, y,DSTF_TOPLEFT);
				y += height + 1;*/
		surface->DrawString(surface, "Subservice", -1, 5, y,DSTF_TOPLEFT);
	//	surface->DrawString(surface, ":", -1, 154, y,DSTF_TOPLEFT);
		surface->DrawString(surface, serviceNameSelected, -1, 140, y,DSTF_TOPLEFT);
		y += height + 1;
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height + 1;


		/*sprintf(Recipt,"Receipt No: %s",recptnum);
	    //surface->DrawString(surface,"Receipt No.", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, Recipt, -1, rectangle_x+150, y, DSTF_TOPLEFT);
*/

		surface->DrawString(surface,"Receipt No: ", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, recptnum, -1,rectangle_x+135, y, DSTF_TOPLEFT);
		y += height + 1;

		surface->DrawString(surface, "Pay Mode", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, pmode, -1, rectangle_x+135, y, DSTF_TOPLEFT);
		y += height + 1;

		if(strcmp(pmode,"Cash") != 0)
		{
			surface->DrawString(surface, "Payment Ref", -1, rectangle_x, y, DSTF_TOPLEFT);
			surface->DrawString(surface, payment_ref_code, -1, rectangle_x+135, y, DSTF_TOPLEFT);
			y += height + 1;
		}

		surface->DrawString(surface, "Amount Paid", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, charge, -1, rectangle_x+135, y, DSTF_TOPLEFT);
		y += height + 1;

		if(strcmp(type_of_receipt,"VOIDED RECEIPT")==0 || strcmp(type_of_receipt,"REPRINTED RECEIPT")==0)
		{
			surface->DrawString(surface, "Orignal Date", -1, rectangle_x, y, DSTF_TOPLEFT);
			surface->DrawString(surface, voided_tx_time, -1, rectangle_x+135, y, DSTF_TOPLEFT);
			y += height + 1;

		}

		surface->DrawString(surface, "Time & Date", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, getdate_mine(), -1, rectangle_x+135, y, DSTF_TOPLEFT);
		y += height + 1;

		if(number_of_trans > 0){
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
		}




		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 8;

		snprintf(myholder, 34, "Served By %s",CurrentUser.name );
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

		surface->SetFont(surface, font_24);
		font_24->GetHeight(font_24, &height);
		surface->DrawString(surface, "The kenya Forest service and the ", -1,5, y, DSTF_TOPLEFT);
		y += height + 1;
		surface->DrawString(surface, "Friends of Karura Forest do not accept", -1,5, y, DSTF_TOPLEFT);
		y += height + 1;
		surface->DrawString(surface, "liability for any accidents, injuries or", -1,5, y, DSTF_TOPLEFT);
		y += height + 1;
		surface->DrawString(surface, "other cost while visiting Karura Forest.", -1,5, y, DSTF_TOPLEFT);
		y += height + 1;
		surface->DrawString(surface, "", -1,5, y, DSTF_TOPLEFT);
		y += height + 1;
		surface->DrawString(surface, "Ticket is valid from 6:00AM to 6:00 PM", -1,5, y, DSTF_TOPLEFT);
		y += height + 1;

		y += height -8;


		surface->SetFont(surface, font_32);
		surface->DrawString(surface, "-----------------------------------", -1,x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		y += height -8;

		surface->SetFont(surface, font_24);
		font_24->GetHeight(font_24, &height);
		surface->DrawString(surface, "POWERED BY COMPULYNX", -1, x, y,DSTF_TOPCENTER);
		y += height+1;
		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);

		strcpy(recptnum, "");



	}

	if (0 == retval) {

		lcd_printf(ALG_LEFT, "Printing ....");
		lcd_flip();
		kb_hit();
		print_surface(ifd, load_image(dfb, filename), y);

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
		print_surface(ifd, load_image(dfb, filenames), y + 250);
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

		} while (status.status != 0);

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
		Printer_Demo(type_of_receipt);
		print_complete=1;
	}
	number_of_trans=0;
/*	if(transactin_name){
		free(transactin_name);
	}*/
	//PrintHeader();

}

void print_x_z_report(char * report_type) {

	int reprint= 0;
	int ifd = -1;
	int retval = 0;
	printer_param_t param;
	printer_status_t status;
	DFBSurfaceDescription surfdesc;
	IDirectFBSurface * surface = NULL;
	IDirectFBFont *font_16 = NULL;
	IDirectFBFont *font_24 = NULL;
	IDirectFBFont *font_32 = NULL;
	DFBFontDescription fdesc;
	//const char * disp_text = "HELLO WORLD";
	int x, y, height;
	int rectangle_x, rectangle_y, rectangle_width;

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
			lcd_printf(ALG_LEFT, "craeteVehicle Reg font16 failed");
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
		surfdesc.height = 8000;
		dfb->CreateSurface(dfb, &surfdesc, &surface);
		surface->Clear(surface, 0xFF, 0xFF, 0xFF, 0xFF);

		surface->SetColor(surface, 0x00, 0x00, 0x00, 0xFF);

		x = param.ro_width / 2;
		y = 20;

		rectangle_x = 5;
		rectangle_y = y - 10;
		rectangle_width = param.ro_width - 5 * 2,
				//printf("Param WIDTH : %d \n\n\n\n",rectangle_width);

		//Header:	surface->SetFont(surface, font_48);

		printf("Into\n");
		surface->DrawString(surface, "_________________________________*", -1,
				x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;

		surface->DrawString(surface, "KARURA FOREST", -1, x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;

		surface->DrawString(surface, "_________________________________*", -1,
				x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;

		surface->SetFont(surface, font_24);
		if(strcmp(report_type,"X")==0)
		surface->DrawString(surface, "    X-REPORT   ", -1, x, y,DSTF_TOPCENTER);
		else
			surface->DrawString(surface, "  Z-REPORT   ", -1, x, y,DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height - 5;

		surface->DrawString(surface, "_________________________________", -1, x,
				y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface,font_32);
		y += height - 5;

		surface->SetFont(surface, font_24);
		surface->DrawString(surface, " ", -1, rectangle_x+110, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height - 5;

		surface->SetFont(surface, font_24);
		surface->DrawString(surface, "Gate", -1, 10, y,DSTF_TOPLEFT);
		surface->DrawString(surface, CurrentUser.market, -1, 155, y,DSTF_TOPLEFT);
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height + 1;



		surface->DrawString(surface, "Name", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, CurrentUser.name, -1, rectangle_x + 150, y, DSTF_TOPLEFT);
		y += height + 1;

		surface->DrawString(surface, "Serial No.", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, mac_address, -1,  rectangle_x + 150, y,DSTF_TOPLEFT);
		y += height + 1;

		if(strcmp(report_type,"Z")==0){
		//sprintf(myholder,  "%d",z_number_recieved);
		surface->DrawString(surface, "Z_Number", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, z_date, -1,  rectangle_x + 150, y, DSTF_TOPLEFT);
		y += height + 1;
		}


		strcpy(myholder,getdate_mine());
		surface->DrawString(surface, "Time & Date", -1, rectangle_x, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 150, y, DSTF_TOPLEFT);
		y += height + 1;

		surface->DrawString(surface, "---------------------------------------------", -1,x, y, DSTF_TOPCENTER);
		y += height + 1;

		surface->SetFont(surface, font_32);
		surface->DrawString(surface, "TRANSACTION DETAILS", -1, x, y,DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		y += height + 1;

		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, "---------------------------------------------", -1,x, y, DSTF_TOPCENTER);
		y += height + 1;


		surface->DrawString(surface, "", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		y += height + 1;
		surface->DrawString(surface, "Daily Collections :", -1, rectangle_x+10, y, DSTF_TOPLEFT);
		y += height + 1;
		surface->DrawString(surface, "", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		y += height + 1;

		sprintf(myholder,  ": %d",x_report_total_txt_number);
		surface->DrawString(surface, "Total Online Txs", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		sprintf(myholder,  ": %d",x_report_total_txt_amount);
		surface->DrawString(surface, "Total Online Amount", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		sprintf(myholder,  ": %d",x_report_total_txt_number_offline);
		surface->DrawString(surface, "Total Offline Txs", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		sprintf(myholder,  ": %d",x_report_total_txt_amount_offline);
		surface->DrawString(surface, "Total Offline Amount", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		///
		sprintf(myholder,  ": %d",(x_report_total_txt_number_offline+ x_report_total_txt_number));
		surface->DrawString(surface, "Total Transactions", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		sprintf(myholder,  ": %d",(x_report_total_txt_amount_offline+x_report_total_txt_amount));
		surface->DrawString(surface, "Total Amount", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		///
		sprintf(myholder,  ": %d",x_report_total_txt_number_voided);
		surface->DrawString(surface, "Total Voids No.", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;
		sprintf(myholder,  ": %d",x_report_total_txt_amount_voided);
		surface->DrawString(surface, "Total Void Amount", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		sprintf(myholder,  ": %d",x_report_total_num_invalid);
		surface->DrawString(surface, "Total Invalid No", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		sprintf(myholder,  ": %d",x_report_total_val_invalid);
		surface->DrawString(surface, "Total Invalid Amnt", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;


		sprintf(myholder,  ": %d",x_report_total_txt_number + x_report_total_txt_number_offline- x_report_total_txt_number_voided-x_report_total_num_invalid);
		surface->DrawString(surface, "Net Transaction", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		sprintf(myholder,  ": %d",x_report_total_txt_amount + x_report_total_txt_amount_offline - x_report_total_txt_amount_voided-x_report_total_val_invalid);
		surface->DrawString(surface, "Net Amount Tx", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		//SBP
	/*	surface->DrawString(surface, "", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		y += height + 1;
		surface->DrawString(surface, "SBP Collections:", -1, rectangle_x+10, y, DSTF_TOPLEFT);
		y += height + 1;
		surface->DrawString(surface, "", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		y += height + 1;

*/


/*

		sprintf(myholder,  ": %d",x_report_total_num_sbp);
		surface->DrawString(surface, "Total Transactions", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		sprintf(myholder,  ": %d",x_report_total_val_sbp);
		surface->DrawString(surface, "Total Amount", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;



		sprintf(myholder,  ": %d",x_report_total_num_invalid_sbp);
		surface->DrawString(surface, "Total Invalid No", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		sprintf(myholder,  ": %d",x_report_total_val_invalid_sbp);
		surface->DrawString(surface, "Total Invalid Amnt", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		sprintf(myholder,  ": %d",x_report_total_num_sbp - x_report_total_num_invalid_sbp);
		surface->DrawString(surface, "Net Transaction", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

		sprintf(myholder,  ": %d",x_report_total_val_sbp - x_report_total_val_invalid_sbp);
		surface->DrawString(surface, "Net Amount SBP", -1, rectangle_x+40, y, DSTF_TOPLEFT);
		surface->DrawString(surface, myholder, -1,  rectangle_x + 240, y, DSTF_TOPLEFT);
		y += height + 1;

*/




	if(strcmp(report_type,"Z")==0)
		{
			font_32->GetHeight(font_32, &height);
			surface->SetFont(surface, font_32);
			surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
			y += height +1;
			surface->DrawString(surface, "Z Report Verification", -1, x,y, DSTF_TOPCENTER);
			y += height - 12;
			surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
			y += height - 2;
			font_24->GetHeight(font_24, &height);
			surface->SetFont(surface, font_24);
			surface->DrawString(surface, "Admin Name", -1, 5, y, DSTF_TOPLEFT);
			surface->DrawString(surface, ": _______________________", -1, 150, y, DSTF_TOPLEFT);
			y += height +1 ;
			surface->DrawString(surface, "Admin Sign ", -1, 5, y, DSTF_TOPLEFT);
			surface->DrawString(surface, ": _______________________", -1, 150, y, DSTF_TOPLEFT);
			y += height + 1;
			surface->DrawString(surface, "Agent Name ", -1, 5, y, DSTF_TOPLEFT);
			surface->DrawString(surface, ":", -1, 150, y, DSTF_TOPLEFT);
			surface->DrawString(surface, CurrentUser.name, -1,  160, y, DSTF_TOPLEFT);
			y += height + 1;
			surface->DrawString(surface, "Agent Sign ", -1, 5, y, DSTF_TOPLEFT);
			surface->DrawString(surface, ": _______________________", -1, 150, y, DSTF_TOPLEFT);
			y += height +1 ;
		}



		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 8;


		surface->SetFont(surface, font_24);
		font_24->GetHeight(font_24, &height);
		surface->DrawString(surface, "POWERED BY COMPULYNX", -1, x, y,DSTF_TOPCENTER);
		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
		y += height + 90;

	}
	printf("Line 712\n");

	if (0 == retval) {

		lcd_printf(ALG_LEFT, "Printing....");
		lcd_flip();
		kb_hit();
		print_surface(ifd, load_image(dfb, filename), y);
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

					lcd_flip();
					reprint = 1;
					kb_getkey();
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
					lcd_flip();
					reprint = 1;
					kb_getkey();

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

		lcd_printf(ALG_LEFT, "Finished.");
		lcd_flip();
	}

	surface->SetFont(surface, font_32);
	surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
	font_32->GetHeight(font_32, &height);
	y += height + 20;
/*	if (NULL != dfb)
		dfb->Release(dfb);*/
	if (NULL != surface)
		surface->Release(surface);
/*
	if (NULL != dfb)
		dfb->Release(dfb);*/

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
	END:
	 	 if(reprint){
	 		print_x_z_report(report_type);
	 	 }
	//PrintHeader();

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

void PrintHeader(void) {

	int ifd = -1;
	int retval = 0;
	printer_param_t param;
	printer_status_t status;
	DFBSurfaceDescription surfdesc;
	IDirectFBSurface * surface = NULL;
	IDirectFBFont *font_48 = NULL;
	DFBFontDescription fdesc;
	int x, y, height;
	int rectangle_x, rectangle_y, rectangle_width;


	//const char * utf8text = string_covert(disp_text, strlen(disp_text));

	lcd_clean();
	lcd_printf(ALG_LEFT, "Prepare for print...");
	lcd_flip();
	kb_hit();

	memset(&fdesc, 0, sizeof(fdesc));
	fdesc.flags = DFDESC_HEIGHT;

	if (0 == retval) {
		fdesc.height = 48;
		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_48);
		if (NULL == font_48) {
			lcd_printf(ALG_LEFT, "craete font16 failed");
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
		surfdesc.height = 1000;
		dfb->CreateSurface(dfb, &surfdesc, &surface);
		surface->Clear(surface, 0xFF, 0xFF, 0xFF, 0xFF);

		surface->SetColor(surface, 0x00, 0x00, 0x00, 0xFF);

		x = param.ro_width / 2;
		y = 20;

		rectangle_x = 5;
		rectangle_y = y - 10;
		rectangle_width = param.ro_width - 5 * 2,

		surface->SetFont(surface, font_48);
		surface->DrawString(surface, "KARURA FOREST", -1, x, y, DSTF_TOPCENTER);
		font_48->GetHeight(font_48, &height);

		surface->SetFont(surface, font_48);
		surface->DrawString(surface, "_________________________________*", -1,
				x, y, DSTF_TOPCENTER);
		font_48->GetHeight(font_48, &height);
		y += height + 1;
	}

	if (0 == retval) {

		lcd_printf(ALG_LEFT, "Printing....");
		lcd_flip();
		kb_hit();
		print_surface(ifd, surface, y + 150);
		lcd_printf(ALG_LEFT, "Printing ........");
		do {
			get_printstatus(ifd, status);

		} while (status.status != 0);
		print_surface(ifd, load_image(dfb, filename), y + 150);
		lcd_printf(ALG_LEFT, "Printing ........");
		do {
			get_printstatus(ifd, status);

		} while (status.status != 0);

		lcd_flip();
	}
	lcd_printf(ALG_LEFT, "Prepare for print...");
	lcd_flip();
	kb_hit();

	memset(&fdesc, 0, sizeof(fdesc));
	fdesc.flags = DFDESC_HEIGHT;

	if (0 == retval) {
		fdesc.height = 48;
		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_48);
		if (NULL == font_48) {
			lcd_printf(ALG_LEFT, "craete font16 failed");
			lcd_flip();
			retval = -1;
		}
	}
	surfdesc.flags = DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT;
	surfdesc.caps = DSCAPS_NONE;
	surfdesc.width = param.ro_width;
	surfdesc.height = 1000;
	dfb->CreateSurface(dfb, &surfdesc, &surface);
	surface->Clear(surface, 0xFF, 0xFF, 0xFF, 0xFF);

	surface->SetColor(surface, 0x00, 0x00, 0x00, 0xFF);

	x = param.ro_width / 2;
	y = 20;

	rectangle_x = 5;
	rectangle_y = y - 10;
	rectangle_width = param.ro_width - 5 * 2, surface->SetFont(surface, font_48);
	surface->DrawString(surface, "_________________________________*", -1, x, y,
			DSTF_TOPCENTER);
	font_48->GetHeight(font_48, &height);

	surface->SetFont(surface, font_48);
	surface->DrawString(surface, "Hello Transaction*", -1, x, y,
			DSTF_TOPCENTER);
	font_48->GetHeight(font_48, &height);

	surface->SetFont(surface, font_48);
	surface->DrawString(surface, "_________________________________*", -1, x, y,
			DSTF_TOPCENTER);
	font_48->GetHeight(font_48, &height);

	if (0 == retval) {

		lcd_printf(ALG_LEFT, "Printing....");
		lcd_flip();
		kb_hit();
		print_surface(ifd, surface, y + 150);
		lcd_printf(ALG_LEFT, "Printing ........");
		do {
			get_printstatus(ifd, status);

		} while (status.status != 0);

	}

	y += height + 1;
	if (NULL != surface)
		surface->Release(surface);
	if (NULL != dfb)
		dfb->Release(dfb);
	if (NULL != font_48)
		font_48->Release(font_48);

	if (ifd >= 0) {
		printer_close(ifd);
		ifd = -1;
	}
}

void get_printstatus(int ifd, printer_status_t status) {

	usleep(100000);

	printer_get_status(ifd, &status);
	if (((status.status >> PRINTER_STATUS_BUSY) & 0x01) == 0x01)
		lcd_printf(ALG_LEFT, "printer status = busy");

	else if (((status.status >> PRINTER_STATUS_HIGHT_TEMP) & 0x01) == 0x01) {
		lcd_printf(ALG_LEFT, "printer status = tempreture high");
	} else if (((status.status >> PRINTER_STATUS_PAPER_LACK) & 0x01) == 0x01) {
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
			lcd_flip();
			kb_getkey();
		}
		else
		wx=0;
			}
	} else if (((status.status >> PRINTER_STATUS_FEED) & 0x01) == 0x01) {
		lcd_printf(ALG_LEFT, "printer status = feed paper");
	} else if (((status.status >> PRINTER_STATUS_PRINT) & 0x01) == 0x01) {
		lcd_printf(ALG_LEFT, "printer status = printing");
	} else if (((status.status >> PRINTER_STATUS_FORCE_FEED) & 0x01) == 0x01) {
		lcd_printf(ALG_LEFT, "printer status = force feed paper");
	} else if (((status.status >> PRINTER_STATUS_POWER_ON) & 0x01) == 0x01) {
		lcd_printf(ALG_LEFT, "printer status = power on");
	}
	lcd_flip();
}



void print_z_report(void) {

	int ifd = -1;
	int retval = 0;
	printer_param_t param;
	printer_status_t status;
	DFBSurfaceDescription surfdesc;
	IDirectFBSurface * surface = NULL;
	IDirectFBFont *font_16 = NULL;
	IDirectFBFont *font_24 = NULL;
	IDirectFBFont *font_32 = NULL;
	DFBFontDescription fdesc;
	//const char * disp_text = "HELLO WORLD";
	int x, y, height;
	int rectangle_x, rectangle_y, rectangle_width;
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

		x = param.ro_width / 2;
		y = 20;

		rectangle_x = 5;
		rectangle_y = y - 10;
		rectangle_width = param.ro_width - 5 * 2,

		//Header:	surface->SetFont(surface, font_48);

		printf("Into\n");
		surface->DrawString(surface, "_________________________________*", -1,
				x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;

		surface->DrawString(surface,"KARURA FOREST", -1, x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;

		surface->DrawString(surface, "_________________________________*", -1,
				x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height + 1;

		surface->SetFont(surface, font_24);
		surface->DrawString(surface, "**  Z-REPORT  **", -1, x, y,
				DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height - 5;

		surface->DrawString(surface, "_________________________________", -1, x,
				y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 5;

		surface->SetFont(surface, font_24);
		surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height - 5;

		surface->SetFont(surface, font_24);
		surface->DrawString(surface, CurrentUser.market, -1, x, y,
				DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height + 1;

		surface->SetFont(surface, font_24);
		surface->DrawString(surface, "Serial No.", -1, x, y, DSTF_TOPRIGHT);
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		//y += height + 1;

		surface->SetFont(surface, font_24);
		snprintf(myholder, 34, ": %s", mac_address);
		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPRIGHT);
		font_24->GetHeight(font_24, &height);
		y += height + 1;

		//sprintf(str, "%d", receipt_num);
		surface->SetFont(surface, font_24);
		//snprintf(myholder, 34, "MAC ID     : %s", mac_address);
		surface->DrawString(surface, mac_address, -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;

		snprintf(myholder, 34, "NAME       : %s", CurrentUser.name);
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;

		snprintf(myholder, 34, "Time & Date : %-19s.", getdate_mine());
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;

		surface->DrawString(surface, "-----------------------------------", -1,x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height + 1;

		surface->SetFont(surface, font_32);
		surface->DrawString(surface, "TRANSACTION DETAILS", -1, x, y,DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		y += height + 1;

		surface->DrawString(surface, "-----------------------------------", -1,x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		surface->SetFont(surface, font_24);
		y += height + 1;

		printf("Line 659\n x_report_"
				"total_txt_number : \n");
		snprintf(myholder, 34, "Total Transaction : %d",
				x_report_total_txt_number);
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;

		printf("Line 667\n x_report_total_txt_number :\n");
		snprintf(myholder, 34, "Total Amount     : %d",x_report_total_txt_amount);
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;

		snprintf(myholder, 34, "Total Voids No   : %d",x_report_total_txt_number_voided);
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;

		snprintf(myholder, 34, "Total Void Amount : %d",x_report_total_txt_amount_voided);
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;

		snprintf(myholder, 34, "Net Amount Collected: %d",x_report_total_txt_amount - x_report_total_txt_amount_voided);
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;

		printf("Line 667\n x_report_total_txt_number :\n");
		snprintf(myholder, 50, "Z done Successfully");
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, myholder, -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;

		surface->DrawString(surface, "_________________________________", -1, x,y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		surface->SetFont(surface, font_32);
		y += height - 8;
		snprintf(myholder, 34, "Served by  %s",CurrentUser.name);
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, myholder , -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;


		surface->SetFont(surface, font_24);
		surface->DrawString(surface, "Thank you, welcome again", -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;

		surface->SetFont(surface, font_32);
		surface->DrawString(surface, "-----------------------------------", -1,
				x, y, DSTF_TOPCENTER);
		font_32->GetHeight(font_32, &height);
		y += height + 1;

		surface->SetFont(surface, font_24);
		surface->DrawString(surface, "POWERED BY COMPULYNX", -1, x, y,DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;
		surface->SetFont(surface, font_24);
		surface->DrawString(surface, " ", -1, x, y, DSTF_TOPCENTER);
		font_24->GetHeight(font_24, &height);
		y += height + 1;


	}
	printf("Line 712\n");

	if (0 == retval) {

		lcd_printf(ALG_LEFT, "Printing....");
		lcd_flip();
		kb_hit();
		print_surface(ifd, load_image(dfb, filename), y);
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
					lcd_flip();
					kb_getkey();
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
					lcd_flip();
					kb_getkey();
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

		lcd_printf(ALG_LEFT, "Finished.");
		lcd_flip();
	}

/*	if (NULL != dfb)
		dfb->Release(dfb);*/

	if (NULL != surface)
		surface->Release(surface);


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
	//PrintHeader();

}




















void display_image(void) {


	DFBSurfaceDescription surfdesc;
	IDirectFBSurface * surface = NULL;
	DFBFontDescription fdesc;
	printer_param_t param;
	//const char * disp_text = "HELLO WORLD";
	int x, y, height;
	int rectangle_x, rectangle_y, rectangle_width;
	lcd_clean();

	memset(&fdesc, 0, sizeof(fdesc));
	fdesc.flags = DFDESC_HEIGHT;
	surfdesc.flags = DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT;
			surfdesc.caps = DSCAPS_NONE;
			surfdesc.width = param.ro_width;
			surfdesc.height = 8000;
			dfb->CreateSurface(dfb, &surfdesc, &surface);
			surface->Clear(surface, 0xFF, 0xFF, 0xFF, 0xFF);

			surface->SetColor(surface, 0x00, 0x00, 0x00, 0xFF);

			x = param.ro_width / 2;
			y = 20;

			rectangle_x = 5;
			rectangle_y = y - 10;
			rectangle_width = param.ro_width - 5 * 2,

	load_image_for_screen(dfb, filename ,surface );

/*	if (NULL != surface)
		surface->Release(surface);*/
	//PrintHeader();

}




















void load_image_for_screen(IDirectFB *dfb, const char *filename ,IDirectFBSurface *main_surface ) {
	IDirectFBImageProvider *provider;
	IDirectFBSurface *imgsurf = NULL;
	DFBSurfaceDescription dsc;
	DFBResult err;
	 int i;
	 static int screen_width  = 0;
	 static int screen_height = 0;

	dsc.height = 32;
	err = dfb->CreateImageProvider(dfb, filename, &provider);
	if (err != DFB_OK) {
		fprintf( stderr, "Couldn't load image from file '%s': %s\n", filename,DirectFBErrorString(err));
		return NULL;
	}
	DFBCHECK (main_surface->GetSize (main_surface, &screen_width, &screen_height));
	provider->GetSurfaceDescription(provider, &dsc);
	dsc.flags = DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT;
	dsc.pixelformat = DSPF_ARGB;
	if (dfb->CreateSurface(dfb, &dsc, &imgsurf) == DFB_OK)
		provider->RenderTo(provider, imgsurf, NULL);

    DFBCHECK (main_surface->Blit (main_surface, imgsurf, NULL, (screen_width-dsc.width)/2, (screen_height - dsc.height) / 2));

    DFBCHECK (main_surface->Flip (main_surface, NULL, DSFLIP_WAITFORSYNC));

    kb_getkey();
	provider->Release(provider);
}
