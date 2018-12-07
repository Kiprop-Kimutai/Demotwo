#include <posapi.h>
#include <iccard.h>

#include "../src/utilities/lcd.h"

void ContactSmartCard_Demo(void){
	#define reader_name  "/dev/usercard"
	int ifd = -1;
	int key;
	int i, len;
	unsigned char atr[40];
	char szTemp[100];
	uint32_t uiApdu= 0;
	unsigned char bApdu[30];
	uint32_t uiApduResp= 0;
	unsigned char bApduResp[300];
	
	lcd_clean();

	ifd = iccard_open_cardslot(reader_name);


	if (ifd < 0){
		lcd_printf(ALG_LEFT, "Device Open failed.");
		lcd_flip();
		kb_getkey();
	}else {
		lcd_printf(ALG_LEFT, "Device open success....");
		lcd_printf(ALG_LEFT, "Please insert card....");
		lcd_flip();
		//check card in slot
		while (iccard_check_card_in_slot(ifd) != 0){
			if (kb_hit()){
				key = kb_getkey();
				if (key == 0x1B){
					goto END;
				}
			}
		}

		//power up
		len = sizeof(atr);
		if (0 != iccard_power_up(ifd, CVCC_5_VOLT, EMV_MODE, &len, atr)){
			lcd_printf(ALG_LEFT, "Card Power up Failed");
			lcd_flip();	
			kb_getkey();
			goto END;
		}else {
			memset(szTemp, 0, sizeof(szTemp));
			for (i = 0; i < len; i++){
				sprintf(szTemp + strlen(szTemp), "%02X", atr[i]);
			}
			lcd_printf(ALG_LEFT, "ATR:%s", szTemp);
			lcd_flip();	
			kb_hit();
			
			//Execute apdu
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
			
			if (0 == iccard_exchange_apdu(ifd, uiApdu, bApdu, &uiApduResp, bApduResp)){
				memset(szTemp, 0, sizeof(szTemp));
				for (i = 0; i < (int)uiApduResp; i++){
					sprintf(szTemp + strlen(szTemp), "%02X", bApduResp[i]);
				}
				lcd_printf(ALG_LEFT, "Card Response:\n%s", szTemp);
			}
			else {
				lcd_printf(ALG_LEFT, "Exchage apdu failed");
			}
			lcd_flip();
			//power off
			iccard_power_down(ifd);

			kb_getkey();
		}

		
			
		
		
	}
END:
	if (ifd >=0){
		iccard_close_cardslot(ifd);
		ifd = -1;
	}
	
}

