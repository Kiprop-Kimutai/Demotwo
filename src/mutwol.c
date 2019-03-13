/*
 * mutwol.c

 *
 *  Created on: Dec 9, 2018
 *      Author: pos
 */

#include "mutwol.h"
#include "posapi.h"
#include "seos.h"
#include "desfire.h"
#include "iso14443.h"
#include "miful.h"
#include "mifone.h"
#include "../src/utilities/keyboart.h"
#include "../src/utilities/lcd.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
//#include "cardinfo.h"
#include "../src/utilities/cJSON.h"
#include "../src/utilities/sgfplib.h"
#include "fingerprint_reader.h"
#include "../src/utilities/common_functions.h"


int enrollbeneficiary(int fd,char MF[3],uint8_t keyno,char *authkey);
int createfpfile(int fd,uint8_t keyno,char MF[3],char *authkey,unsigned char *fp);
int createbeneficiaryapplication(int fd,uint8_t keyno,char MF[3],char *authkey);
int createbiodatafile(int fd,uint8_t keyno,char MF[3],char *authkey,char *biodata);
int createtransactionfile(int fd,uint8_t keyno,char MF[3],char *authkey , char  * balances);
int readfile(int fd,uint8_t keyno,char MF[3],char APP[3],char *authkey,char ** personal_details ,char ** transaction_file , BYTE * fingerprint[]);
//int readfile(int fd,uint8_t keyno,char MF[3],char APP[3],char *authkey,char * response[] , BYTE * fingerprint[]);
//int cardoperations(int operation,char *data,char * response[] , BYTE * fingerprint[]);
int writefile(int fd,uint8_t keyno,char MF[3],char APP[3],char * authkey,char * transactiondata,char* personaldetails);
int fd;



char * readcarduid(void){
	/**
	 * requirements:
	 *   -card must be authenticated with any key before command is issued
	 *   -authentication must be either desfire_authenticate_iso/desfire_authenticate_aes
	 */
	int fdd,ret;
	char *uid;
	char app0[3] = {0x00,0x00,0x00};
	char authkey[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	fdd = mif_open("/dev/nfc");
	if(fdd>=0){
		int ret = desfire_select_application(fd,app0);
		printf("mifare open success\n");
		ret = desfire_authenticate(fdd,(uint8_t )0,authkey);
		if(ret>=0){
			printf("authentication success");
			ret =  desfire_get_card_uid(fdd,uid);
			printf("card uid status:::%d\n",ret);
			if(ret>=0){
				printf("card uid %s\n",uid);
				desfire_deactive(fd);
				mif_close(fd);
				return uid;
			}
			desfire_deactive(fd);
			mif_close(fd);
			return ""; //get card uid failed
		}
		else{
			printf("card authentication failed...with result %d error no  %d \n",ret ,errno);
			desfire_deactive(fd);
			mif_close(fd);
			return "";
		}

	}
	else{
		desfire_deactive(fd);
		mif_close(fd);
		return ""; //mifare open failed
	}
}


int testnfc(){
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
	/*		(info).AID[0]=0x10;
		(info).AID[1]=0x00;
		(info).AID[2]=0x00;
		info.MasterKeySetting = 0;
		info.NumberOfKey = 10;
		info.CryptoMode =0;
		info.IsSupFID = 0;
		info.IsSupISOFID = 0;
		info.LenOfDFName = 0;*/

	/*	desfire_std_backup_file_info fileinfo;
		fileinfo.file_size = 100;
		fileinfo.com_sett = 0x00;
		//fileinfo.FlgISOFileID = 0x00;
		fileinfo.change_access_right_keyno = 0;
		fileinfo.read_and_write_access_right_keyno = 0;
		fileinfo.write_access_right_keyno = 0;
		fileinfo.read_access_right_keyno = 0;*/

	char *datain;
	char dataout[100];
	uint32_t outlen;

	uint8_t kk = 2;
	uint8_t *kkk = &kk;
	char aids[84];

	/////Start
	int timeout=0;
	fd = mif_open("/dev/nfc");
	if(fd>=0){
		//open card success. Tap card and get poll status
		message_display_function(1,"","Please tap card", "", (char *)NULL);
		while(1){
			timeout++;
			//printf("waiting for card...\n");
			//printf("%d\n",timeout);;
			//poll card if it is within RF field
			pollresult =  desfire_poll(fd,0,uid,atslen,ats);
			if(pollresult >= 0){
				printf("card atslen %d\n",*atslen);
				strcpy(uidd ,  "");
				printf("Dec \n\n");
				for(k=0;k<7;k++){
					//printf("k :  %d\n" , k);
					printf("%02X",ats[k]);
				}
				printf(" Int \n\n");
				for(k=0;k<7;k++){
					//printf("k :  %d\n" , k);
					//printf("%02X",ats[k]);
					printf("%d",ats[k]);
					memset(uidbuff,0,sizeof(uidbuff));
					sprintf(uidbuff,"%02X",ats[k]);
					//strcat(uidd,uidbuff);
					//strcpy(uidbuff,"");
				}
				printf("\n\n");
				printf("uid %s\n",uidd);
				//do something here


			}
		}
	}

	kb_getkey();
	//End

	fd = mif_open("/dev/nfc");
	printf("fd status %d\n",fd);
	if(fd<0){
		printf("Failed to open nfc reader module error %d\n",errno);
	}
	else{
		pollresult =  desfire_poll(fd,0,uid,atslen,ats);
		while(1){
			//mif_set_timeout(const struct mif_timeout *ptime)
			printf("Initialization done...\n");
			/*************************************************************************/
			/***poll card here ****/
			//pollresult =  desfire_poll(fd,0,uid,atslen,ats);
			printf("poll result %d\n",pollresult);
			if(pollresult >= 0){
				printf("card atslen %d\n",*atslen);
				for(k=0;k<7;k++){
					printf("%02X",ats[k]);
					memset(uidbuff,0,sizeof(uidbuff));
					sprintf(uidbuff,"%02x",ats[k]);
					strcat(uidd,uidbuff);
					strcpy(uidbuff,"");
				}
				printf("\n\n");
				printf("uid %s\n",uidd);
				//do something here
			}
			else{
				printf("poll result failed...\n");
			}

			break;
		}
	}
	desfire_deactive(fd);
	printf("closing reader module  with result %d\n",mif_close(fd));
	//printf("closing reader module  with result \n");


	return 0;
}



int personalizecard(char *personaldetails,unsigned char *fp  ,  char  * balances , char * card_number){
	int pollresult =-1;
	int timeout=0;
	char uid[7];
	char temp_uid[30];
	uint8_t x = 40;
	uint8_t *atslen = &x;
	char ats[40];
	char app0[3] = {0x00,0x00,0x00};
	char benapp[3] = {0x11,0x00,0x00};
	char authkey[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t keyno = 0;
	printf("printing json here.....\n\n");
	printf("JSON string %s\n",personaldetails);
	printf("JSON Balances string %s\n",balances);
	printf("----------------------------------------------\n\n");
	fd = mif_open("/dev/nfc");



	if(fd>=0){
		//open card success. Tap card and get poll status
		message_display_function(1,"","Please tap card", "", (char *)NULL);
		while(1){
			timeout++;
			printf("waiting for card...\n");
			printf("%d\n",timeout);;
			//poll card if it is within RF field
			pollresult =  desfire_poll(fd,0,uid,atslen,ats);
			if(pollresult == 0){
				printf("card found\n\n");
				strcpy(temp_uid , processcarduid(uid));

				printf("card_uid_to_be_obtained : %s \n " ,  temp_uid);

				if(strcmp (temp_uid , card_number) == 0)
				{
					message_display_function(1,"","card found", "please do not remove card", (char *)NULL);
					//personalize card here
					int ret = desfire_select_application(fd,app0);
					if(ret<0)
					{
						message_display_function(1,"","Card Error", "Invalid card status. Please retry", (char *)NULL);
						kb_getkey();
						desfire_deactive(fd);
						mif_close(fd);
						return -1;
					}
					int authstatus = desfire_authenticate(fd,keyno,authkey);
					if(authstatus>=0){
						int x;
						ret =  desfire_get_card_uid(fd,uid);
						printf("card iccid:%s\n",uid);
						x = createbeneficiaryapplication(fd,keyno,app0,authkey);
						if(x==0){

							int  writeops;
							writeops = createbiodatafile(fd,keyno,app0,authkey,personaldetails);
							sleep(1);
							writeops = createtransactionfile(fd,keyno,app0,authkey ,  balances);
							sleep(1);
							writeops =createfpfile(fd,keyno,app0,authkey,fp);
							sleep(1);

							printf("personalize card result %d\n",x);
							message_display_function(1,"","card personalization success", "please  remove card", (char *)NULL);
							kb_getkey();
							lcd_clean();
							desfire_deactive(fd);
							mif_close(fd);
							return 1;
							break;
						}
						else if(x==-3){
							//card already personalized
							message_display_function(1,"","personalization error", "card  used", (char *)NULL);
							kb_getkey();
							lcd_clean();
							desfire_deactive(fd);
							mif_close(fd);
							return 0;
							break;
						}

						else{
							printf("personalize card result %d\n",x);
							sleep(5);
							message_display_function(1,"","failed to personalize card", "please  retry", (char *)NULL);
							sleep(1);
							kb_getkey();
							lcd_clean();
							desfire_deactive(fd);
							mif_close(fd);
							return 0;
							break;
						}
					}
					else{
						//authentication failed
						printf("personalize card failed result %d corresponding to errno %d\n",authstatus,errno);
						sleep(3);
						message_display_function(1,"","card error", "authentication failed", (char *)NULL);
						sleep(1);
						kb_getkey();
						lcd_clean();
						desfire_deactive(fd);
						mif_close(fd);
						return 0;
						break;
					}
					break;
				}
				else
				{
					message_display_function(1,"","Card Error", "Please tap the original card and press enter.", (char *)NULL);
					mif_close(fd);
					kb_getkey();
				}
			}
			//keep waiting for card until timeout
			//sleep(1);
			if(timeout>500){
				printf("Timeout achieved\n");
				message_display_function(1,"","NFC card not found", "", (char *)NULL);
				sleep(2);
				lcd_clean();
				desfire_deactive(fd);
				mif_close(fd);
				return 0;
				break;
			}

		}
	}
	else{
		printf("open iccid failed with status %d corresponding to error %d\n",fd,errno);
		message_display_function(1,"","Card Reader Failed", "Please try again.", (char *)NULL);
		kb_getkey();
		desfire_deactive(fd);
		mif_close(fd);
		return 0;
	}
}


int enrollbeneficiary(int fd,char MF[3],uint8_t keyno,char *authkey){

	desfire_appinfo benapp;
	(benapp).AID[0]=0x11;
	(benapp).AID[1]=0x00;
	(benapp).AID[2]=0x00;
	benapp.MasterKeySetting = 0;
	benapp.NumberOfKey = 2;
	benapp.CryptoMode =0;
	benapp.IsSupFID = 0;
	benapp.IsSupISOFID = 0;
	benapp.LenOfDFName = 0;


	desfire_std_backup_file_info biodatafilee,balancefilee,transactionfilee;
	biodatafilee.file_size = 100;
	biodatafilee.com_sett = 0x00;
	biodatafilee.change_access_right_keyno = 0;
	biodatafilee.read_and_write_access_right_keyno = 0;
	biodatafilee.write_access_right_keyno = 0;
	biodatafilee.read_access_right_keyno = 0;
	uint8_t biodatafilenumberr = 0;
	/*************************************/
	int ret,authstatus;
	int createbiodatafileresult,createtransactionfileresult;
	/******************************************/

	while(1){
		printf("will personalize card.... with fd value %d\n\n",fd);
		//select MF
		ret = desfire_select_application(fd,MF);
		printf("select MF result %d corresponding to errno %d\n",ret,errno);
		authstatus = desfire_authenticate(fd,keyno,authkey);
		printf("Auth status %d optionally corresponding to errno %d\n",authstatus,errno);
		if(authstatus>=0){
			//create beneficiary app here
			ret = desfire_create_application(fd,&benapp);

			if(ret==0){
				//select app, authenticate and create files
				ret = desfire_select_application(fd,benapp.AID);
				if(ret==0){
					printf("app selected successfully\n");
					printf("creating files------\n\n");
					ret = desfire_authenticate(fd,keyno,authkey);
					printf("file authentication result %d corresponding to errorno %d\n",ret,errno);
					if(ret>=0){
						printf("Authentication successful\n");
						createbiodatafileresult =  desfire_create_std_datafile(fd,biodatafilenumberr,&biodatafilee);
						printf("biodaata file created with result %d and optionally error %d\n",createbiodatafileresult,errno);
						//ret = desfire_select_application(fd,MF);
						printf("select MF result %d corresponding to errno %d\n",ret,errno);
						//try creating your file here
						printf("trying to create files....\n\n");
					}
					else if(errno == 1150){ //optionally add other errors
						//invalid command length;
						/*desfire_deactive(fd);
						mif_close(fd);*/
						return -1;
					}

				}
				else{
					printf("App selection failed with err %d and status %d\n",ret,errno);

				}

			}
			else if(ret!=0 && errno == 1246){
				//application exists.return positive
			/*	desfire_deactive(fd);
				mif_close(fd);*/
				return 1;
			}
			else{
				//unknown error.
				printf("beneficiary app creation failed with status %d corresponding to errno %d\n",ret,errno);
			/*	desfire_deactive(fd);
				mif_close(fd);*/
				return -1;
			}


		}
		else{
			printf("authentication failed with err %d corresponding to status %d\n",authstatus,errno);
		}


		break;
	}
	desfire_deactive(fd);
	mif_close(fd);
	return -1;

}

int createbeneficiaryapplication(int fd,uint8_t keyno,char MF[3],char *authkey){
	desfire_appinfo benapp;
	(benapp).AID[0]=0x11;
	(benapp).AID[1]=0x00;
	(benapp).AID[2]=0x00;
	benapp.MasterKeySetting = 0;
	//benapp.NumberOfKey = 2;
	benapp.NumberOfKey = 3;
	benapp.CryptoMode =0;
	benapp.IsSupFID = 0;
	benapp.IsSupISOFID = 0;
	benapp.LenOfDFName = 0;

	int ret,authstatus;

	ret = desfire_select_application(fd,MF);
	printf("select MF result %d corresponding to errno %d\n",ret,errno);
	authstatus = desfire_authenticate(fd,keyno,authkey);
	printf("Auth status %d optionally corresponding to errno %d\n",authstatus,errno);
	if(authstatus >= 0){
		//go ahead and create application
		ret = desfire_create_application(fd,&benapp);
		if(ret==0){
			printf("beneficiary app created successfully\n");
			return 0;
		}
		else{
			if(errno==1226){
				//command was not fully executed.optionally log
			/*	desfire_deactive(fd);
				mif_close(fd);*/
				return -1;
			}
			else if(errno ==1150){
				//length of command string invalid
				/*desfire_deactive(fd);
				mif_close(fd);*/
				return -2;
			}
			else if(errno ==1246){
				//file/app with similar id exists
			/*	desfire_deactive(fd);
				mif_close(fd);*/
				return -3;
			}
			else{
				/*desfire_deactive(fd);
				mif_close(fd);*/
				return -4;
			}
		}

	}
	else{
		desfire_deactive(fd);
		mif_close(fd);
		return-5;
	}

}

int createbiodatafile(int fd,uint8_t keyno,char MF[3],char *authkey,char *biodata){
	desfire_appinfo benapp2;
	(benapp2).AID[0]=0x11;
	(benapp2).AID[1]=0x00;
	(benapp2).AID[2]=0x00;
	benapp2.MasterKeySetting = 0;
	//benapp2.NumberOfKey = 2;
	benapp2.NumberOfKey = 3;
	benapp2.CryptoMode =0;
	benapp2.IsSupFID = 0;
	benapp2.IsSupISOFID = 0;
	benapp2.LenOfDFName = 0;

	desfire_std_backup_file_info biodatafilee;
	biodatafilee.file_size = 200;
	biodatafilee.com_sett = 0x00;
	biodatafilee.change_access_right_keyno = 0;
	biodatafilee.read_and_write_access_right_keyno = 0;
	biodatafilee.write_access_right_keyno = 0;
	biodatafilee.read_access_right_keyno = 0;
	uint8_t biodatafilenumberr = 0;

	int ret,authstatus;
	ret = desfire_select_application(fd,MF);
	printf("select MF result %d corresponding to errno %d\n",ret,errno);
	authstatus = desfire_authenticate(fd,keyno,authkey);
	printf("Auth status %d optionally corresponding to errno %d\n",authstatus,errno);
	ret = desfire_select_application(fd,benapp2.AID);
	if(ret==0){
		printf("app selected successfully\n");
		authstatus = desfire_authenticate(fd,keyno,authkey);
		printf("auth status %d corresponding to errno %d\n",authstatus,errno);
		//create file of choice
		ret =  desfire_create_std_datafile(fd,biodatafilenumberr,&biodatafilee);
		if(ret==0){
			printf("Biodata File created successfully----- with result %d\n",ret);
			ret = desfire_write_data(fd,biodatafilenumberr,0x00,0,strlen(biodata),biodata);
			if(ret==0){
				printf("biodata file written successfully with result %d\n\n",ret);
				/*desfire_deactive(fd);
				mif_close(fd);*/
				return 0;
			}
			else{
				printf("Error writing biodata to card corresponding to errno %d\n",errno);
			/*	desfire_deactive(fd);
				mif_close(fd);*/
				return -1;
			}

		}
		else{
			printf("File creation failed with status %d corresponding to errno %d\n",ret,errno);
			/*desfire_deactive(fd);
			mif_close(fd);*/
			return -1;
		}
	}
	else{
	/*	desfire_deactive(fd);
		mif_close(fd);*/
		return -1;
	}

}
int createfpfile(int fd,uint8_t keyno,char MF[3],char *authkey,unsigned char *fp){
	int ret,ret1,authstatus;
	desfire_appinfo benapp2;
	(benapp2).AID[0]=0x11;
	(benapp2).AID[1]=0x00;
	(benapp2).AID[2]=0x00;
	benapp2.MasterKeySetting = 0;
	//benapp2.NumberOfKey = 2;
	benapp2.NumberOfKey = 3;
	benapp2.CryptoMode =0;
	benapp2.IsSupFID = 0;
	benapp2.IsSupISOFID = 0;
	benapp2.LenOfDFName = 0;


	desfire_std_backup_file_info balancefilee;
	balancefilee.file_size = 400;
	balancefilee.com_sett = 0x00;
	balancefilee.change_access_right_keyno = 0;
	balancefilee.read_and_write_access_right_keyno = 0;
	balancefilee.write_access_right_keyno = 0;
	balancefilee.read_access_right_keyno = 0;
	//uint8_t balancefilenumber = 0;
	uint8_t balancefilenumber = 1;

	ret = desfire_select_application(fd,MF);
	printf("select MF result %d corresponding to errno %d\n",ret,errno);
	authstatus = desfire_authenticate(fd,keyno,authkey);
	printf("Auth status %d optionally corresponding to errno %d\n",authstatus,errno);
	//ret1 = desfire_create_application(fd,&benapp2);
	//printf("app created with status %d and potentially errno %d\n",ret1,errno);
	//now select app
	ret = desfire_select_application(fd,benapp2.AID);
	if(ret==0){
		printf("app selected successfully\n");
		authstatus = desfire_authenticate(fd,keyno,authkey);
		printf("auth status %d corresponding to errno %d\n",authstatus,errno);
		//create file of choice
		ret =  desfire_create_std_datafile(fd,balancefilenumber,&balancefilee);
		if(ret==0){
			printf("File created successfully----- with result %d\n",ret);
			ret = desfire_write_data(fd,balancefilenumber,0x00,0,400,fp);
			if(ret == 0){
				printf("fingerprint details written successfully with result %d\n",ret);
			/*	desfire_deactive(fd);
				mif_close(fd);*/
				return 0;
			}
			else{
				printf("fingerprint details written  with result %d corresponding to errno %d\n",ret,errno);
				/*desfire_deactive(fd);
				mif_close(fd);*/
				return -1;
			}
		}
		else{
			message_display_function(1,"","FP Create  File", "File creation failed with status %d corresponding to errno %d\n",ret,errno, (char *)NULL);
			kb_getkey();
			printf("File creation failed with status %d corresponding to errno %d\n",ret,errno);
			/*desfire_deactive(fd);
			mif_close(fd);*/
			return -1;
		}
	}
	else{
		/*desfire_deactive(fd);
		mif_close(fd);*/
		return -1;
	}

}

int createtransactionfile(int fd,uint8_t keyno,char MF[3],char *authkey ,  char  * balances){
	int ret,ret1,authstatus;
	desfire_appinfo benapp2;
	(benapp2).AID[0]=0x11;
	(benapp2).AID[1]=0x00;
	(benapp2).AID[2]=0x00;
	benapp2.MasterKeySetting = 0;
	//benapp2.NumberOfKey = 2;
	benapp2.NumberOfKey = 3;
	benapp2.CryptoMode =0;
	benapp2.IsSupFID = 0;
	benapp2.IsSupISOFID = 0;
	benapp2.LenOfDFName = 0;


	desfire_std_backup_file_info transactionfile;
	transactionfile.file_size = 1500;
	transactionfile.com_sett = 0x00;
	transactionfile.change_access_right_keyno = 0;
	transactionfile.read_and_write_access_right_keyno = 0;
	transactionfile.write_access_right_keyno = 0;
	transactionfile.read_access_right_keyno = 0;
	uint8_t transactionfilenumber = 2;

	//char *txns = "{\"balances\":[],\"transactions\":{\"benTxn\":\"\"}}";
	//printf("Data  : %s\n" , txns1 );

	ret = desfire_select_application(fd,MF);
	printf("select MF result %d corresponding to errno %d\n",ret,errno);
	authstatus = desfire_authenticate(fd,keyno,authkey);
	printf("Auth status %d optionally corresponding to errno %d\n",authstatus,errno);
	ret = desfire_select_application(fd,benapp2.AID);
	if(ret==0){
		printf("app selected successfully\n");
		authstatus = desfire_authenticate(fd,keyno,authkey);
		printf("auth status %d corresponding to errno %d\n",authstatus,errno);
		//create file of choice
		ret =  desfire_create_std_datafile(fd,transactionfilenumber,&transactionfile);
		if(ret==0){
			printf("File created successfully----- with result %d\n",ret);
			ret = desfire_write_data(fd,transactionfilenumber,0x00,0,strlen(balances),balances);
			if(ret==0){
				printf("biodata file written successfully with result %d\n\n",ret);

				return 0;
			}
			else{

				printf("Error writing biodata to card corresponding to errno %d\n",errno);
				return -1;
			}
			return 0;
		}
		else{
			message_display_function(1,"","Transaction Create  File", "File creation failed with status %d corresponding to errno %d\n",ret,errno, (char *)NULL);
			kb_getkey();

			printf("File creation failed with status %d corresponding to errno %d\n",ret,errno);
			return -1;
		}
	}
	else{
		return -1;
	}
}

int cardoperations(int operation,char *data , char ** personal_details ,char ** transaction_file,  BYTE * fingerprint[] , char  *  input_personal_details , char  * input_transaction_data , char  * card_number ,  char ** card_uid_to_be_obtained){
	desfire_appinfo benapp2;
	(benapp2).AID[0]=0x11;
	(benapp2).AID[1]=0x00;
	(benapp2).AID[2]=0x00;
	benapp2.MasterKeySetting = 0;
	//benapp2.NumberOfKey = 2;
	benapp2.NumberOfKey = 3;
	benapp2.CryptoMode =0;
	benapp2.IsSupFID = 0;
	benapp2.IsSupISOFID = 0;
	benapp2.LenOfDFName = 0;
	char * p_details;
	char * t_details;
	char data_p_details[300];
	char temp_uid[30];



	int timeout = 0;
	char uid[7];
	uint8_t x = 40;
	uint8_t *atslen = &x;
	char ats[40];
	int pollresult;
	char app0[3] = {0x00,0x00,0x00};
	/*char benapp[3] = {0x11,0x00,0x00};*/
	char authkey[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t keyno = 0;
	int authstatus,ret,readops , writeops ;
	int fd = mif_open("/dev/nfc");
	if(fd>=0){
		//open card success. Tap card and get poll status
		message_display_function(1,"","Please tap card", "", (char *)NULL);
		while(1){
			timeout++;
			desfire_deactive(fd);
			pollresult =  desfire_poll(fd,0,uid,atslen,ats);
			if(pollresult == 0){
				printf("card found\n\n");

				beep(2000 ,  200);
				strcpy(temp_uid , processcarduid(uid));
				*card_uid_to_be_obtained = temp_uid;

				printf("card_uid_to_be_obtained : %s \n " ,  *card_uid_to_be_obtained);

				message_display_function(1,"","card found", "please do not remove card", (char *)NULL);
				//personalize card here
				ret = desfire_select_application(fd,app0);
				printf(" desfire_select_application : %d\n" ,  ret );
				if(ret <0)
				{
					beep(2000 ,  200);
					message_display_function(1,"","Card Error", "Invalid card status. Please retry", (char *)NULL);
					kb_getkey();
					desfire_deactive(fd);
					mif_close(fd);
					return -1;
				}
				authstatus = desfire_authenticate(fd,keyno,authkey);
				if(authstatus>=0){
					/*			  ret =  desfire_get_card_uid(fd,uid);
					  int w;
					  for(w= 0 ;w<7 ;w++)
					  printf("card iccid:%d\n",uid);*/
					//select app and authenticate
					ret = desfire_select_application(fd,benapp2.AID);
					if(ret <0)
					{
						if(operation == 3)
						{
							//Means card isnt personalized and can be used.
							beep(2000 ,  200);
							desfire_deactive(fd);
							mif_close(fd);
							return 1;
						}
						beep(2000 ,  200);
						message_display_function(1,"","Card Error", "The card is not personalized or the contact time is too short. Please try again  ", (char *)NULL);
						kb_getkey();

						desfire_deactive(fd);
						mif_close(fd);
						return -1;
					}
					if(operation == 3)
					{
						//Means card is personalized and cant be used.
						beep(2000 ,  200);
						message_display_function(1,"","Card Error", "The card is personalized and cannot be reused. Please pick another card and try again  ", (char *)NULL);
						kb_getkey();
						desfire_deactive(fd);
						mif_close(fd);
						return 0;
					}
					authstatus = desfire_authenticate(fd,keyno,authkey);
					if(authstatus>=0){
						//select and authenticate application success. perform card operations
						switch(operation){
						case 1:
						{

							char * final_transactions , *tx_returned;
							readops=  readfile(fd,keyno,app0,benapp2.AID,authkey, &p_details ,&t_details,fingerprint);
							if(readops>=0){
							printf("card read result::%d\n",readops);
							//peronal_details =
							printf("\n1: %s\n , " , p_details);
							strcpy(data_p_details   ,p_details);

							*personal_details = data_p_details;
							//final_transactions =  malloc(strlen(t_details));
							//strcpy(t_details );
							printf("out :%s\n , " , t_details);
							final_transactions  = strtok(t_details , "~");
							tx_returned  =  malloc(strlen(final_transactions) + 2);
							strcpy(tx_returned , final_transactions);
							*transaction_file = tx_returned;
							printf("\n1: %s\n , " , data_p_details);
							pretty_printf(tx_returned ,  100);
							beep(2000 ,  200);
							desfire_deactive(fd);
							mif_close(fd);
							}
							return readops;
							break;
						}
						case 2:
						{
							char * final_string = malloc(strlen(input_transaction_data) + 15);
							strcpy(final_string , input_transaction_data);
							strcat(final_string , "~");
							printf("card_uid_to_be_obtained : %s \n " ,  *card_uid_to_be_obtained);
							printf("card_passed : %s \n " ,  card_number);

							if(strcmp(temp_uid , card_number) == 0)
							{
								writeops = writefile(fd,keyno,app0,benapp2.AID,authkey,final_string,input_personal_details);
								printf("card update result ...%d\n",writeops);

/*								while(writeops == -1){
									message_display_function(1,  "" , "Error Writing card" ,  "The POS could not complete write data to  card. Please press enter, tap and hold until successful." , (char *) NULL);
									kb_getkey();
									writeops = writefile(fd,keyno,app0,benapp2.AID,authkey,final_string,input_personal_details);
								}*/
								beep(2000 ,  200);
								desfire_deactive(fd);
								mif_close(fd);
								return writeops;

							}
							else
							{
								beep(2000 ,  200);
								message_display_function(1,"","card error", "Wrong card presented. Please place the original  card and try  again", (char *)NULL);
								kb_getkey();

								desfire_deactive(fd);
								mif_close(fd);
								return -1;
								//printf("Wrong card presented\n");
								//Wrong card
							}
							break;
						}
						default:
							beep(2000 ,  200);
							desfire_deactive(fd);
							mif_close(fd);
							return -2;
							break;
						}
						mif_close(fd);
						return 1;
					}
					else{
						//printf personal details file not found
						beep(2000 ,  200);
						message_display_function(1,"","card error", "The card attached is not an issued card", (char *)NULL);
						kb_getkey();
						desfire_deactive(fd);
						mif_close(fd);
						return -1;
					}
				}
				else{
					//authentication failed
					printf("personalize card failed result %d %d \n",authstatus , errno);
					beep(2000 ,  200);
					message_display_function(1,"","card error", "Card authentication failed", (char *)NULL);
					kb_getkey();
					lcd_clean();
					desfire_deactive(fd);
					mif_close(fd);
					return -1;
					//return errno
					break;
				}
				break;
			}
			//keep waiting for card until timeout
			//sleep(1);
			if(timeout>500){
				printf("Timeout achieved\n");
				beep(2000 ,  200);
				message_display_function(1,"","NFC card not found", "", (char *)NULL);
				sleep(2);
				lcd_clean();
				desfire_deactive(fd);
				mif_close(fd);
				return 0;
				break;
			}
		}

	}
	else{
		printf("open iccid failed with status %d corresponding to error %d\n",fd,errno);
		beep(2000 ,  200);
		message_display_function(1,"","Card Reader Failed", "Please try again.", (char *)NULL);
		kb_getkey();
		desfire_deactive(fd);
		mif_close(fd);
		return 0;
	}
}


int readfile(int fd,uint8_t keyno,char MF[3],char APP[3],char *authkey,char ** personal_details ,char ** transaction_file , BYTE * fingerprint[]){
	int ret,ret1,authstatus;
	desfire_appinfo benapp2;
	(benapp2).AID[0]=0x11;
	(benapp2).AID[1]=0x00;
	(benapp2).AID[2]=0x00;
	benapp2.MasterKeySetting = 0;
	benapp2.NumberOfKey = 3;
	benapp2.CryptoMode =0;
	benapp2.IsSupFID = 0;
	benapp2.IsSupISOFID = 0;
	benapp2.LenOfDFName = 0;

	uint32_t outlen;
	char personaldetails[400];
	unsigned char fingerprints[400];
	char txndetails[1500];

	strcpy(personaldetails ,  "");


	ret = desfire_select_application(fd,MF);
	printf("select MF result %d corresponding to errno %d\n",ret,errno);
	authstatus = desfire_authenticate(fd,keyno,authkey);
	printf("Auth status %d optionally corresponding to errno %d\n",authstatus,errno);
	if(authstatus>=0){
	ret = desfire_select_application(fd,APP);
	if(ret >=0){
		printf("app selected successfully\n");
		authstatus = desfire_authenticate(fd,keyno,authkey);
		printf("auth status %d corresponding to errno %d\n",authstatus,errno);
		//read personal details
		ret =  desfire_read_data(fd,0,0x00,0,200,&outlen,personaldetails);
		if(ret >= 0){
			printf("data out ...\n");
			printf("personal details out:%s\n",personaldetails);
			//*(response+0) = personaldetails;
			//*personal_details  = "";
			*personal_details  = personaldetails;
			printf("personal details out:%s\n",*personal_details);
		}
		else{
			printf("Failed to read card with status ret %d corresponding to errno %d\n",ret,errno);
			desfire_deactive(fd);
			mif_close(fd);
			return -1;
		}
		//read transaction file
		ret =  desfire_read_data(fd,2,0x00,0,1500,&outlen,txndetails);
		if(ret >= 0){
			printf("data out ...\n");
			printf("transactions out : %s\n",txndetails);
			*transaction_file = txndetails;
			printf("<==========done============>\n");
		}
		else{
			printf("Failed to read card with status ret %d corresponding to errno %d\n",ret,errno);
			desfire_deactive(fd);
			mif_close(fd);
			return -1;
		}
		//read fingerprint file
		ret =  desfire_read_data(fd,1,0x00,0,400,&outlen,fingerprints);
		if(ret >= 0){
			printf("data out ...\n");
			printf("fingerprints out %d\n",outlen);
			*(fingerprint+0) = fingerprints;
			int w = 0;
			while(w<400)
			{
				//printf("%02X",fingerprint[w]);
				fp_response[w] = fingerprints[w];
				w++;
			}
			printf("done.....\n");
			desfire_deactive(fd);
			mif_close(fd);
			return 1;
		}
		else{
			printf("Fp : Failed to read card with status ret %d corresponding to errno %d\n",ret,errno);
			desfire_deactive(fd);
			mif_close(fd);
			return -1;
		}

	}
	else{
		desfire_deactive(fd);
		mif_close(fd);
		return -1;
	}
	}
	else{
		if(authstatus<0 && errno ==62 ){
		message_display_function(0, "","Card Error","card contact  period is insufficient. Please try  again" ,  (char *)NULL );
		kb_getkey();
		desfire_deactive(fd);
		mif_close(fd);
		return -1;
		}
		else{
			message_display_function(0, "","Card Error","Please try  again" ,  (char *)NULL );
			kb_getkey();
			desfire_deactive(fd);
			mif_close(fd);
			return -1;
		}
	}
}



int writefile(int fd,uint8_t keyno,char MF[3],char APP[3],char * authkey,char * transactiondata,char* personaldetails){
	int ret,ret1,authstatus;
	desfire_appinfo benapp2;
	(benapp2).AID[0]=0x11;
	(benapp2).AID[1]=0x00;
	(benapp2).AID[2]=0x00;
	benapp2.MasterKeySetting = 0;
	benapp2.NumberOfKey = 3;
	benapp2.CryptoMode =0;
	benapp2.IsSupFID = 0;
	benapp2.IsSupISOFID = 0;
	benapp2.LenOfDFName = 0;

	uint32_t outlen;

	ret = desfire_select_application(fd,MF);
	printf("select MF result %d corresponding to errno %d\n",ret,errno);
	authstatus = desfire_authenticate(fd,keyno,authkey);
	printf("Auth status %d optionally corresponding to errno %d\n",authstatus,errno);
	ret = desfire_select_application(fd,APP);
	if(ret>=0){
		printf("app selected successfully\n");
		authstatus = desfire_authenticate(fd,keyno,authkey);
		printf("auth status %d corresponding to errno %d\n",authstatus,errno);
		//write personal details
		if(strlen(personaldetails)>0){
			ret =  desfire_write_data(fd,0,0x00,0,strlen(personaldetails),personaldetails);
			//int desfire_write_data(int fd,uint8_t fileno,uint8_t comm_set,uint32_t offset,uint32_t len,personaldetails);
			if(ret >= 0){
				printf("data written successfully ...\n");
				printf("personal details data written %s\n",personaldetails);
				desfire_deactive(fd);
				mif_close(fd);
				/*	mif_close(fd);*/
				return 1;
			}
			else{
				printf("Px Failed to write card with status ret %d corresponding to errno %d\n",ret,errno);
				/*mif_close(fd);*/
				desfire_deactive(fd);
				mif_close(fd);
				return -1;
			}
		}
		//write transaction file
		if(strlen(transactiondata) >0){
			printf(transactiondata);
			ret =  desfire_write_data(fd,2,0x00,0,strlen(transactiondata),transactiondata);
			if(ret >= 0){
				printf("data out ...\n");
				printf("transaction data written %s\n",transactiondata);
				/*mif_close(fd);*/
				desfire_deactive(fd);
				mif_close(fd);
				return 1;
			}
			else{
				printf("Tx Failed to read card with status ret %d corresponding to errno %d\n",ret,errno);
				/*mif_close(fd);*/
				desfire_deactive(fd);
				mif_close(fd);
				return -1;
			}
		}

		return -1;
	}
	else{
		printf("beneficiary app not found\n");
		desfire_deactive(fd);
		mif_close(fd);
		/*mif_close(fd);*/
		return -1;
	}
}


char *processcarduid(char *uid){
	int k;
	char *finaluid;
	char *tempuid;
	char m[3];
	finaluid = malloc(50);
	tempuid = malloc(2 *sizeof(char));
	memset(finaluid,0,50);
	for(k=0;k<strlen(uid);k++){
		printf("%02X \n",uid[k]);
		sprintf(tempuid,"%02X",uid[k]);
		m[0] = tempuid[strlen(tempuid) - 2];
		m[1] = tempuid[strlen(tempuid) - 1];
		m[2] = '\0';
		strcat(finaluid,m);
		if(k==6)
			break;
	}
	printf("final uid %s\n",finaluid);
	return hextodecimalconverter(finaluid);
}

