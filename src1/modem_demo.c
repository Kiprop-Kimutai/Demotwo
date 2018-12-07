#include <posapi.h>
#include <modem.h>
#include <modem_iface.h>

#include "../src/utilities/lcd.h"
#define MODEM_DEVICE_NAME "/dev/ttyS1"
#define TEL_NUMBER 		  "9075583163166"
#define MSG               "HELLO WORLD"
void modem_demo(void){
	int retval = 0;
	int ifd = -1;
	int status = 0;
	
	lcd_clean();
	
	lcd_printf(ALG_CENTER, "Modem Demo");
	lcd_printf(ALG_LEFT, "Open device.....");
	lcd_flip();
	kb_hit();

	
	
	ifd = modem_open(MODEM_DEVICE_NAME, O_RDWR | O_NONBLOCK);
	if (ifd < 0){
		lcd_printf(ALG_LEFT, "Device open failed.");
		lcd_flip();
		kb_getkey();
		retval = -1;
	}else {
		lcd_printf(ALG_LEFT, "Wait modem ready.....");
		lcd_flip();		
		while (1){
			modem_get_status(ifd, &status);
			if ((status >> 31) == 0)
				break;
			else
				usleep(10000);
		}
	}

	if (0 == retval)
	{
		lcd_printf(ALG_LEFT, "Dialing %s...", TEL_NUMBER);
		lcd_flip();
		kb_hit();
		retval = modem_dialing(ifd, TEL_NUMBER);
		if (0 != retval){
			lcd_printf(ALG_LEFT, "moem_dialing() failed.");
			lcd_flip();
			kb_getkey();
			retval = -1;
		}else {
			while (1)
			{
				modem_get_status(ifd, &status);
				if ((status >> 31) == 0)
					break;
				else
					usleep(10000);
			}

			status = status & 0x7FFFFFFF;
			if (MODEM_STATE_CONNECT != status){
				lcd_printf(ALG_LEFT, "dial failed.");
				lcd_flip();
				kb_getkey();
				retval = -1;
			}else {
				lcd_printf(ALG_LEFT, "Connected");
				lcd_flip();	
				lcd_printf(ALG_LEFT, "Sendding data...");
				lcd_flip();	

				modem_write(ifd, MSG, strlen(MSG) + 1);
				modem_wait_until_sent(ifd);
				
			}
		}
		modem_hangup(ifd);
	}
	

	if (ifd >=0){
		modem_close(ifd);
		ifd = -1;
	}

}
