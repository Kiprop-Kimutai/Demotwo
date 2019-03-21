/*
 * testsecugen.c
 *
 *  Created on: Mar 19, 2019
 *      Author: linux
 */

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

#include "utilities/sgfplib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities/lcd.h"
#define FALSE 0
#define TRUE 1

HSGFPM  hsgfplib;

// ---------------------------------------------------------------- main() ---

int test_hamster_pro(void){


  long err;
  DWORD templateSize, templateSizeMax;
  DWORD quality;
  char function[100];
  char kbBuffer[100];
  char kbWhichFinger[100];
  int fingerLength = 0;
  char *finger;
  BYTE *imageBuffer1;
  BYTE *imageBuffer2;
  BYTE *imageBuffer3;
  BYTE *minutiaeBuffer1;
  BYTE *minutiaeBuffer2;
  BYTE *minutiaeBuffer3;
  FILE *fp = NULL;
  SGDeviceInfoParam deviceInfo;
  DWORD score;
  SGFingerInfo fingerInfo;
  BOOL matched;
  int i;


  for (i=0; i < 100; ++i)
     kbWhichFinger[i] = 0x00;


  printf("\n-------------------------------------\n");
  printf(  "SecuGen PRO SGFPLIB Test\n");
  printf(  "-------------------------------------\n");

  ///////////////////////////////////////////////
  // Initialize SGFPLib
  strcpy(function,"SGFPM_Create()");
  printf("\nCall %s\n",function);
  err = SGFPM_Create(&hsgfplib);
  if (!hsgfplib)
  {
      printf("ERROR - Unable to open SGFPM library\n");
      return FALSE;
  }
  printf("%s returned: %ld\n",function,err);

  ///////////////////////////////////////////////
  // Init()
  strcpy(function,"Init(SG_DEV_FDU05)");
  printf("\nCall %s\n",function);
  err = SGFPM_Init(hsgfplib,SG_DEV_FDU05);
  printf("%s returned: %ld\n",function,err);

  ///////////////////////////////////////////////
  // OpenDevice()
  strcpy(function,"OpenDevice(SG_DEV_FDU05)");
  printf("\nCall %s\n",function);
  err = SGFPM_OpenDevice(hsgfplib,SG_DEV_FDU05);
  printf("%s returned: %ld\n",function,err);

  if (err == SGFDX_ERROR_NONE)
  {
        ///////////////////////////////////////////////
        // setLedOn(TRUE)
        printf("Press <Enter> to turn fingerprint sensor LEDs on >> ");
        getc(stdin);
	strcpy(function,"SetLedOn(TRUE)");
        printf("\nCall %s\n",function);
        err = SGFPM_SetLedOn(hsgfplib,TRUE);
	printf("%s returned: %ld\n",function,err);

        ///////////////////////////////////////////////
        // setLedOn(FALSE)
        printf("Fingerprint Sensor LEDS should now be illuminated.\n\n");
        printf("Press <Enter> to turn fingerprint sensor LEDs off >> ");
        getc(stdin);
	strcpy(function,"SetLedOn(FALSE)");
        printf("\nCall %s\n",function);
        err = SGFPM_SetLedOn(hsgfplib,FALSE);
	printf("%s returned: %ld\n",function,err);

        ///////////////////////////////////////////////
        // setBrightness()
	strcpy(function,"SetBrightness()");
        printf("\nCall %s\n",function);
        err = SGFPM_SetBrightness(hsgfplib,30);
	printf("%s returned: %ld\n",function,err);

        ///////////////////////////////////////////////
        // getDeviceInfo()
	deviceInfo.DeviceID = 0;
	strcpy(function,"GetDeviceInfo()");
        printf("\nCall %s\n",function);
	err = SGFPM_GetDeviceInfo(hsgfplib,&deviceInfo);
	printf("%s returned: %ld\n",function,err);
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
	printf("\n");


        ///////////////////////////////////////////////
        ///////////////////////////////////////////////
        printf("Fingerprint Sensor LEDS should now be off.\n");
        printf("The next tests will require mutiple captures of the same finger.\n");
        printf("Which finger would you like to test with? (e.g. left thumb) >> ");
        fgets(kbWhichFinger,100,stdin);
	//Remove CR/NL (<ENTER>)
	fingerLength = strlen(kbWhichFinger);
        for (i=0; i < strlen(kbWhichFinger); ++i)
        {
            if ((kbWhichFinger[i] == 0x0A) || (kbWhichFinger[i] == 0x0D)|| (kbWhichFinger[i] == 0x00))
            {
                fingerLength = i;
                break;
            }
        }
        if (fingerLength > 0)
	{
	    finger = (char*) malloc(fingerLength + 1);
            strncpy(finger,kbWhichFinger,fingerLength);
	    finger[fingerLength] = 0x00;
	}
	else
	{
		finger = (char*) malloc (7);
		strcpy(finger,"finger");
	}

        ///////////////////////////////////////////////
        // getImage() - 1st Capture
        printf("Capture 1. Please place [%s] on sensor and press <ENTER> ",finger);
        getc(stdin);
        imageBuffer1 = (BYTE*) malloc(deviceInfo.ImageHeight*deviceInfo.ImageWidth);
	strcpy(function,"GetImage()");
        printf("\nCall %s\n",function);
        err = SGFPM_GetImage(hsgfplib,imageBuffer1);
	printf("%s returned: %ld\n",function,err);
        if (err == SGFDX_ERROR_NONE)
        {
	    sprintf(kbBuffer,"%s1.raw",finger);
	    fp = fopen(kbBuffer,"wb");
	    fwrite (imageBuffer1 , sizeof (BYTE) , deviceInfo.ImageWidth*deviceInfo.ImageHeight , fp);
            fclose(fp);
	}

        ///////////////////////////////////////////////
        // getImageQuality()
        quality = 0;
	strcpy(function,"GetImageQuality()");
        printf("\nCall %s\n",function);
        err = SGFPM_GetImageQuality(hsgfplib,deviceInfo.ImageWidth, deviceInfo.ImageHeight, imageBuffer1, &quality);
	printf("%s returned: %ld\n",function,err);
        printf("Image quality : [%ld]\n\n",quality);

        ///////////////////////////////////////////////
        // getMaxTemplateSize()
        quality = 0;
	strcpy(function,"GetMaxTemplateSize()");
        printf("\nCall %s\n",function);
        err = SGFPM_GetMaxTemplateSize(hsgfplib,&templateSizeMax);
	printf("%s returned: %ld\n",function,err);
        printf("Max Template Size : [%ld]\n\n",templateSizeMax);


        ///////////////////////////////////////////////
        // getMinutiae()
	strcpy(function,"CreateTemplate()");
        printf("\nCall %s\n",function);
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
	  strcpy(function,"GetTemplateSize()");
          printf("\nCall %s\n",function);
          err = SGFPM_GetTemplateSize(hsgfplib,minutiaeBuffer1, &templateSize);
	  printf("%s returned: %ld\n",function,err);
          printf("Template Size : [%ld]\n\n",templateSize);
	  sprintf(kbBuffer,"%s1.min",finger);
	  fp = fopen(kbBuffer,"wb");
	  fwrite (minutiaeBuffer1 , sizeof (BYTE) , templateSize , fp);
          fclose(fp);
        }

        ///////////////////////////////////////////////
        // getImage() - 2nd Capture
        printf("Capture 2. Remove and replace [%s] on sensor and press <ENTER> ",finger);
        getc(stdin);
        imageBuffer2 = (BYTE*) malloc(deviceInfo.ImageHeight*deviceInfo.ImageWidth);
        strcpy(function,"GetImage()");
        printf("\nCall %s\n",function);
        err = SGFPM_GetImage(hsgfplib,imageBuffer2);
        printf("%s returned: %ld\n",function,err);
        if (err == SGFDX_ERROR_NONE)
        {
            sprintf(kbBuffer,"%s2.raw",finger);
            fp = fopen(kbBuffer,"wb");
            fwrite (imageBuffer2 , sizeof (BYTE) , deviceInfo.ImageWidth*deviceInfo.ImageHeight , fp);
            fclose(fp);
        }

        ///////////////////////////////////////////////
        // getImageQuality()
        quality = 0;
        strcpy(function,"GetImageQuality()");
        printf("\nCall %s\n",function);
        err = SGFPM_GetImageQuality(hsgfplib,deviceInfo.ImageWidth, deviceInfo.ImageHeight, imageBuffer2, &quality);
        printf("%s returned: %ld\n",function,err);
        printf("Image quality : [%ld]\n\n",quality);

        ///////////////////////////////////////////////
        // getMinutiae()
        strcpy(function,"CreateTemplate()");
        printf("\nCall %s\n",function);
        minutiaeBuffer2 = (BYTE*) malloc(templateSizeMax);
        fingerInfo.FingerNumber = SG_FINGPOS_UK;
        fingerInfo.ViewNumber = 1;
        fingerInfo.ImpressionType = SG_IMPTYPE_LP;
        fingerInfo.ImageQuality = quality; //0 to 100
        err = SGFPM_CreateTemplate(hsgfplib,&fingerInfo, imageBuffer2, minutiaeBuffer2);
        printf("CreateTemplate returned : [%ld]\n",err);
        if (err == SGFDX_ERROR_NONE)
        {
          ///////////////////////////////////////////////
          // getTemplateSize()
          quality = 0;
          strcpy(function,"GetTemplateSize()");
          printf("\nCall %s\n",function);
          err = SGFPM_GetTemplateSize(hsgfplib,minutiaeBuffer2, &templateSize);
          printf("%s returned: %ld\n",function,err);
          printf("Template Size : [%ld]\n\n",templateSize);
          sprintf(kbBuffer,"%s2.min",finger);
          fp = fopen(kbBuffer,"wb");
          fwrite (minutiaeBuffer2 , sizeof (BYTE) , templateSize , fp);
          fclose(fp);
        }

        ///////////////////////////////////////////////
        // MatchTemplate()
        strcpy(function,"MatchTemplate()");
        printf("\nCall %s\n",function);
        err = SGFPM_MatchTemplate(hsgfplib,minutiaeBuffer1, minutiaeBuffer2, SL_NORMAL, &matched);
        printf("%s returned: %ld\n",function,err);
        if (matched == TRUE)
            printf("<<MATCH>>\n\n");
        else
            printf("<<NO MATCH>>\n\n");

        ///////////////////////////////////////////////
        // GetMatchingScore()
        strcpy(function,"GetMatchingScore()");
        printf("\nCall %s\n",function);
        err = SGFPM_GetMatchingScore(hsgfplib,minutiaeBuffer1, minutiaeBuffer2, &score);
        printf("%s returned: %ld\n",function,err);
        printf("Score is : [%ld]\n\n",score);

        ///////////////////////////////////////////////
        // getImage() - 3rd Capture
        printf("Capture 3. Remove and replace [%s] on sensor and press <ENTER> ",finger);
        getc(stdin);
        imageBuffer3 = (BYTE*) malloc(deviceInfo.ImageHeight*deviceInfo.ImageWidth);
        strcpy(function,"GetImage()");
        printf("\nCall %s\n",function);
        err = SGFPM_GetImage(hsgfplib,imageBuffer3);
        printf("%s returned: %ld\n",function,err);
        if (err == SGFDX_ERROR_NONE)
        {
            sprintf(kbBuffer,"%s3.raw",finger);
            fp = fopen(kbBuffer,"wb");
            fwrite (imageBuffer3 , sizeof (BYTE) , deviceInfo.ImageWidth*deviceInfo.ImageHeight , fp);
            fclose(fp);
        }

        ///////////////////////////////////////////////
        // getImageQuality()
        quality = 0;
        strcpy(function,"GetImageQuality()");
        printf("\nCall %s\n",function);
        err = SGFPM_GetImageQuality(hsgfplib,deviceInfo.ImageWidth, deviceInfo.ImageHeight, imageBuffer3, &quality);
        printf("%s returned: %ld\n",function,err);
        printf("Image quality : [%ld]\n\n",quality);

        ///////////////////////////////////////////////
        // getMinutiae()
        strcpy(function,"CreateTemplate()");
        printf("\nCall %s\n",function);
        minutiaeBuffer3 = (BYTE*) malloc(templateSizeMax);
        fingerInfo.FingerNumber = SG_FINGPOS_UK;
        fingerInfo.ViewNumber = 1;
        fingerInfo.ImpressionType = SG_IMPTYPE_LP;
        fingerInfo.ImageQuality = quality; //0 to 100
        err = SGFPM_CreateTemplate(hsgfplib,&fingerInfo, imageBuffer3, minutiaeBuffer3);
        printf("CreateTemplate returned : [%ld]\n",err);
        if (err == SGFDX_ERROR_NONE)
        {
          ///////////////////////////////////////////////
          // getTemplateSize()
          quality = 0;
          strcpy(function,"GetTemplateSize()");
          printf("\nCall %s\n",function);
          err = SGFPM_GetTemplateSize(hsgfplib,minutiaeBuffer3, &templateSize);
          printf("%s returned: %ld\n",function,err);
          printf("Template Size : [%ld]\n\n",templateSize);
          sprintf(kbBuffer,"%s3.min",finger);
          fp = fopen(kbBuffer,"wb");
          fwrite (minutiaeBuffer3 , sizeof (BYTE) , templateSize , fp);
          fclose(fp);
        }

        ///////////////////////////////////////////////
        // MatchTemplate()
        strcpy(function,"MatchTemplate()");
        printf("\nCall %s\n",function);
        err = SGFPM_MatchTemplate(hsgfplib,minutiaeBuffer1, minutiaeBuffer3, SL_NORMAL, &matched);
        printf("%s returned: %ld\n",function,err);
        if (matched == TRUE)
            printf("<<MATCH>>\n\n");
        else
            printf("<<NO MATCH>>\n\n");

        ///////////////////////////////////////////////
        // GetMatchingScore()
        strcpy(function,"GetMatchingScore()");
        printf("\nCall %s\n",function);
        err = SGFPM_GetMatchingScore(hsgfplib,minutiaeBuffer1, minutiaeBuffer3, &score);
        printf("%s returned: %ld\n",function,err);
        printf("Score is : [%ld]\n\n",score);


        ///////////////////////////////////////////////
        // closeDevice()
        printf("\nCall CloseDevice()\n");
        err = SGFPM_CloseDevice(hsgfplib);
        printf("CloseDevice returned : [%ld]\n",err);

        ///////////////////////////////////////////////
        // Terminate()
        printf("\nCall Terminate()\n");
        err = SGFPM_Terminate(hsgfplib);
        printf("Terminate returned : [%ld]\n",err);

        free(imageBuffer1);
        free(imageBuffer2);
        free(imageBuffer3);
        free(minutiaeBuffer1);
        free(minutiaeBuffer2);
        free(minutiaeBuffer3);
        imageBuffer1 = NULL;
        imageBuffer2 = NULL;
        imageBuffer3 = NULL;
        minutiaeBuffer1 = NULL;
        minutiaeBuffer2 = NULL;
        minutiaeBuffer3 = NULL;
        finger = NULL;

  }
  return 0;
}
