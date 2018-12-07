/*************************************************************
 *
 * Author :      SecuGen Corporation
 * Description : FPLibTest main.cpp source code module
 * Copyright(c): 2005 SecuGen Corporation, All rights reserved
 * History : 
 * date        person   comments
 * ======================================================
 *
 *
 *************************************************************/

#include <stdio.h>
#include "../src1/general_funtions_and_varaibles.h"
#include "../src1/services.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../src/utilities/keyboart.h"
#include "../src/utilities/lcd.h"
#include "../src1/jsonread.h"
#include "../src1/main_old.h"
#include "utilities/sgfplib.h"
//#include ""

#define FALSE 0
#define TRUE 1

HSGFPM  hsgfplib;

// ---------------------------------------------------------------- main() ---

int fp_counter;
int fplib_test(int option)
{
	//option 0 = capture
	//Option 1  = compare
	long err;
	DWORD templateSize, templateSizeMax;
	DWORD quality;
	BYTE *imageBuffer1;
	BYTE imageBuffer2[13][405];
	BYTE *minutiaeBuffer1;
	SGDeviceInfoParam deviceInfo;
	DWORD score;
	SGFingerInfo fingerInfo;
	BOOL matched;
	int x = 0;
	int w , yr;
	int y , key;
	int n = 1;


	err = SGFPM_Create(&hsgfplib);
	if (!hsgfplib)
	{
		message_display_function(1,"","error", "could_not_innitialize_fingerprint_library",  (char *)NULL);
		kb_getkey();
		return FALSE;
	}

	//For Gray

	err = SGFPM_Init(hsgfplib,SG_DEV_FDU04);
	err = SGFPM_OpenDevice(hsgfplib,SG_DEV_AUTO);
	if(err != SGFDX_ERROR_NONE)
	{
		printf("Failed : SG_DEV_FDU04 \n");

		err = SGFPM_Init(hsgfplib,SG_DEV_FDU04);
		err = SGFPM_OpenDevice(hsgfplib,SG_DEV_AUTO);
		if(err != SGFDX_ERROR_NONE)
		{
			printf("Failed : SG_DEV_FDU04 \n");
			err = SGFPM_Init(hsgfplib,SG_DEV_FDU04);
			if(err != SGFDX_ERROR_NONE)
			{
				printf("Failed : Driver\n");
				message_display_function(1,"","Finger Print Error ", "fingerprint driver not found. Please connect and try  again",  (char *)NULL);
				kb_getkey();
				err = SGFPM_Terminate(hsgfplib);
				return 0;
			}
			err = SGFPM_OpenDevice(hsgfplib,SG_DEV_AUTO);
			if(err != SGFDX_ERROR_NONE)
			{
				printf("Failed : SG_DEV_FDU05 \n");
			}
		}
	}


	//For  Blue.

	/*err = SGFPM_Init(hsgfplib,SG_DEV_FDU03);
			err = SGFPM_OpenDevice(hsgfplib,SG_DEV_AUTO);
			if(err != SGFDX_ERROR_NONE)
			{
				printf("Failed : SG_DEV_FDU03 \n");

				err = SGFPM_Init(hsgfplib,SG_DEV_FDU03);
				err = SGFPM_OpenDevice(hsgfplib,SG_DEV_AUTO);
				if(err != SGFDX_ERROR_NONE)
				{
					printf("Failed : SG_DEV_FDU03 \n");
					err = SGFPM_Init(hsgfplib,SG_DEV_FDU03);
					if(err != SGFDX_ERROR_NONE)
					{
						printf("Failed : Driver\n");
						message_display_function(1,"","error", "fingerprint_driver_not_found",  (char *)NULL);
						kb_getkey();
						err = SGFPM_Terminate(hsgfplib);
						return 0;
					}
					err = SGFPM_OpenDevice(hsgfplib,SG_DEV_AUTO);
					if(err != SGFDX_ERROR_NONE)
					{
						printf("Failed : SG_DEV_FDU05 \n");
					}
				}
			}*/

	if (err == SGFDX_ERROR_NONE)
	{
		//Start get stored tempelate from Smartcard : If you aare sending to Anita , skip this part
		/**	fp_counter = 1 ;
		while(fp_counter<13)
		{
			if(get_fingerprint_from_smartcard())
			{
				if(response_lenght == 0)
					break;
				w=0;
				while(w < 200)
				{
					imageBuffer2[fp_counter-1][w] = fp_response1[w];
					w++;
				}
				w = 200;
				yr = 0;
				while(w < 400)
				{
					imageBuffer2[fp_counter-1][w] = fp_response[yr];
					w++;
					yr++;
				}
			}
			else
				break;
			fp_counter++;
		}

		//End getting template from card
		message_display_function(1,"","fingerprint_authentication","press_enter_after_placing_finger", (char *)NULL);

		if(fp_counter == 1)
		{
			message_display_function(1,"","error", "no_fingerprint_templates_found_in_card",  (char *)NULL);
			kb_getkey();
			err = SGFPM_CloseDevice(hsgfplib);
			err = SGFPM_Terminate(hsgfplib);
			return 0;
		}
		 **/

		//Compare fingerprint
		if(option == 1)
		{
			message_display_function(1,"","Fingerprint authentication","Please press enter after placing finger", (char *)NULL);
			while(n<3)
			{
				y = get_y_position();
				//Alex
				lcd_printf(ALG_LEFT ,"%s  %d : " ,"please place finger", n);
				lcd_flip();
				key = kb_getkey();
				if(key== 0x1B){
					err = SGFPM_CloseDevice(hsgfplib);
					err = SGFPM_Terminate(hsgfplib);
					return 0;
				}
				err = SGFPM_SetLedOn(hsgfplib,TRUE);
				err = SGFPM_SetBrightness(hsgfplib,30);
				deviceInfo.DeviceID = 0;
				err = SGFPM_GetDeviceInfo(hsgfplib,&deviceInfo);
				if (err == SGFDX_ERROR_NONE)
				{
					printf("\tdeviceInfo.DeviceID   : %ld\n", deviceInfo.DeviceID);
					printf("\tdeviceInfo.DeviceSN   : %s\n",  deviceInfo.DeviceSN);
					printf("\tdeviceInfo.ComPort    : %ld\n", deviceInfo.ComPort);
					printf("\tdeviceInfo.ComSpeed   : %ld\n", deviceInfo.ComSpeed);
					printf("\tdeviceInfo.ImageWidth : %ld\n", deviceInfo.ImageWidth);
					printf("\tdeviceInfo.ImageHeight: %ld\n", deviceInfo.ImageHeight);
					printf("\tdeviceInfo.Contrast   : %ld\n", deviceInfo.Contrast);
					printf("\tdeviceInfo.Brightness : %ld\n", deviceInfo.Brightness);
					printf("\tdeviceInfo.Gain       : %ld\n", deviceInfo.Gain);
					printf("\tdeviceInfo.ImageDPI   : %ld\n", deviceInfo.ImageDPI);
					printf("\tdeviceInfo.FWVersion  : %04X\n", (unsigned int) deviceInfo.FWVersion);
				}

				imageBuffer1 = (BYTE*) malloc(deviceInfo.ImageHeight*deviceInfo.ImageWidth);
				err = SGFPM_GetImage(hsgfplib,imageBuffer1);
				if (err != SGFDX_ERROR_NONE)
				{
					message_display_function(1,"","Error", "Please note that could not fetch image from finger print device",  (char *)NULL);
					kb_getkey();
					err = SGFPM_CloseDevice(hsgfplib);
					err = SGFPM_Terminate(hsgfplib);
					return 0;
				}

				err = SGFPM_SetLedOn(hsgfplib,FALSE);
				w=0;
				while(w<400)
				{
					printf("%02X",imageBuffer1[w]);
					myBenf.fingerprint[w]= imageBuffer1[w];
					w++;
				}
				if (err == SGFDX_ERROR_NONE)
				{
				}

				// getImageQuality()
				quality = 0;
				err = SGFPM_GetImageQuality(hsgfplib,deviceInfo.ImageWidth, deviceInfo.ImageHeight, imageBuffer1, &quality);

				///////////////////////////////////////////////
				// getMaxTemplateSize()
				quality = 0;
				err = SGFPM_GetMaxTemplateSize(hsgfplib,&templateSizeMax);

				///////////////////////////////////////////////
				minutiaeBuffer1 = (BYTE*) malloc(templateSizeMax);
				fingerInfo.FingerNumber = SG_FINGPOS_UK;
				fingerInfo.ViewNumber = 1;
				fingerInfo.ImpressionType = SG_IMPTYPE_LP;
				fingerInfo.ImageQuality = quality; //0 to 100
				err = SGFPM_CreateTemplate(hsgfplib,&fingerInfo, imageBuffer1, minutiaeBuffer1);
				printf("CreateTemplate returned : [%ld]\n",err);
				if (err == SGFDX_ERROR_NONE)
				{
					///////////////////////////////////////////////
					// getTemplateSize()
					quality = 0;
					err = SGFPM_GetTemplateSize(hsgfplib,minutiaeBuffer1, &templateSize);
					printf("We got the image");

				}
				x = 0;

				//Compare the FP
				while(x<fp_counter)
				{

					quality = 0;
					printf("CreateTemplate returned : [%ld]\n",err);
					err = SGFDX_ERROR_NONE;
					if (err == SGFDX_ERROR_NONE)
					{
						quality = 0;
						err = SGFPM_GetTemplateSize(hsgfplib, imageBuffer2[x], &templateSize);
					}
					// MatchTemplate()
					err = SGFPM_MatchTemplate(hsgfplib,minutiaeBuffer1, imageBuffer2[x], SL_LOW, &matched);
					//  kb_getkey();
					if (matched == TRUE)
					{

						lcd_set_font_color(COLOR_BLUE);
						//Alex
						lcd_printf_ex(ALG_RIGHT, y , "authentication_success");
						lcd_flip();
						lcd_set_font_color(COLOR_BLACK);
						sleep(1);
						printf("<<MATCH>>\n\n");
						err = SGFPM_CloseDevice(hsgfplib);
						err = SGFPM_Terminate(hsgfplib);
						return 1;
					}
					else
					{

						printf("<<NO MATCH>>\n\n");
					}

					err = SGFPM_GetMatchingScore(hsgfplib,minutiaeBuffer1, imageBuffer2[x], &score);
					printf("Score is : [%ld]\n\n",score);
					x++;

				}

				free(imageBuffer1);
				free(minutiaeBuffer1);
				imageBuffer1 = NULL;
				minutiaeBuffer1 = NULL;

				lcd_set_font_color(COLOR_RED);

				//Alex
				lcd_printf_ex(ALG_RIGHT, y , "authentication_failed");
				lcd_flip();
				lcd_set_font_color(COLOR_BLACK);

				n++;
			}
			message_display_function(1,"","Authentication failed ","Invalid finger print. Please try  again", (char *)NULL);
			kb_getkey();
			err = SGFPM_CloseDevice(hsgfplib);
			err = SGFPM_Terminate(hsgfplib);
			return 0;
		}
		else
		{
			y = get_y_position();
							//Alex
			message_display_function(1,"","Fetching Finger Print","Press Enter after placing finger", (char *)NULL);

							lcd_printf(ALG_LEFT ,"%s : " ,"Please place finger");
							lcd_flip();
							key = kb_getkey();
							if(key== 0x1B){
								err = SGFPM_CloseDevice(hsgfplib);
								err = SGFPM_Terminate(hsgfplib);
								return 0;
							}
							err = SGFPM_SetLedOn(hsgfplib,TRUE);
							err = SGFPM_SetBrightness(hsgfplib,30);
							deviceInfo.DeviceID = 0;
							err = SGFPM_GetDeviceInfo(hsgfplib,&deviceInfo);
							if (err == SGFDX_ERROR_NONE)
							{
								printf("\tdeviceInfo.DeviceID   : %ld\n", deviceInfo.DeviceID);
								printf("\tdeviceInfo.DeviceSN   : %s\n",  deviceInfo.DeviceSN);
								printf("\tdeviceInfo.ComPort    : %ld\n", deviceInfo.ComPort);
								printf("\tdeviceInfo.ComSpeed   : %ld\n", deviceInfo.ComSpeed);
								printf("\tdeviceInfo.ImageWidth : %ld\n", deviceInfo.ImageWidth);
								printf("\tdeviceInfo.ImageHeight: %ld\n", deviceInfo.ImageHeight);
								printf("\tdeviceInfo.Contrast   : %ld\n", deviceInfo.Contrast);
								printf("\tdeviceInfo.Brightness : %ld\n", deviceInfo.Brightness);
								printf("\tdeviceInfo.Gain       : %ld\n", deviceInfo.Gain);
								printf("\tdeviceInfo.ImageDPI   : %ld\n", deviceInfo.ImageDPI);
								printf("\tdeviceInfo.FWVersion  : %04X\n", (unsigned int) deviceInfo.FWVersion);
							}

							imageBuffer1 = (BYTE*) malloc(deviceInfo.ImageHeight*deviceInfo.ImageWidth);
							err = SGFPM_GetImage(hsgfplib,imageBuffer1);
							if (err != SGFDX_ERROR_NONE)
							{
								//message_display_function(1,"","error", "could_not_fetch_image_from_fingerprint_device",  (char *)NULL);
								//kb_getkey();
								//err = SGFPM_CloseDevice(hsgfplib);
								//err = SGFPM_Terminate(hsgfplib);
								//return 0;
							}

							err = SGFPM_SetLedOn(hsgfplib,FALSE);
							w=0;
							while(w<400)
							{
								printf("%02X",imageBuffer1[w]);
								w++;

							}
							//strcpy(myBenf.fpbyte, imageBuffer1[w] );
							if (err == SGFDX_ERROR_NONE)
							{
							}

							// getImageQuality()
							quality = 0;
							err = SGFPM_GetImageQuality(hsgfplib,deviceInfo.ImageWidth, deviceInfo.ImageHeight, imageBuffer1, &quality);

							///////////////////////////////////////////////
							// getMaxTemplateSize()
							quality = 0;
							err = SGFPM_GetMaxTemplateSize(hsgfplib,&templateSizeMax);

							///////////////////////////////////////////////
							minutiaeBuffer1 = (BYTE*) malloc(templateSizeMax);
							fingerInfo.FingerNumber = SG_FINGPOS_UK;
							fingerInfo.ViewNumber = 1;
							fingerInfo.ImpressionType = SG_IMPTYPE_LP;
							fingerInfo.ImageQuality = quality; //0 to 100
							err = SGFPM_CreateTemplate(hsgfplib,&fingerInfo, imageBuffer1, minutiaeBuffer1);
							printf("CreateTemplate returned : [%ld]\n",err);
							if (err == SGFDX_ERROR_NONE)
							{
								///////////////////////////////////////////////
								// getTemplateSize()
								quality = 0;
								err = SGFPM_GetTemplateSize(hsgfplib,minutiaeBuffer1, &templateSize);
								strcpy(myBenf.fingerprint, minutiaeBuffer1 );
								printf("We got the image");

							}
							free(imageBuffer1);
							free(minutiaeBuffer1);
							imageBuffer1 = NULL;
							minutiaeBuffer1 = NULL;
							err = SGFPM_CloseDevice(hsgfplib);
							err = SGFPM_Terminate(hsgfplib);
							return 1;

		}
	}
	else
	{
		message_display_function(1,"","Device Error ", "Finger print device not found. Please check  if it is properly connected and try  again", (char *)NULL);
		kb_getkey();
	}

	return 0;
}
