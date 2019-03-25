#include "keyboart.h"

#include "ctype.h"
#include "unistd.h"
/*
#include "../../src1/general_funtions_and_varaibles.h"
#include "../../src1/jsonread.h"
#include "../../src1/desfire_test.h"
*/
#include <pthread.h>

#include <stdarg.h>		/* for va_ stuff */

#include <sys/time.h>

#include "visualstring.h"
#include "cJSON.h"
#include "lcd.h"
#include "postslib_extern.h"
#include "common_functions.h"
#include "../device_management.h"

static char *pszAlphaTbl[10]   = { "0.,*# ~`!@$%^&-+=(){}[]<>_|\\:;\"\'?/",
		"1QZqz ", "2ABCabc", "3DEFdef", "4GHIghi", "5JKLjkl", "6MNOmno",
		"7PRSprs", "8TUVtuv", "9WXYwxy", };
static char *pszAlphaSmall[10] = { "0. ,*#~`!@$%^&-+=(){}[]<>_|\\:;\"\'?/",
		"qz ", "abc", "def", "ghi", "jkl", "mno",
		"prs", "tuv", "wxy", };
static char *pszAlphaCaps[10] = { "0. ,*#~`!@$%^&-+=(){}[]<>_|\\:;\"\'?/",
		"QZ", "ABC", "DEF", "GHI", "JKL", "MNO",
		"PRS", "TUV", "WXY", };
int KEY_TIMEOUT = 0;
int lastypedKey = 0;
time_t start, current;

void key_board_demo1(void);
void organise_screen_on_input_one(char * charact,int iLen, char * szWorkBuff, char* mode_in);
void organise_screen_on_input_login(char * charact,char * charact2,int iLen,  char * txt1, char * txt2, char* mode_in, int location , int display_string_below, char * title);
void translate_long_strings_and_print(char *str);
//ContactlessSmartCard_Demo();


/*
void register_ben(int argc, char *argv[]){
	int ret;
	int change_made =0;
	int x= 0;

	cJSON * txToPosted=cJSON_CreateObject();
	char getCharacters[40];
	char getCharacters1[40];
	char name1[30];
	char name[100];
	char details[3][40];

	const char menu1[][100] = {
			"Male",
			"Female"
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

	//Get  Beneficiary details
	lcd_clean();
	for( x=0; x<4;x++)
	{
		change_made =0;

		if(x==3)
		{
			while (selected>=0){
				selected= lcd_menu("Please select gender", menu1, sizeof(menu1)/100, selected);
				printf("Selected on %d\n", selected);
				if(selected == 0 )
				{
					strcpy(myBenf.gender, "Male" );
					cJSON_AddStringToObject(txToPosted,"Gender","Male");
					break;
				}
				else if(selected == 1)
				{
					strcpy(myBenf.gender, "Female" );
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
					strcpy(myBenf.firstname, getCharacters );
					cJSON_AddStringToObject(txToPosted,"firstname",getCharacters);
				}
				else if(x==1)
				{
					strcpy(myBenf.lastname, getCharacters );
					cJSON_AddStringToObject(txToPosted,"lastname",getCharacters);
				}
				else if(x==2)
				{
					strcpy(myBenf.dateofbirth, getCharacters );
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

	printf("%s\n",myBenf.firstname);
	printf("%s\n",myBenf.lastname);
	printf("%s\n",myBenf.dateofbirth);
	printf("%s\n",myBenf.gender);


	//Get  Beneficiary fingerprint
	if(fplib_test(0))
	{
		//printf("You know what here it is\n " );
		//printf(">>>>>>>JSON %s\n", cJSON_Print(txToPosted));

		int w=0;
		while(w<400)
		{
			printf( "%02X",myBenf.fingerprint[w]);
			//strcpy(m[w],"%02X",imageBuffer1[w]);
			w++;
		}

		if(createApplicationAndFileAndWriteData(cJSON_Print(txToPosted),myBenf.fingerprint))
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

*/
void key_board_demo1(void) {
	int cancel_times = 0;
	int key = 0, key_pre = 0;
	;
	lcd_clean();
	lcd_printf(ALG_CENTER, "PLEASE PRESS KEY");
	lcd_printf(ALG_CENTER, "DOUBLE PRESS CANCEL TO EXIT");
	while (cancel_times < 2) {

		lcd_flip();
		key = kb_getkey();
		lcd_clean();
		lcd_printf(ALG_CENTER, "PLEASE PRESS KEY");
		lcd_printf(ALG_CENTER, "DOUBLE PRESS CANCEL TO EXIT");
		lcd_printf_ex(ALG_CENTER, 100, "KEY CODE = %04X", key);
		if (key == 0x1B) {
			if (key_pre == key)
				cancel_times++;
			else {
				key_pre = key;
				cancel_times = 1;
			}
		} else {
			cancel_times = 0;
			key_pre = key;
		}
	}

}
void key_board_demo(void) {
	int cancel_times = 0;
	int key = 0, key_pre = 0;
	;
	lcd_clean();
	lcd_printf(ALG_CENTER, "PLEASE PRESS KEY");
	lcd_printf(ALG_CENTER, "DOUBLE PRESS CANCEL TO EXIT");
	while (cancel_times < 2) {

		lcd_flip();
		key = kb_getkey();
		lcd_clean();
		lcd_printf(ALG_CENTER, "PLEASE PRESS KEY");
		lcd_printf(ALG_CENTER, "DOUBLE PRESS CANCEL TO EXIT");
		lcd_printf_ex(ALG_CENTER, 100, "KEY CODE = %04X", key);
		if (key == 0x1B) {
			if (key_pre == key)
				cancel_times++;
			else {
				key_pre = key;
				cancel_times = 1;
			}
		} else {
			cancel_times = 0;
			key_pre = key;
		}
	}

}
void MapChar2Index(char ch, int *piRow, int *piCol) {
	int i;
	char *p;
	for (i = 0; i < sizeof(pszAlphaTbl) / sizeof(pszAlphaTbl[0]); i++) {
		for (p = pszAlphaTbl[i]; *p; p++) {
			if (*p == toupper(ch)) {
				*piRow = i;
				*piCol = p - pszAlphaTbl[i];
				break;
			}
		}
	}
}



/*void* threader_keyboard(void *arg)
{
	const int RUNTIME = 3;

	int spaces = 0;
	start = time(NULL);
	do {
		current = time(NULL);
		if (difftime(current, start) == RUNTIME) {
			start = time(NULL);
			KEY_TIMEOUT = 1;
		}
	} while (1);
	return NULL;
}*/
/******************************************************
 FUNC:kb_getString

 Out put the string , press alpha to change to letter

 PARAM:
 ucMode :
 #define NUM_IN			0x01	// only number input
 #define ALPHA_IN		0x02	// number&letter
 #define PASS_IN			0x04	// Ciphertext input ******
 #define ECHO_IN		0x10	// clear input
 #define CARRY_IN		0x08	// with default input value
 #define HEX_IN          0x20    // HEX input
 #define AUTO_FONT		0x80	// RFU
 ucMinlen:
 the mimimum length of data
 ucMaxlen:
 the maximum length pf data
 pszOut:
 out put value


 ******************************************************/
char **  increament_pointersize_and_copy(char ** original_pointer ,int i ,char * str){
	original_pointer = realloc(original_pointer, (i + 1) * sizeof(char *));
	original_pointer[i] = malloc(strlen(str) + 1);
	strcpy(original_pointer[i], str);
	return original_pointer;
}


int kb_getString(uint8_t ucMode, uint8_t ucMinlen, uint8_t ucMaxlen,
		char *pszOut, const char *ch, char *charact) {
	uint8_t bClearInData, szWorkBuff[1024 + 1];  //512 the max length
	int iLen, iIndex, iLastKey, iKeyValue, iCnt ;
	uint32_t uiOldTime, uiNewTime, ucTimeOut;
	uint8_t szDispBuff[1024 + 1];
	iIndex = iLastKey = 0;
	int charcount;
	char * masked_char[10];
	char mode_in[10];
	int get_settings = 0;
	char x_x[40];
	/*	int scrolling = 0;
			pthread_t tid;
			int err;
			KEY_TIMEOUT = 0;
			err = pthread_create(&(tid), NULL, &threader_keyboard, NULL);*/

	if (ucMode & ALPHA_IN)
		strcpy(mode_in, "NUM :OFF");
	if (ucMode & NUM_IN || ucMode & PASS_IN )
		strcpy(mode_in, "NUM :ON");
	if ((ucMode & CARRY_IN) || (ucMode & ECHO_IN)) {
		sprintf((char *) szWorkBuff, "%.*s", 1024, pszOut);
		if (ucMode & HEX_IN) {
			iLen = strlen((char *) szWorkBuff);
			for (iCnt = 0; iCnt < iLen; iCnt++) {
				if ((('0' <= szWorkBuff[iCnt]) && (szWorkBuff[iCnt] <= '9'))
						|| (('A' <= szWorkBuff[iCnt])
								&& (szWorkBuff[iCnt] <= 'F'))
								|| (('a' <= szWorkBuff[iCnt])
										&& (szWorkBuff[iCnt] <= 'f'))) {

					continue;
				} else {
					memset(szWorkBuff, 0, sizeof(szWorkBuff));
				}
			}

		}


		if (ucMode & PASS_IN) {

			iLen = strlen((char *) szWorkBuff);
			for (iCnt = 0; iCnt < iLen; iCnt++) {
				if ((('0' <= szWorkBuff[iCnt]) && (szWorkBuff[iCnt] <= '9'))
						|| (('A' <= szWorkBuff[iCnt])
								&& (szWorkBuff[iCnt] <= 'F'))
								|| (('a' <= szWorkBuff[iCnt])
										&& (szWorkBuff[iCnt] <= 'f'))) {

					memset(szDispBuff, 0, sizeof(szDispBuff));
					memset(szDispBuff, '*', iLen);
					lcd_printf(ALG_LEFT, "%s", szDispBuff);
					continue;
				} else
					lcd_printf(ALG_LEFT, "%s", szWorkBuff);
			}
		}

	} else {
		memset(szWorkBuff, 0, sizeof(szWorkBuff));
	}

	iLen = strlen((char *) szWorkBuff);
	if (iLen > 0) {
		MapChar2Index(szWorkBuff[iLen - 1], &iLastKey, &iIndex);
	} else {
		iLastKey = -1;
		iIndex = 0;
	}

	bClearInData = (ucMode & ECHO_IN);
	while (1) {
		uiOldTime = sysGetTimerCount();
		while (1) {
			lcd_clean();
			organise_screen_on_input_one(charact,iLen, szWorkBuff,mode_in);
			//strcpy(x_x,szWorkBuff);

			//Display_login_screen("Vihiga County","Please enter Login details","Username","Password",x_x,"");


			iKeyValue = kb_getkey();


			/*			//
			printf("Last : %d",iKeyValue);
			printf("cyrrent : %d",lastypedKey);
			printf("TIME : %d",KEY_TIMEOUT);


			if (iKeyValue == lastypedKey && KEY_TIMEOUT == 0 )
			{
				//get key
				scrolling =1;


			}
			else
			{
				//Next key
				scrolling = 0;
			}


			start = time(NULL);
			current = time(NULL);
			KEY_TIMEOUT = 0;
			lastypedKey =iKeyValue;*/
			if (iKeyValue != DIKS_ENTER)
				get_settings = 0;
			else {
				if(get_settings)
					change_configuration(-1);
			}

			if (iKeyValue != 0)
			{
				break;
			}

			uiNewTime = sysGetTimerCount();
			if (uiOldTime + ucTimeOut * 1000 < uiNewTime)
			{
				break;
			}

			//			DrawRect(120,62,127,63);
			//usleep(50 * 1000);
		}
		if (iKeyValue == DIKS_ESCAPE) {
			if(!login_successful)
			{
				sprintf((char *) pszOut, "%s", "CANCEL_PRESSED");
				break;
			}

			else if(login_successful)
			{
				sprintf((char *) pszOut, "%s", "");
				break;
			}

		} else if (iKeyValue == DIKS_ENTER) {
			bClearInData = 0;
			if (iLen < (int) ucMinlen) {
				continue;
			}
			sprintf((char *) pszOut, "%s", szWorkBuff);
			break;
		}
		else if (iKeyValue == DIKS_CONTROL) {
			printf("Login: login_successful %d \n ", login_successful);
			if(!login_successful)
				get_settings = 1;
			if (!((ucMode & ALPHA_IN) || (ucMode & HEX_IN))) {
				continue;
			}

			iIndex = 0;
			//printf("Hello world\n");
			if(strcmp(mode_in, "Caps:ON")==0)
				strcpy(mode_in, "Caps:OFF");
			else if(strcmp(mode_in, "Caps:OFF")==0)
				strcpy(mode_in, "NUM :OFF");
			else
				strcpy(mode_in, "Caps:ON");
			continue;
		}
		else if(iKeyValue == DIKS_ALT){
			printf("Login: login_successful %d \n ", login_successful);
			if(!login_successful)
				get_settings = 1;

		}
		else if (iKeyValue >= DIKS_0 && iKeyValue <= DIKS_9 ) {
			if (bClearInData) {	// clear in buffer
				szWorkBuff[0] = 0;
				iLen = 0;
				iLastKey = -1;
				bClearInData = 0;
			}
			// save key in data
			if (iLen < (int) ucMaxlen) {
				iLastKey = iKeyValue - DIKS_0;
				if(strcmp(mode_in, "Caps:ON")==0)
					szWorkBuff[iLen++] = pszAlphaCaps[iLastKey][0];
				else if(strcmp(mode_in, "Caps:OFF")==0)
					szWorkBuff[iLen++] = pszAlphaSmall[iLastKey][0];
				else
					szWorkBuff[iLen++] = iKeyValue;

				//szWorkBuff[iLen++] = iKeyValue;
				szWorkBuff[iLen] = 0;

				iIndex = 0;
				//printf("Ikey : %d\n",iKeyValue);
			}
		} else if (iKeyValue == DIKS_CLEAR) {
			szWorkBuff[0] = 0;
			iLen = 0;
			iLastKey = -1;
		} else if (iKeyValue == DIKS_BACKSPACE) // backspace
		{
			bClearInData = 0;
			if (iLen <= 0) {
				continue;
			}
			szWorkBuff[--iLen] = 0;
			if (iLen > 0) {
				MapChar2Index(szWorkBuff[iLen - 1], &iLastKey, &iIndex);
			} else {
				iLastKey = -1;
				iIndex = 0;
			}
		} else if (iKeyValue == DIKS_SHIFT ) {
			bClearInData = 0;
			if (!((ucMode & ALPHA_IN) || (ucMode & HEX_IN)) || iLastKey < 0|| iLen < 1) {
				continue;
			}

			if(strcmp(mode_in, "Caps:OFF")==0)
			{
				iIndex = (iIndex + 1) % strlen(pszAlphaSmall[iLastKey]);
				if (ucMode & HEX_IN) {
					if ((('0' <= pszAlphaSmall[iLastKey][iIndex])
							&& (pszAlphaSmall[iLastKey][iIndex] <= '9'))
							|| (('A' <= pszAlphaSmall[iLastKey][iIndex])
									&& (pszAlphaSmall[iLastKey][iIndex] <= 'F'))
									|| (('a' <= pszAlphaSmall[iLastKey][iIndex])
											&& (pszAlphaSmall[iLastKey][iIndex] <= 'f'))) {
						szWorkBuff[iLen - 1] = pszAlphaSmall[iLastKey][iIndex];
					} else {
						continue;
					}
				} else {
					szWorkBuff[iLen - 1] = pszAlphaSmall[iLastKey][iIndex];
				}
			}
			else if(strcmp(mode_in, "Caps:ON")==0)
			{
				iIndex = (iIndex + 1) % strlen(pszAlphaCaps[iLastKey]);
				if (ucMode & HEX_IN) {
					if ((('0' <= pszAlphaCaps[iLastKey][iIndex])
							&& (pszAlphaCaps[iLastKey][iIndex] <= '9'))
							|| (('A' <= pszAlphaCaps[iLastKey][iIndex])
									&& (pszAlphaCaps[iLastKey][iIndex] <= 'F'))
									|| (('a' <= pszAlphaCaps[iLastKey][iIndex])
											&& (pszAlphaCaps[iLastKey][iIndex] <= 'f'))) {
						szWorkBuff[iLen - 1] = pszAlphaCaps[iLastKey][iIndex];
					} else {
						continue;
					}
				} else {
					szWorkBuff[iLen - 1] = pszAlphaCaps[iLastKey][iIndex];
				}

			}


			//


			else{

				iIndex = (iIndex + 1) % strlen(pszAlphaTbl[iLastKey]);
				if (ucMode & HEX_IN) {
					if ((('0' <= pszAlphaTbl[iLastKey][iIndex])
							&& (pszAlphaTbl[iLastKey][iIndex] <= '9'))
							|| (('A' <= pszAlphaTbl[iLastKey][iIndex])
									&& (pszAlphaTbl[iLastKey][iIndex] <= 'F'))
									|| (('a' <= pszAlphaTbl[iLastKey][iIndex])
											&& (pszAlphaTbl[iLastKey][iIndex] <= 'f'))) {
						szWorkBuff[iLen - 1] = pszAlphaTbl[iLastKey][iIndex];
					} else {
						continue;
					}
				} else {
					szWorkBuff[iLen - 1] = pszAlphaTbl[iLastKey][iIndex];
				}

			}

		}
	}
	memset(charact, 0, sizeof(charact));
	flag_getting_pwd=0;
	return 0;

}
void organise_screen_on_input_one(char * charact,int iLen, char * szWorkBuff, char* mode_in)
{
	char masked_char[50];
	lcd_clean();
	lcd_printf(ALG_CENTER, " REVENUE SYSTEM");
	lcd_printf(ALG_LEFT, " ");
	lcd_printf(ALG_CENTER, charact);
	lcd_printf(ALG_LEFT, " ");
	if(!flag_new_line)
		lcd_printf(ALG_LEFT, " ");

	flag_new_line=0;

	lcd_draw_rectangle(40, 140, 250, 40);
	if(flag_getting_pwd)
	{
		char stars[20];
		strcpy(stars ,"**********" );
		if(iLen>0)
		{
			snprintf(masked_char, iLen+1,stars);
			lcd_printf(ALG_CENTER, "%s", masked_char);
		}
		else
			lcd_printf(ALG_CENTER, "%s", szWorkBuff);

	}
	else
		if(strlen(szWorkBuff)==0)
			lcd_printf(ALG_CENTER,"");
		else
			lcd_printf(ALG_CENTER, "%s", szWorkBuff);
	if(iLen==0)
		lcd_printf(ALG_CENTER, "");
	lcd_printf(ALG_LEFT, mode_in);

	lcd_flip();
}



int kb_getStringtwo(uint8_t ucMode,uint8_t ucMode1,  uint8_t ucMinlen, uint8_t ucMaxlen, char *pszOut,char *pszOut1 ,  const char *ch, char *charact, char *charact2 ,char * title , int display_string_below) {
	uint8_t bClearInData , szWorkBuff[1024 + 1]; //512 the max length
	int iLen, iIndex, iLastKey, iKeyValue,latestKey ,  iCnt ;
	uint32_t uiOldTime, uiNewTime, ucTimeOut;
	uint8_t szDispBuff[1024 + 1];
	int timeexpired = 0 , num_expired = 0 ,dot_exist = 0;
	//uint8_t test;q

	int remaining = 0;
	char mode_in[10];
	char * text_to_display;
	int get_settings = 0;
	int x =0;

	donnot_get_key = 0;
	iIndex = iLastKey = latestKey = 0;
	flag_getting_pwd1=0;
	flag_getting_pwd2=0;
	strcpy(pszOut,"");
	if(display_string_below)
	{
		printf("Text : %s\n",pszOut1);
		if(strlen(pszOut1)>0)
		{
			text_to_display = malloc(strlen(pszOut1));
			strcpy(text_to_display,pszOut1);
		}
		else
		{
			text_to_display = malloc(10);
			strcpy(text_to_display,"NULL");
		}
	}


	strcpy(pszOut1,"");
	/*	int scrolling = 0;
			pthread_t tid;
			int err;
			KEY_TIMEOUT = 0;
			err = pthread_create(&(tid), NULL, &threader_keyboard, NULL);*/

	if (ucMode & PASS_IN )
		flag_getting_pwd1 = 1;
	if (ucMode1 & PASS_IN )
		flag_getting_pwd2 = 1;


	for( x = 0;x < 2; x++)
	{
		if(x == 1){
			dot_exist = 0;
			num_expired = 0;
		}
		strcpy(( char *)szWorkBuff,"");
		iIndex = iLastKey = 0;

		//Exit for only one input
		if(x==1 && (strlen(charact2)==0 || display_string_below==1))
		{
			donnot_get_key =1;
			return 0;
		}

		printf("X: %d\n", x);
		if(x>0)
			ucMode = ucMode1;
		if ((ucMode & ALPHA_IN) || (ucMode & CARRY_IN))
			strcpy(mode_in, "Abc123");
		if (ucMode & NUM_IN)
			strcpy(mode_in, "123");
		if (ucMode & PASS_IN )
			strcpy(mode_in, "***");
		if ((ucMode & CARRY_IN) || (ucMode & ECHO_IN)) {
			if(x==0)
				sprintf((char *) szWorkBuff, "%.*s", 1024, pszOut);
			else if(x==1)
				sprintf((char *) szWorkBuff, "%.*s", 1024, pszOut1);
			if (ucMode & HEX_IN) {
				iLen = strlen((char *) szWorkBuff);
				for (iCnt = 0; iCnt < iLen; iCnt++) {
					if ((('0' <= szWorkBuff[iCnt]) && (szWorkBuff[iCnt] <= '9'))
							|| (('A' <= szWorkBuff[iCnt])
									&& (szWorkBuff[iCnt] <= 'F'))
									|| (('a' <= szWorkBuff[iCnt])
											&& (szWorkBuff[iCnt] <= 'f'))) {

						continue;
					} else {
						memset(szWorkBuff, 0, sizeof(szWorkBuff));
					}
				}

			}


			if (ucMode & PASS_IN) {

				iLen = strlen((char *) szWorkBuff);
				for (iCnt = 0; iCnt < iLen; iCnt++) {
					if ((('0' <= szWorkBuff[iCnt]) && (szWorkBuff[iCnt] <= '9'))
							|| (('A' <= szWorkBuff[iCnt])
									&& (szWorkBuff[iCnt] <= 'F'))
									|| (('a' <= szWorkBuff[iCnt])
											&& (szWorkBuff[iCnt] <= 'f'))) {

						memset(szDispBuff, 0, sizeof(szDispBuff));
						memset(szDispBuff, '*', iLen);
						//					lcd_printf(ALG_LEFT, "%s", szDispBuff);
						continue;
					} else
						lcd_printf(ALG_LEFT, "%s", szWorkBuff);
				}
			}

		} else {
			memset(szWorkBuff, 0, sizeof(szWorkBuff));
		}

		iLen = strlen((char *) szWorkBuff);
		if (iLen > 0) {
			MapChar2Index(szWorkBuff[iLen - 1], &iLastKey, &iIndex);
		} else {
			iLastKey = -1;
			iIndex = 0;
		}

		bClearInData = (ucMode & ECHO_IN);
		timeexpired = 1;
		while (1) {

			while (1) {
				lcd_clean();
				if(x ==0)
				{
					if(!display_string_below)
						organise_screen_on_input_login(charact,charact2,iLen, ( char *)szWorkBuff,( char *)pszOut1,mode_in,1,display_string_below , title);
					else
						organise_screen_on_input_login(charact,charact2,iLen, ( char *)szWorkBuff,text_to_display,mode_in,1,display_string_below ,title);
				}

				if(x==1)
				{
					organise_screen_on_input_login(charact,charact2,iLen, pszOut,( char *)szWorkBuff,mode_in,2, display_string_below ,title);
				}

				iKeyValue = kb_getkey();
				if (!((ucMode & ALPHA_IN) || (ucMode & HEX_IN) || (ucMode & CARRY_IN)))
				{

					timeexpired = 1;
					if(iKeyValue == latestKey && iKeyValue >= DIKS_0 && iKeyValue <= DIKS_9 )
					{
						if(((sysGetTimerCount() - uiOldTime )>1000))
						{
							num_expired =1;
						}

						else
						{
							num_expired =0;

						}
					}
					else
					{
						num_expired = 1;
					}
					if(dot_exist)
						num_expired = 1;
				}
				else
				{
					if(iKeyValue == latestKey && iKeyValue >= DIKS_0 && iKeyValue <= DIKS_9 )
					{
						if(((sysGetTimerCount() - uiOldTime )>1000))
						{
							timeexpired =1;
						}

						else
						{
							timeexpired =0;

						}
					}
					else
					{
						timeexpired = 1;
					}
				}
				uiOldTime = sysGetTimerCount();

				if (iKeyValue != DIKS_ENTER)
					get_settings = 0;
				else {
					if(get_settings)
						change_configuration(-1);
				}

				if (iKeyValue != 0)
				{
					break;
				}

				uiNewTime = sysGetTimerCount();
				if (uiOldTime + ucTimeOut * 1000 < uiNewTime)
				{
					break;
				}
			}
			if (iKeyValue == DIKS_ESCAPE) {
				sprintf((char *) pszOut, "%s", "");
				sprintf((char *) pszOut1, "%s", "");
				/*			label_one = 0;
			label_two = 0;*/
				return -1;

			} else if (iKeyValue == DIKS_ENTER) {
				bClearInData = 0;
				if (iLen < (int) ucMinlen) {
					continue;
				}
				//printf("Value : %s\n",szWorkBuff);
				if(x==0)

					sprintf((char *) pszOut, "%s", szWorkBuff);
				else if(x==1)
					sprintf((char *) pszOut1, "%s", szWorkBuff);
				break;


			}
			else if (iKeyValue == DIKS_CONTROL) {
				if (!((ucMode & ALPHA_IN) || (ucMode & HEX_IN) || (ucMode & CARRY_IN))) {
					continue;
				}

				iIndex = 0;
				//printf("Hello world\n");
				if(strcmp(mode_in, "ABC")==0)
					strcpy(mode_in, "abc");
				else if(strcmp(mode_in, "abc")==0)
					strcpy(mode_in, "123Abc");
				else
					strcpy(mode_in, "ABC");
				continue;
			}
			else if(iKeyValue == DIKS_ALT){
				printf("Login: login_successful %d \n ", login_successful);
				/*			 if(!login_successful)
			 get_settings = 1;*/
				if(!login_successful)
					get_settings = 1;

			}
			else if ((iKeyValue >= DIKS_0 && iKeyValue <= DIKS_9 && timeexpired) ) {
				if (bClearInData) {	// clear in buffer
					szWorkBuff[0] = 0;
					iLen = 0;
					iLastKey = -1;
					bClearInData = 0;
				}
				// save key in data
				if (iLen < (int) ucMaxlen) {
					iLastKey = iKeyValue - DIKS_0;
					if(strcmp(mode_in, "ABC")==0)
						szWorkBuff[iLen++] = pszAlphaCaps[iLastKey][0];
					else if(strcmp(mode_in, "abc")==0)
						szWorkBuff[iLen++] = pszAlphaSmall[iLastKey][0];
					else
					{
						if((ucMode & NUM_IN) && remaining < 0 && dot_exist)
						{}
						else
							szWorkBuff[iLen++] = iKeyValue;
					}

					//szWorkBuff[iLen++] = iKeyValue;
					szWorkBuff[iLen] = 0;

					iIndex = 0;
					//printf("Ikey : %d\n",iKeyValue);
				}
			} else if (iKeyValue == DIKS_CLEAR) {
				szWorkBuff[0] = 0;
				iLen = 0;
				iLastKey = -1;
				dot_exist = 0;
			} else if (iKeyValue == DIKS_BACKSPACE) // backspace
			{
				bClearInData = 0;
				if (iLen <= 0) {
					continue;
				}
				if((ucMode & NUM_IN))
				{
					printf("Delete Key : %d\n",remaining);
					if(dot_exist)
						remaining++;
					if(szWorkBuff[iLen -1] == '.' )
						dot_exist = 0;

				}
				szWorkBuff[--iLen] = 0;
				if (iLen > 0) {
					MapChar2Index(szWorkBuff[iLen - 1], &iLastKey, &iIndex);
				} else {
					iLastKey = -1;
					iIndex = 0;
				}

			} else if (!timeexpired) {
				bClearInData = 0;

				/*			printf("This is it\n");
			if((ucMode & NUM_IN)){
				printf("This is it 2\n");
				if(iLastKey == '0')
				{
				 szWorkBuff[iLen - 1] = '.';
				 iLastKey = '.';
				}
				if(iLastKey == '.')
				{
				 szWorkBuff[iLen - 1] = '0';
				 iLastKey = '0';
				}
			}*/
				if (!((ucMode & ALPHA_IN) || (ucMode & CARRY_IN) || (ucMode & HEX_IN)) || iLastKey < 0|| iLen < 1 ) {
					continue;
				}


				if(strcmp(mode_in, "abc")==0)
				{
					iIndex = (iIndex + 1) % strlen(pszAlphaSmall[iLastKey]);
					if (ucMode & HEX_IN) {
						if ((('0' <= pszAlphaSmall[iLastKey][iIndex])
								&& (pszAlphaSmall[iLastKey][iIndex] <= '9'))
								|| (('A' <= pszAlphaSmall[iLastKey][iIndex])
										&& (pszAlphaSmall[iLastKey][iIndex] <= 'F'))
										|| (('a' <= pszAlphaSmall[iLastKey][iIndex])
												&& (pszAlphaSmall[iLastKey][iIndex] <= 'f'))) {
							szWorkBuff[iLen - 1] = pszAlphaSmall[iLastKey][iIndex];
						} else {
							continue;
						}
					} else {
						szWorkBuff[iLen - 1] = pszAlphaSmall[iLastKey][iIndex];
					}
				}



				else if(strcmp(mode_in, "ABC")==0)
				{
					iIndex = (iIndex + 1) % strlen(pszAlphaCaps[iLastKey]);
					if (ucMode & HEX_IN) {
						if ((('0' <= pszAlphaCaps[iLastKey][iIndex])
								&& (pszAlphaCaps[iLastKey][iIndex] <= '9'))
								|| (('A' <= pszAlphaCaps[iLastKey][iIndex])
										&& (pszAlphaCaps[iLastKey][iIndex] <= 'F'))
										|| (('a' <= pszAlphaCaps[iLastKey][iIndex])
												&& (pszAlphaCaps[iLastKey][iIndex] <= 'f'))) {
							szWorkBuff[iLen - 1] = pszAlphaCaps[iLastKey][iIndex];
						} else {
							continue;
						}
					} else {
						szWorkBuff[iLen - 1] = pszAlphaCaps[iLastKey][iIndex];
					}

				}
				//


				else{

					iIndex = (iIndex + 1) % strlen(pszAlphaTbl[iLastKey]);
					if (ucMode & HEX_IN) {
						if ((('0' <= pszAlphaTbl[iLastKey][iIndex])
								&& (pszAlphaTbl[iLastKey][iIndex] <= '9'))
								|| (('A' <= pszAlphaTbl[iLastKey][iIndex])
										&& (pszAlphaTbl[iLastKey][iIndex] <= 'F'))
										|| (('a' <= pszAlphaTbl[iLastKey][iIndex])
												&& (pszAlphaTbl[iLastKey][iIndex] <= 'f'))) {
							szWorkBuff[iLen - 1] = pszAlphaTbl[iLastKey][iIndex];
						} else {
							continue;
						}
					} else {
						szWorkBuff[iLen - 1] = pszAlphaTbl[iLastKey][iIndex];
					}

				}

			}
			if((iKeyValue ==  DIKS_0) && (ucMode & NUM_IN) && ( latestKey == DIKS_0) )
			{
				if(!num_expired)
				{
					if(iLen>0)
					{
						szWorkBuff[--iLen] = 0;
					}
					else
					{

					}
					if(szWorkBuff[iLen - 1] == '0')
					{
						szWorkBuff[iLen - 1] = '.';
						dot_exist = 1;
						remaining = 2;
					}
					else if(szWorkBuff[iLen - 1] == '.')
					{
						szWorkBuff[iLen - 1] = '0';
						dot_exist = 0;
						remaining = 0;
					}

				}
			}
			if( (ucMode & NUM_IN) &&  dot_exist && iKeyValue >= DIKS_0 && iKeyValue <= DIKS_9)
			{
				printf("Subract : %d\n",remaining);
				if(remaining>=0)
					remaining -- ;
			}
			latestKey = iKeyValue;
		}

		memset(szWorkBuff,0,sizeof(szWorkBuff));
		if(x == 0)
			flag_getting_pwd1=0;
		if( x ==1)
			flag_getting_pwd2 =0;

	}

	/*	label_one = 0;
	label_two = 0;*/
	donnot_get_key =1;
	return 0;

}

void message_display_function(int image_present , char * imageurl , char * title ,char * message , ...)
{
	/*	if( started_program)
		str = translate_name_by_key(str1);
	else
		str = str1;*/
	//char * part_translated;
	va_list argp;
	char *part , *final_msg;
	char  mymsg[2000];
	lcd_clean();
	define_y_position(70);

	screen_title(title);
	define_y_position(99);

	if(strlen(message)>0)
	{
		final_msg = malloc(strlen(message)+5);
		strcpy(final_msg,message);
	}
	else
	{
		final_msg = malloc(5);
		strcpy(final_msg,"");
	}
	va_start(argp, message);		/* restart; 2nd scan */

	while((part = va_arg(argp, char *)) != NULL)
	{
		if(strlen(part)>0)
		{
			if(strlen(message)>0)
			{
				final_msg = realloc(final_msg, strlen(part)+strlen(final_msg)+5);
				strcat(final_msg," ");
				strcat(final_msg,part);
			}
			printf("part %s \n", part);
		}

		//printf(" : %s\n",part);
		//(void)strcat(retbuf, p);
	}

	va_end(argp);

	strcpy(mymsg , final_msg);
	translate_long_strings_and_print(mymsg);
	//lcd_printf(ALG_CENTER, message);
	if(image_present)
		lcd_show_picture("images.jpg");

	lcd_set_font_color(COLOR_BLACK);
	lcd_flip();
	free(final_msg);


}

//
void message_display_function_extended(int image_present , char * imageurl , char * title ,const char* format, ...)
{
	/*	char  mymsg[strlen(message)+ 2];
	lcd_clean();
	define_y_position(70);
	if(started_program)
		screen_title(translate_name_by_key(title));
	else
		screen_title(title);
	define_y_position(99);
	strcpy(mymsg , message);
	translate_long_strings_and_print(mymsg);
	//lcd_printf(ALG_CENTER, message);
	if(image_present)
	lcd_show_picture("images.png");

	lcd_set_font_color(COLOR_BLACK);*/


}
void translate_long_strings_and_print(char *str)
{
	//	char * str;

	if (str != NULL) {
		int counter =0;
		char newstring[100];
		char * string;
		char ** data = 0;
		char * pch = NULL;
		int i;



		pch = strtok (str,"\n");
		while (pch != NULL)
		{

			printf("PCH = %s \n" , pch);
			data = increament_pointersize_and_copy(data,counter,pch);
			counter ++;
			pch = strtok (NULL,"\n");
		}
		for(i = 0 ; i<counter; i++){
			string = malloc(strlen(data[i])+2);
			strcpy(string, data[i]);
			strcpy(newstring ,"");
			//strcpy(pch ,"");
			pch = strtok (string," ");
			if(pch != NULL)

				while (pch != NULL)
				{


					if((strlen(newstring) + strlen(pch)) >=40){

						lcd_printf(ALG_CENTER, newstring);
						strcpy(newstring ,"");
						//counter =0;
					}
					else
						strcat(newstring, " " );
					strcat(newstring, pch);
					//counter = counter + strlen(pch)+1;
					pch = strtok (NULL, " ");

				}
			if(strlen(newstring))
				lcd_printf(ALG_CENTER, newstring);
			strcpy(newstring ,"");
			free(string);
		}
	}
}
void organise_screen_on_input_login(char * charact,char * charact2,int iLen, char * txt1, char * txt2, char* mode_in,int location, int display_string_below,char * title)
{
	char masked_char[50];
	char stars[20];
	//char * title = " Sample Title  ";
	strcpy(stars ,"*************" );
	lcd_clean();
	define_y_position(0);
	define_y_position(70);
	screen_title(title);
	//lcd_printf(ALG_LEFT, " ");
	//screen_header();
	//lcd_printf(ALG_LEFT, " ");
	define_y_position(99);
	lcd_printf(ALG_LEFT, charact);


	lcd_set_font_color(COLOR_BLACK);
	if(location == 1)
		lcd_draw_rectangle_colored(30, 119, 250, 24,0x00,0x00, 0xFF,0xFF);
	else
		lcd_draw_rectangle(30, 119, 250, 24);


	//main_surface->SetColor(main_surface, 0x00, 0x9C, 0xFB, 0xff);

	lcd_set_font_color(COLOR_BLUE);
	//if(!flag_new_line)
	//lcd_printf(ALG_LEFT, " ");

	//flag_new_line=0;



	define_y_position(125);
	if(flag_getting_pwd1)
	{

		if(strlen(txt1)>0)
		{
			snprintf(masked_char,strlen(txt1)+1,stars);
			lcd_printf(ALG_CENTER, "%s", masked_char);
		}
		else
			lcd_printf(ALG_CENTER, "%s", txt1);

	}
	else

		lcd_printf(ALG_CENTER, "%s", txt1);
	//if(iLen==0)
	//lcd_printf(ALG_CENTER, "");
	//lcd_printf(ALG_LEFT, mode_in);


	//lcd_printf(ALG_CENTER, " VIHIGA COUNTY ");
	//

	if(!display_string_below)
	{
		if(strlen(charact2)> 0)
		{
			lcd_set_font_color(COLOR_BLACK);
			define_y_position(147);
			lcd_printf(ALG_LEFT, charact2);
			//lcd_printf(ALG_LEFT, " ");


			if(location == 2)
				lcd_draw_rectangle_colored(30, 167, 250, 24,0x00,0x00, 0xFF,0xFF);
			else
				lcd_draw_rectangle(30, 167, 250, 24);

			define_y_position(175);
			lcd_set_font_color(COLOR_BLUE);
			if(flag_getting_pwd2)
			{
				if(strlen(txt2)>0)
				{
					snprintf(masked_char,strlen(txt2)+1,stars);
					lcd_printf(ALG_CENTER, "%s", masked_char);
				}
				else
					lcd_printf(ALG_CENTER, "%s", txt2);

			}
			else
				lcd_printf(ALG_CENTER, "%s", txt2);

		}
		lcd_set_font_color(COLOR_BLACK);

		//Get one input with Description below it


	}
	else
	{
		lcd_set_font_color(COLOR_BLACK);
		define_y_position(147);
		lcd_printf(ALG_LEFT, charact2);
		lcd_printf(ALG_CENTER, "%s", txt2);

	}
	//if(iLen==0)

	lcd_printf_ex(ALG_CENTER,178, "_____________________________________________");
	define_y_position(194);

	lcd_set_font_color(COLOR_RED);
	lcd_printf(ALG_CENTER, mode_in);
	lcd_set_font_color(COLOR_BLACK);
	lcd_show_picture("images.jpg");

	lcd_flip();
}
int message_display_getdate( char * day , char * month){
	int ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  2, 2, day, month,NULL ,"Please enter day","Please enter month", "Getting Date Values", 0);

}


 int confirm_screen(char  * header ,  cJSON * object)
 {
	 int y  , i;
	 defined_x = 100;
	 int key;
	 while(1)
	 {
		 message_display_function(1,"", header," ", (char *)NULL);


		 for( i =0 ;  i <cJSON_GetArraySize(object)  ; i ++ )
		 {
			 cJSON * array_object =  cJSON_GetArrayItem(object  , i );
			 y = get_y_position();
			 lcd_printf(ALG_LEFT ,"%s", get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(array_object , "key"))));
			 lcd_printf_ex(ALG_LEFT_DEF ,y ,": %s",get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(array_object , "value"))));
		 }

		 lcd_printf(ALG_LEFT ,"");
		 lcd_set_font_color(COLOR_RED);

		 lcd_printf(ALG_CENTER ,"____________________________________________");
		 y = get_y_position();
		 defined_x = 150;

		 lcd_set_font_color(COLOR_BLUE);
		 lcd_printf(ALG_CENTER ,"%s" , "Cancel to exit      :     Enter to confirm");

		 lcd_flip();
		 lcd_set_font_color(COLOR_BLACK);

		 key = kb_getkey();
		 printf("Key  :  %d\n" ,key ) ;
		 if(key == DIKS_ENTER  || key==DIKS_ESCAPE){
			break;
		 }
	 }
	// key = kb_getkey();
	 if(key == DIKS_ENTER){
		 return 1;

	 }else if(key==DIKS_CANCEL)
	 {
		 return 0;
	 }
	 return  0 ;

 }
