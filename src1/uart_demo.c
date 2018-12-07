#include <posapi.h>
#include <stdio.h>
#include <uart.h>

#include "../src/utilities/lcd.h"

#define DEVICE_NAME "/dev/ttyS3"
void uart_demo(void){
	int ifd = -1;
	unsigned char bTemp[1024];
	int           iTemp;
	
	struct timeval	t;
	fd_set			rfds;
	

	lcd_clean();
	ifd = open(DEVICE_NAME,  O_RDWR);
	if (ifd >=0){
		lcd_printf(ALG_CENTER, "UART %s", DEVICE_NAME);
		lcd_printf(ALG_CENTER, "SETTING: 115200,8,n,1");
		
		lcd_flip();
		tty_property_config(ifd, 115200, 8,'n',1,'n');
		while (1){
			t.tv_sec  = 0;
			t.tv_usec = 500000;
			
			FD_ZERO(&rfds);
			FD_SET(ifd, &rfds);
			
			if ((select(ifd + 1, &rfds, NULL, NULL, &t) > 0)&&
				FD_ISSET(ifd, &rfds))
			{
				iTemp = read(ifd, bTemp, sizeof(bTemp));
				if (iTemp > 0){
					write(ifd, bTemp, iTemp);
					lcd_printf(ALG_LEFT, "Recived %d bytes....", iTemp);
					lcd_flip();
				}
			}
			else {
				
				strcpy((char*)bTemp, "HELLO WORLD");
				iTemp = strlen((char*)bTemp);
				write(ifd, bTemp, iTemp);
				usleep(100000);
			}

			
			if (kb_hit()){
				if (0x1B == kb_getkey())
					break;
			}
		}
	}else{
		lcd_printf(ALG_CENTER, "OPEN UART Failed.");
		lcd_flip();
		kb_getkey();
	}
	
	
	if (ifd >=0){
		close(ifd);
		ifd = -1;
	}
}

