/*
 * beneficiary_transactions.c
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */

#include <stdlib.h>
#include "common_includes.h"
#include "beneficiary_transactions.h"
#include "../src/utilities/lcd.h"
#include "utilities/keyboart.h"
#include "utilities/cJSON.h"
#include "fingerprint_reader.h"
#include "../src1/desfire_test.h"

/*
 * Function to  register beneficiary
 */
void register_beneficiary(void);

void beneficiary_transactions(void )
{
	int selected = 0;
	const char  beneficiary_menu[][100] = {"Beneficiary Transaction","Beneficiary Registration","Balance Inquiry/Update","Mini-Statement" , "Exit"};
	while(1)
	{
		switch(lcd_menu("Beneficiary Transactions", beneficiary_menu, 5 ,selected))
		{
		case 0:
			//beneficiary_transaction_operations();
			printf("Beneficiary Transaction\n");
			break;
		case 1:
			register_beneficiary();
			break;
		case 2:
			printf("Balance Inquiry/Update\n");
			break;
		case 3:
			printf("Mini-Statement");
			break;
		case 4 :
			return;
			break;
		}
	}
}

//int argc, char *argv[]
void register_beneficiary(void){
	int ret;
	int change_made =0;
	int x= 0;

	cJSON * txToPosted=cJSON_CreateObject();
	char getCharacters[40];
	char getCharacters1[40];
	char name1[30];
	char name[100];
	char details[7][40];

	const char gender[][100] = {
			"Male",
			"Female"
	};
	const char document_type[][100] = {
			//
			"National ID",
			"Passport",
			"Alien ID",
			"NGO Card"
	};
	//BYTE fp23;
	int selected = 0;


	//Read card // replace with Kims Functions


	strcpy(name1, "");

	//Innitialize the keywords
	strcpy(details[0], "First Name");
	strcpy(details[1], "Last Name");
	strcpy(details[2], "Date of Birth") ;
	strcpy(details[3], "Gender") ;
	strcpy(details[4], "Document Type");
	strcpy(details[5], "Document Number");
	strcpy(details[6], "Address Information") ;

	//Get  Beneficiary details
	lcd_clean();
	for( x=0; x<4;x++)
	{
		change_made =0;

		//Get Gender
		if(strcpy(details[x], "Gender")  )
		{
			while (selected>=0){

				selected= lcd_menu("Please select Gender", gender, sizeof(gender)/100, selected);

				printf("Selected on %d\n", selected);
				if(selected > -1 && selected < sizeof(gender)/100 )
				{
					//strcpy(myBenf.gender, "Male" );
					cJSON_AddStringToObject(txToPosted,details[x],gender[selected]);
					break;
				}
				else if(selected == -1)
				{
					return;
				}
				else
				{
					message_display_function(1,"","Invalid Selection", "Please select  again.", (char *)NULL);
					kb_getkey();
				}
			}
		}

		//Get Document Type
		if(strcpy(details[x], "Document Type")  )
		{
			while (selected>=0){

				selected= lcd_menu("Please select Document Type", document_type, sizeof(gender)/100, selected);

				printf("Selected on %d\n", selected);
				if(selected == 0 )
				{
					//strcpy(myBenf.documenttype, "Male" );
					cJSON_AddStringToObject(txToPosted,"Gender","Male");
					break;
				}
				else if(selected == 1)
				{
					//strcpy(myBenf.gender, "Female" );
					cJSON_AddStringToObject(txToPosted,"Gender","Female");
					break;
				}
				else if(selected == -1)
				{
					return;
				}
				else
				{
					message_display_function(1,"","Invalid Selection", "Please select  again.", (char *)NULL);
					kb_getkey();
				}
			}
		}


		else{
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, details[x], name1,"Register Beneficiary", 0);
			printf("Ret on %d", ret);
			if( strlen(getCharacters)>0 && ret!=-1)
			{
				change_made =1;
				if(x==0)
				{
					//strcpy(myBenf.firstname, getCharacters );
					cJSON_AddStringToObject(txToPosted,"firstname",getCharacters);
				}
				else if(x==1)
				{
					//strcpy(myBenf.lastname, getCharacters );
					cJSON_AddStringToObject(txToPosted,"lastname",getCharacters);
				}
				else if(x==2)
				{
					//strcpy(myBenf.dateofbirth, getCharacters );
					cJSON_AddStringToObject(txToPosted,"dateofbirth",getCharacters);
				}


			}

			else if(ret==-1  )
			{

				//printf("Ret on ")
				return;
				//&& strlen(getCharacters)>0
			}

		}
	}

/*	printf("%s\n",myBenf.firstname);
	printf("%s\n",myBenf.lastname);
	printf("%s\n",myBenf.dateofbirth);
	printf("%s\n",myBenf.gender);*/


	//Get  Beneficiary fingerprint
	if(fplib_test(0))
	{
		//printf("You know what here it is\n " );
		//printf(">>>>>>>JSON %s\n", cJSON_Print(txToPosted));

		int w=0;
		while(w<400)
		{
			//printf( "%02X",myBenf.fingerprint[w]);
			//strcpy(m[w],"%02X",imageBuffer1[w]);
			w++;
		}

		if(createApplicationAndFileAndWriteData(cJSON_Print(txToPosted),"myBenf.fingerprint"))
		{

			//printf("You know what here it is\n \n %s \n" ,  str);

			//Validate Beneficiary online
			message_display_function(1,"","Registration Successful", "Please note that  your card has been successfully created. Thank you and welcome again", (char *)NULL);
			kb_getkey();
			//Write Beneficiary to  card card
			//end
		}
		else
		{
			message_display_function(1,"","EMV card Error ", "Error reading card, please check your card reader and place your at  proximity  and try  again", (char *)NULL);
			kb_getkey();
		}
	}
	else
	{
		return;
	}



}



void register_ben(int argc, char *argv[]){
	int key;
	message_display_function(1, "","Beneficiary Registration" ,  "Please tap the NFC card and press enter key");
	key=kb_getkey();
	if(key==DIKS_ENTER){

		//register_ben();
	}
	else
		return;
}
