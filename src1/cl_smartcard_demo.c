#include <posapi.h>
#include <iso14443.h>

#include "../src/utilities/lcd.h"


void ContactlessSmartCard_Demo(void){
	#define reader_name  "/dev/nfc"
	#define CL_SHOW_CARD_PICTURE "cl_bk_pic.jpg"
	int ifd = -1;
	int retval = 0;
	int status;

	int i;

	char szTemp[800];
	uint32_t uiApdu= 0;
	unsigned char bApdu[30];
	uint32_t uiApduResp= 0;
	unsigned char bApduResp[300];
	
	lcd_clean();

	ifd = mif_open(reader_name);
	printf("mif_open(%s) return %d\r\n", reader_name, ifd);

	if (ifd < 0){
		printf("mif_open() return %d\r\n", ifd);
		lcd_printf(ALG_LEFT, "NFC init failed.");
		lcd_flip();
		kb_getkey();
	}else {
		lcd_printf(ALG_LEFT, "Device open success....");
		lcd_printf(ALG_LEFT, "Tap card....");
		lcd_flip();
		lcd_save();
		lcd_show_picture(CL_SHOW_CARD_PICTURE);
		emv_contactless_active_picc(ifd);
		lcd_restore();
		while (1){
			emv_contactless_obtain_status(ifd, &status);
			if (status == NFC_OP_EXCHANGE_APDU)
			{
				break;
			}
			else{
				emv_contactless_get_lasterror(ifd, &status);
				if (EMULTIHOP == status){
					lcd_printf(ALG_LEFT, "multi card....");
					lcd_flip();
				}
			}

			if (kb_hit()){
				if (0x1B == kb_getkey()){
					retval = -1;
					break;
				}				
			}
		}

		
		if (0 == retval){
			uiApdu = 0;
			bApdu[uiApdu++] = 0x00;
			bApdu[uiApdu++] = 0xA4;
			bApdu[uiApdu++] = 0x04;
			bApdu[uiApdu++] = 0x00;
			bApdu[uiApdu++] = 0x07;
			memcpy(bApdu + uiApdu, "\xA0\x00\x00\x00\x03\x10\x10", 7);
			uiApdu += 7;

			memset(szTemp, 0, sizeof(szTemp));
			for (i = 0; i < (int)uiApdu; i++){
				sprintf(szTemp + strlen(szTemp), "%02X", bApdu[i]);
			}
			
			lcd_printf(ALG_LEFT, "Send Apdu:\n%s", szTemp);
			
			kb_hit();
			lcd_flip();			
			uiApduResp = sizeof(bApduResp);

			if (0 == emv_contactless_exchange_apdu(ifd, uiApdu, bApdu, &uiApduResp, bApduResp)){
				memset(szTemp, 0, sizeof(szTemp));
				for (i = 0; i < (int)uiApduResp; i++){
					sprintf(szTemp + strlen(szTemp), "%02X", bApduResp[i]);
				}
				lcd_printf(ALG_LEFT, "Card Response:\n%s", szTemp);
			}
			else {
				lcd_printf(ALG_LEFT, "Exchage apdu failed");
			}

			lcd_printf(ALG_LEFT, "Please remove card");
			lcd_flip();
			emv_contactless_deactive_picc(ifd); 
			//******Capture beneficiary details here*******
			lcd_flip();
			kb_getkey();
		}
	}

	if (ifd >=0){
		printf("mif_close(%d)\r\n", ifd);
		mif_close(ifd);
		ifd = -1;
	}
	
}
int identifyCard_Demo(void){
	#define reader_name  "/dev/nfc"
	#define CL_SHOW_CARD_PICTURE "cl_bk_pic.jpg"
	int ifd = -1;
	int retval = 0;
	int status;

	int i;

	char szTemp[800];
	uint32_t uiApdu= 0;
	unsigned char bApdu[30];
	uint32_t uiApduResp= 0;
	unsigned char bApduResp[300];

	lcd_clean();

	ifd = mif_open(reader_name);
	printf("mif_open(%s) return %d\r\n", reader_name, ifd);

	if (ifd < 0){
		printf("mif_open() return %d\r\n", ifd);
		lcd_printf(ALG_LEFT, "NFC init failed.");
		lcd_flip();
		kb_getkey();
	}else {
		lcd_printf(ALG_LEFT, "Device open success....");
		lcd_printf(ALG_LEFT, "Tap card....");
		lcd_flip();
		lcd_save();
		lcd_show_picture(CL_SHOW_CARD_PICTURE);
		emv_contactless_active_picc(ifd);
		lcd_restore();
		while (1){
			emv_contactless_obtain_status(ifd, &status);
			if (status == NFC_OP_EXCHANGE_APDU)
			{
				break;
			}
			else{
				emv_contactless_get_lasterror(ifd, &status);
				if (EMULTIHOP == status){
					lcd_printf(ALG_LEFT, "multi card....");
					lcd_flip();
				}
			}

			if (kb_hit()){
				if (0x1B == kb_getkey()){
					retval = -1;
					break;
				}
			}
		}


		if (0 == retval){
			uiApdu = 0;
			bApdu[uiApdu++] = 0x00;
			bApdu[uiApdu++] = 0xA4;
			bApdu[uiApdu++] = 0x04;
			bApdu[uiApdu++] = 0x00;
			bApdu[uiApdu++] = 0x07;
			memcpy(bApdu + uiApdu, "\xA0\x00\x00\x00\x03\x10\x10", 7);
			uiApdu += 7;

			memset(szTemp, 0, sizeof(szTemp));
			for (i = 0; i < (int)uiApdu; i++){
				sprintf(szTemp + strlen(szTemp), "%02X", bApdu[i]);
			}

			lcd_printf(ALG_LEFT, "Send Apdu:\n%s", szTemp);

			kb_hit();
			lcd_flip();
			uiApduResp = sizeof(bApduResp);

			if (0 == emv_contactless_exchange_apdu(ifd, uiApdu, bApdu, &uiApduResp, bApduResp)){
				memset(szTemp, 0, sizeof(szTemp));
				for (i = 0; i < (int)uiApduResp; i++){
					sprintf(szTemp + strlen(szTemp), "%02X", bApduResp[i]);
				}
				lcd_printf(ALG_LEFT, "Card Response:\n%s", szTemp);
			}
			else {
				lcd_printf(ALG_LEFT, "Exchage apdu failed");
			}

			lcd_printf(ALG_LEFT, "Please remove card");
			lcd_flip();
			emv_contactless_deactive_picc(ifd);
			lcd_flip();
			kb_getkey();
		}
	}

	if (ifd >=0){
		printf("mif_close(%d)\r\n", ifd);
		mif_close(ifd);
		ifd = -1;
	}
	return 1;

}

