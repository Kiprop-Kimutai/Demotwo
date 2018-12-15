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


int enrollbeneficiary(int fd,char MF[3],uint8_t keyno,char *authkey);
int createfpfile(int fd,uint8_t keyno,char MF[3],char *authkey,unsigned char *fp);
int createbeneficiaryapplication(int fd,uint8_t keyno,char MF[3],char *authkey);
int createbiodatafile(int fd,uint8_t keyno,char MF[3],char *authkey,char *biodata);
int createtransactionfile(int fd,uint8_t keyno,char MF[3],char *authkey);
int readfile(int fd,uint8_t keyno,char MF[3],char APP[3],char *authkey,char * response[] , BYTE * fingerprint[]);
//int cardoperations(int operation,char *data,char * response[] , BYTE * fingerprint[]);
int fd;
int personalizecard(char *personaldetails,unsigned char *fp){
	int pollresult =-1;
	int timeout=0;
	char uid[7];
	uint8_t x = 40;
	uint8_t *atslen = &x;
	char ats[40];
	char app0[3] = {0x00,0x00,0x00};
	char benapp[3] = {0x11,0x00,0x00};
	char authkey[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t keyno = 0;
	printf("printing json here.....\n\n");
	printf("JSON string %s\n",personaldetails);
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
				message_display_function(1,"","card found", "please do not remove card", (char *)NULL);
				//personalize card here
				int ret = desfire_select_application(fd,app0);
				int authstatus = desfire_authenticate(fd,keyno,authkey);
				if(authstatus>=0){
					int x = createbeneficiaryapplication(fd,keyno,app0,authkey);
					if(x==0){

						createbiodatafile(fd,keyno,app0,authkey,personaldetails);
						createtransactionfile(fd,keyno,app0,authkey);
						createfpfile(fd,keyno,app0,authkey,fp);
						printf("personalize card result %d\n",x);
						sleep(2);
						message_display_function(1,"","card personalization success", "please  remove card", (char *)NULL);
						sleep(1);
						kb_getkey();
						lcd_clean();
						mif_close(fd);
						return 1;
						break;
					}
					else if(x==-3){
						//card already personalized
						message_display_function(1,"","personalization error", "card  used", (char *)NULL);
						sleep(1);
						kb_getkey();
						lcd_clean();
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
						mif_close(fd);
						return 0;
						break;
					}
				}
				else{
					//authentication failed
					printf("personalize card failed result %d\n",authstatus);
					sleep(3);
					message_display_function(1,"","card error", "authentication failed", (char *)NULL);
					sleep(1);
					kb_getkey();
					lcd_clean();
					mif_close(fd);
					return 0;
					break;
				}
				break;
			}
			//keep waiting for card until timeout
			//sleep(1);
			if(timeout>500){
				printf("Timeout achieved\n");
				message_display_function(1,"","NFC card not found", "", (char *)NULL);
				sleep(2);
				lcd_clean();
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
						return -1;
					}

				}
				else{
					printf("App selection failed with err %d and status %d\n",ret,errno);

				}

			}
			else if(ret!=0 && errno == 1246){
				//application exists.return positive
				return 1;
			}
			else{
				//unknown error.
				printf("beneficiary app creation failed with status %d corresponding to errno %d\n",ret,errno);
				return -1;
			}


		}
		else{
			printf("authentication failed with err %d corresponding to status %d\n",authstatus,errno);
		}


		break;
	}

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
			printf("beneficiary app created suucessfully\n");
			return 0;
		}
		else{
			if(errno==1226){
				//command was not fully executed.optionally log
				return -1;
			}
			else if(errno ==1150){
				//length of command string invalid
				return -2;
			}
			else if(errno ==1246){
				//file/app with similar id exists
				return -3;
			}
			else{
				return -4;
			}
		}

	}
	else{
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
				return 0;
			}
			else{
				printf("Error writing biodata to card corresponding to errno %d\n",errno);
				return -1;
			}

		}
		else{
			printf("File creation failed with status %d corresponding to errno %d\n",ret,errno);
			return -1;
		}
	}
	else{
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
			ret = desfire_write_data(fd,balancefilenumber,0x00,0,strlen(fp),fp);
			if(ret == 0){
				printf("fingerprint details written successfully with result %d\n",ret);
				return 0;
			}
			else{
				printf("fingerprint details written  with result %d corresponding to errno %d\n",ret,errno);
				return -1;
			}
		}
		else{
			printf("File creation failed with status %d corresponding to errno %d\n",ret,errno);
			return -1;
		}
	}
	else{
		return -1;
	}

}

int createtransactionfile(int fd,uint8_t keyno,char MF[3],char *authkey){
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
	transactionfile.file_size = 100;
	transactionfile.com_sett = 0x00;
	transactionfile.change_access_right_keyno = 0;
	transactionfile.read_and_write_access_right_keyno = 0;
	transactionfile.write_access_right_keyno = 0;
	transactionfile.read_access_right_keyno = 0;
	uint8_t transactionfilenumber = 2;

	char *txns = "173837&100*8cg4230pc1*10*151220181044";

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
			ret = desfire_write_data(fd,transactionfilenumber,0x00,0,strlen(txns),txns);
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
			printf("File creation failed with status %d corresponding to errno %d\n",ret,errno);
			return -1;
		}
	}
	else{
		return -1;
	}
}

int cardoperations(int operation,char *data , char * response[200] , BYTE * fingerprint[]){
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
	char * peronal_details;

	int timeout = 0;
	char uid[7];
	uint8_t x = 40;
	uint8_t *atslen = &x;
	char ats[40];
	int pollresult;
	char app0[3] = {0x00,0x00,0x00};
	char benapp[3] = {0x11,0x00,0x00};
	char authkey[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	uint8_t keyno = 0;
	int authstatus,ret,readops ;
	fd = mif_open("/dev/nfc");
		if(fd>=0){
			//open card success. Tap card and get poll status
			message_display_function(1,"","Please tap card", "", (char *)NULL);
			while(1){
				timeout++;
				pollresult =  desfire_poll(fd,0,uid,atslen,ats);
				if(pollresult == 0){
					printf("card found\n\n");
					message_display_function(1,"","card found", "please do not remove card", (char *)NULL);
					//personalize card here
					 ret = desfire_select_application(fd,app0);
					 authstatus = desfire_authenticate(fd,keyno,authkey);
					if(authstatus>=0){
						//select app and authenticate
						 ret = desfire_select_application(fd,benapp2.AID);
						 authstatus = desfire_authenticate(fd,keyno,authkey);
						 if(authstatus==0){
							 //select and authenticate application success. perform card operations
							 switch(operation){
							 case 1:
								readops=  readfile(fd,keyno,app0,benapp2.AID,authkey,response,fingerprint);
								printf("card read result::%d\n",readops);
								break;
							 default:
								 break;
							 }
						mif_close(fd);
						 }
						 else{
						   //printf personal details file not found
							message_display_function(1,"","card error", "unpersonalized card", (char *)NULL);
							sleep(1);
							kb_getkey();
							mif_close(fd);
							return -1;
						 }
					}
					else{
						//authentication failed
						printf("personalize card failed result %d\n",authstatus);
						sleep(3);
						message_display_function(1,"","card error", "authentication failed", (char *)NULL);
						sleep(1);
						kb_getkey();
						lcd_clean();
						mif_close(fd);
						return 0;
						break;
					}
					break;
				}
				//keep waiting for card until timeout
				//sleep(1);
				if(timeout>500){
					printf("Timeout achieved\n");
					message_display_function(1,"","NFC card not found", "", (char *)NULL);
					sleep(2);
					lcd_clean();
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
			mif_close(fd);
			return 0;
		}
}


int readfile(int fd,uint8_t keyno,char MF[3],char APP[3],char *authkey,char *response[200] , BYTE * fingerprint[]){
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
	char personaldetails[200];
	unsigned char fingerprints[400];
	char txndetails[100];



	ret = desfire_select_application(fd,MF);
	printf("select MF result %d corresponding to errno %d\n",ret,errno);
	authstatus = desfire_authenticate(fd,keyno,authkey);
	printf("Auth status %d optionally corresponding to errno %d\n",authstatus,errno);
	ret = desfire_select_application(fd,APP);
	if(ret==0){
		printf("app selected successfully\n");
		authstatus = desfire_authenticate(fd,keyno,authkey);
		printf("auth status %d corresponding to errno %d\n",authstatus,errno);
		//read personal details
		ret =  desfire_read_data(fd,0,0x00,0,200,&outlen,personaldetails);
		if(ret == 0){
			printf("data out ...\n");
			printf("personal details out %s\n",personaldetails);
			*(response+0) = personaldetails;
			//strcpy(*(response+0),personaldetails);
			//response[0] = personaldetails;
			//return 0;
		}
		else{
			printf("Failed to read card with status ret %d corresponding to errno %d\n",ret,errno);
			//return -1;
		}
		//read transaction file
		ret =  desfire_read_data(fd,2,0x00,0,100,&outlen,txndetails);
		if(ret == 0){
			printf("data out ...\n");
			printf("transactions out %s\n",txndetails);
			*(response+1) = txndetails;
			printf("<==========done============>\n");


		}
		else{
			printf("Failed to read card with status ret %d corresponding to errno %d\n",ret,errno);
			//return -1;
		}
		//read fingerprint file
		ret =  desfire_read_data(fd,1,0x00,0,400,&outlen,fingerprints);
		if(ret == 0){
			printf("data out ...\n");
			printf("fingerprints out %s\n",fingerprints);
			printf("<======dumping fingerprints====>\n");
			*(fingerprint+0) = fingerprints;
			printf("done.....\n");
		}
		else{
			printf("Failed to read card with status ret %d corresponding to errno %d\n",ret,errno);
			//return -1;
		}

	}
	else{
		return -1;
	}
}
