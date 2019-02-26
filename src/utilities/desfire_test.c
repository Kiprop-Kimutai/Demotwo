/*
 * desfire_test.c

 *
 *  Created on: Oct 16, 2018
 *      Author: pos
 */

#include "posapi.h"
#include "seos.h"
#include "desfire.h"
#include "iso14443.h"
#include "miful.h"
#include "mifone.h"
#include "desfire_test.h"
//#include "main_old.h"

int peformCardOperations(void);
int openCardReader(void);
void initializeCardSectors(void);

char *readDataFromMifareCard(char *applicationName,char *filename);


int get_icCard(char  ** iccard_no){
	int fd=-1,pollresult = -1;
	char uidbuff[2],uidd[14];
	char uid[7];
	int k,ret;
	uint8_t x = 40;
	uint8_t *atslen = &x;
	char ats[40];
	int authstatus =-1;
	char app0[3] = {0x00,0x00,0x00};
    char authkey[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t keyno = 0;
	int createappstatus;

		uint8_t fileno = 1;

		desfire_appinfo info;
		(info).AID[0]=0x10;
		(info).AID[1]=0x00;
		(info).AID[2]=0x00;
		info.MasterKeySetting = 0;
		info.NumberOfKey = 10;
		info.CryptoMode =0;
		info.IsSupFID = 0;
		info.IsSupISOFID = 0;
		info.LenOfDFName = 0;

		desfire_std_backup_file_info fileinfo;
		fileinfo.file_size = 100;
		fileinfo.com_sett = 0x00;
		//fileinfo.FlgISOFileID = 0x00;
		fileinfo.change_access_right_keyno = 0;
		fileinfo.read_and_write_access_right_keyno = 0;
		fileinfo.write_access_right_keyno = 0;
		fileinfo.read_access_right_keyno = 0;

		char *datain;
		char dataout[100];
		uint32_t outlen;

	uint8_t kk = 2;
	uint8_t *kkk = &kk;
	char aids[84];

	fd = mif_open("/dev/nfc");
	printf("fd status %d\n",fd);
	if(fd<0){
		printf("Failed to open nfc reader module error %d\n",errno);

	}
	else{

		message_display_function(0 , "","Card Request" , "Please approch card and then press enter" , (char*) NULL);
		kb_getkey();
		pollresult =  desfire_poll(fd,0,uid,atslen,ats);
		while(1){
			 //mif_set_timeout(const struct mif_timeout *ptime)
			printf("Initialization done...\n");
			/*************************************************************************/
			/***poll card here ****/
			pollresult =  desfire_poll(fd,0,uid,atslen,ats);
			printf("poll result %d\n",pollresult);
			if(pollresult == 0){
				printf("card atslen %d\n",*atslen);
				for(k=0;k<7;k++){
					printf("%02X",ats[k]);
					memset(uidbuff,0,sizeof(uidbuff));
					sprintf(uidbuff,"%02x",ats[k]);
					strcat(uidd,uidbuff);
					*iccard_no = uidbuff;
					strcpy(uidbuff,"");
				}
				printf("\n\n");
				printf("uid %s\n",uidd);
				//do something here
			}
		}
	}
	return 0;
}

int peformCardOperations(){
	int fd = -1;
	int ret;
	int recv_len;
	int i;
	uint8_t mode;
	//uint8_t uid[20];
	char uid[50];
	char ats[50];
	uint8_t atslen[50];
	char app0[3] = {0x00,0x00,0x00};
	char authkey[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t keyno = 0;
	desfire_appinfo info;
	//uint8_t pointer[3]={0x00,0x00,0x01};
	(info).AID[0]=0x10;
	(info).AID[1]=0x00;
	(info).AID[2]=0x00;
	info.MasterKeySetting = 0;
	info.NumberOfKey = 1;
	info.CryptoMode =0;
	info.IsSupFID = 1;
	info.IsSupISOFID = 0;
	info.ISOFID[0] = 0x12;
	info.ISOFID[1] = 0x00;
	info.LenOfDFName = 0;

	desfire_std_backup_file_info fileinfo;
	fileinfo.file_size = 0x10;
	fileinfo.com_sett = 0x00;
	fileinfo.FlgISOFileID = 0x01;
	fileinfo.ISOFileID[0] = 0x00;
	fileinfo.ISOFileID[1] = 0x01;
	fileinfo.change_access_right_keyno = 0x00;
	fileinfo.read_and_write_access_right_keyno = 0x00;
	fileinfo.write_access_right_keyno = 0x00;
	uint8_t fileno = 1;
	//info.DFName 0;
	//open card reader
	/* Open device */
	printf("Mifare One Card Test\r\n");
	fd = mif_open("/dev/nfc");
	if (fd < 0) {
		printf("Open nfc failed! errno:%d\r\n", errno);
		//goto exit_entry;
		return 0;

	}
	printf("Open desfire success.Please approach card ....\n");
	while(1){
/*		lcdCls();
		lcdDisplay(0,0,DISP_CFONT,"open device success\n");
		lcdDisplay(0,2,DISP_CFONT,"please show the card\n");
		lcdFlip();*/
		message_display_function(1,"","Message","Device open successful.Please tap your card",(char *)NULL);
		/**poll card and get card uuid**/
		printf(".....\n");
		ret = desfire_poll(fd,0,uid,atslen, ats);
		printf("Returned value::%d\n",ret);
		//printf("Received data::%s\n",ats);
		printf("Received data length::%d\n",sizeof(ats));
		printf("Returned content::%s\n",ats);
		 // char  uid[16];
		  ret =  desfire_get_card_uid(fd,uid);
		  printf("card iccid:%s\n",uid);
		/**select master file******/
		ret = desfire_select_application(fd,app0);
		printf("Returned value on master file select::%d\n",ret);
		  ret =  desfire_get_card_uid(fd,uid);
		  printf("card iccid:%s\n",uid);
		/*now, authenticate card */
		printf("About to authenticate\n");
		 ret = desfire_authenticate(fd,keyno,authkey);
		 printf("Returned value on authentication::%d\n",ret);
		 printf("Init...\n");
		 /**create application ******/
		 printf("create application....\n");
		 ret = desfire_create_application(fd, &info);
		 printf("App creation returned value  %d\n", ret);
		 /**select application then create file*****/
		 ret =  desfire_select_application(fd,info.AID);
		 ret = desfire_authenticate(fd,keyno,authkey);
		 printf("App selection result:%d\n",ret);
		 ret =  desfire_create_std_datafile(fd,fileno,&fileinfo);
		  printf("File creation result:::%d\n",ret);
		  char * martin="Martin chirchir";
		  //uint8_t fileno;
		  ret=desfire_write_data(fd,fileno,0x00, 0, strlen(martin), martin);
		  printf("Write result::%d\n",ret);
		  uint32_t outlen;
		  char dataout[50];
		  ret =  desfire_read_data(fd,fileno,0x00,0,strlen(martin),&outlen,dataout);
		  printf("Read value::%d\n",ret);
		  printf("Read data::%s\n",dataout);

		  /**read card iccid***/

		//goto close_exit;
		//kbGetKeyMs(1000);
		//lcdFlip();
		mif_close(fd);
		return 0;

	}

}

int openCardReader(void){
	int fd = -1;
	/** open device card reader**/
	fd = mif_open("/dev/nfc");
	if(fd<0){
		printf("Open nfc failed! errno:%d\r\n", errno);
		return 0;
	}
	return 0;
}

void initializeCardSectors(void){
	peformCardOperations();
}
int createApplicationAndFileAndWriteData(char *data1, char *data2){
	int fd = -1;
	int ret;
	char MF[3] = {0x00,0x00,0x00};
	char mfauthkey[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t keyno = 0;
	uint8_t fileno=-1;
	char dataout[50];
	uint32_t outlen;
	desfire_appinfo info;
	//uint8_t pointer[3]={0x00,0x00,0x01};
	(info).AID[0]=0x10;
	(info).AID[1]=0x00;
	(info).AID[2]=0x00;

	info.MasterKeySetting = 0;
	info.NumberOfKey = 1;
	info.CryptoMode =0;
	info.IsSupFID = 1;
	info.IsSupISOFID = 0;
	info.ISOFID[0] = 0x13;
	info.ISOFID[1] = 0x00;
	info.LenOfDFName = 0;

	desfire_std_backup_file_info fileinfo;
	fileinfo.file_size = 0x10;
	fileinfo.com_sett = 0x00;
	fileinfo.FlgISOFileID = 0x01;
	//fileinfo.ISOFileID[0] = 0x00;
	//fileinfo.ISOFileID[1] = 0x01;
	fileinfo.change_access_right_keyno = 0x00;
	fileinfo.read_and_write_access_right_keyno = 0x00;
	fileinfo.write_access_right_keyno = 0x00;

	//fd = mif_open("/dev/nfc");
	printf("Mifare One Card Test\r\n");
	fd = mif_open("/dev/nfc");
	if(fd<0){
		printf("open nfc failed! errno:%d\r\n",errno);
		return 0;
	}
	printf("Open desfire success.Please approach card ....\n");
	while(1){
		//lcdCls();
		//lcdDisplay(0,0,DISP_CFONT,"open device success\n");
		//lcdDisplay(0,2,DISP_CFONT,"please show the card\n");
		//lcdFlip();
		/**poll card and get card uuid**/
		printf(".....\n");
		/**select master file******/
		ret = desfire_select_application(fd,MF);
		printf("Returned value on master file select::%d\n",ret);
		/*now, authenticate card */
		printf("About to authenticate\n");
		 ret = desfire_authenticate(fd,keyno,mfauthkey);
		 printf("Returned value on authentication::%d\n",ret);
		 /**create application ******/
		 printf("create application....\n");
		 ret = desfire_create_application(fd, &info);
		 printf("App creation returned value %d\n",ret);
		 /*select application then create file****/
		 ret =  desfire_select_application(fd,info.AID);
		 ret = desfire_authenticate(fd,keyno,mfauthkey);
		 printf("App selection result:%d\n",ret);

		 fileno = 5;
		 fileinfo.ISOFileID[0] = 0x00;
		 fileinfo.ISOFileID[1] = 0x01;

		 ret =  desfire_create_std_datafile(fd,fileno,&fileinfo);
		  printf("Personal details  file creation result:::%d\n   ",ret);

		  fileno = 6;
		  fileinfo.ISOFileID[0] = 0x00;
		  fileinfo.ISOFileID[1] = 0x02;

		  ret =  desfire_create_std_datafile(fd,fileno,&fileinfo);
		  		  printf("Bio details file creation result:::%d\n",ret);
		  char * martin="Martin chirchir";
		  //uint8_t fileno;
		  fileno=1;
		  ret=desfire_write_data(fd,fileno,0x00, 0, strlen(data1), data1);
		  printf("Write result for data1::%d\n  ",ret);
		  fileno=2;
		  ret=desfire_write_data(fd,fileno,0x00, 0, strlen(data2), data2);
		  printf("Write result for data2::%d\n  ",ret);
		  fileno=1;
		  ret =  desfire_read_data(fd,fileno,0x00,0,strlen(data1),&outlen,dataout);
		  printf("Read value::%d\n",ret);
		  printf("Read data1::%s\n",dataout);
		  fileno=2;
		  ret =  desfire_read_data(fd,fileno,0x00,0,strlen(data2),&outlen,dataout);
		  printf("Read value::%d\n",ret);
		  printf("Read data2::%s\n",dataout);
		//goto close_exit;
		//kbGetKeyMs(1000);
		//lcdFlip();
		mif_close(fd);
		return ret;

	}
	//return 0;
}

char * readDataFromMifareCard(char *applicationName,char *filename){
	int fd = -1;
	char MF[3] = {0x00,0x00,0x00};
	char mfauthkey[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	int ret;
	uint8_t keyno = 0;
	uint8_t fileno = -1;
	desfire_appinfo info;
	//char dataout[3000];
	char filedata[1800];
	uint32_t outlen;
	//uint8_t pointer[3]={0x00,0x00,0x01};
	(info).AID[0]=0x10;
	(info).AID[1]=0x00;
	(info).AID[2]=0x00;

	info.MasterKeySetting = 0;
	info.NumberOfKey = 1;
	info.CryptoMode =0;
	info.IsSupFID = 1;
	info.IsSupISOFID = 0;
	info.ISOFID[0] = 0x12;
	info.ISOFID[1] = 0x00;
	info.LenOfDFName = 0;

	if(strcmp(applicationName,"personal") == 0){
		fileno = 1;
	}
	else{
		fileno = 2;
	}
	fd = mif_open("/dev/nfc");
	if(fd<0){
		printf("open nfc failed! errno:%d\r\n",errno);
		return -1;
	}
	printf("Open desfire success.Please approach card ....\n");
	while(1){
		//lcdCls();
		//lcdDisplay(0,0,DISP_CFONT,"open device success\n");
		//lcdDisplay(0,2,DISP_CFONT,"please show the card\n");
		//lcdFlip();
		ret = desfire_select_application(fd,MF);
		printf("Returned value on master file select::%d\n",ret);
		/*now, authenticate card */
		printf("About to authenticate\n");
		 ret = desfire_authenticate(fd,keyno,mfauthkey);
		 printf("Returned value on authentication::%d\n",ret);
		 /*select application then create file****/
		 ret =  desfire_select_application(fd,info.AID);
		 ret = desfire_authenticate(fd,keyno,mfauthkey);
		 printf("App selection result:%d\n",ret);
		 ret =  desfire_read_data(fd,fileno,0x00,0,1800,&outlen,filedata);
		 strcpy(dataout,filedata);
		 strcpy(filedata,"");
		 ret =  desfire_read_data(fd,fileno,0x00,0,1800,&outlen,filedata);
		 strcat(dataout,filedata);
		 printf("Read value::%d\n",ret);
		 printf("Read data::%s\n",dataout);
		 return dataout;
	}
}


