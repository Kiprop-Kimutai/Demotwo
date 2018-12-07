#include <posapi.h>
#include <magstripe.h>

#include "../src/utilities/lcd.h"
#define DEVICE_NAME "/dev/magcard0"

void magneticstrip_demo(void){

	int ifd = -1;
	struct timeval	t;
	fd_set			rfds;
	user_magcard_info_t cardinfo;
	int ret;
	lcd_clean();
	ifd = magstripe_open(DEVICE_NAME, O_RDONLY | O_NONBLOCK);
	if (ifd < 0){
		printf("mif_open() return %d\r\n", ifd);
		lcd_printf(ALG_LEFT, "Device Open failed.");
		lcd_flip();
		kb_getkey();
	}else {
		lcd_printf(ALG_LEFT, "Please swip card....");
		lcd_flip();
		while (1){			
			FD_ZERO(&rfds);
			FD_SET(ifd, &rfds);
			t.tv_sec = 0;
			t.tv_usec = 200000;
			if ((select(ifd + 1, &rfds, NULL, NULL, &t) > 0) && (FD_ISSET(ifd, &rfds))){
				printf("swipted....\r\n");
				memset(&cardinfo, 0, sizeof(cardinfo));
				if ((ret = magstripe_read_timeout(ifd, &cardinfo, 100)) == sizeof(cardinfo))
				{
					int		i;
					printf("read track data....\r\n");
					for (i=0; i < 3; i++) {
						if (cardinfo.magcardtrack[i].status==0) {
							if (strlen((char*) cardinfo.magcardtrack[i].data)>0)
							{
								lcd_printf(ALG_LEFT, "Track %d:\n%s", i+1, cardinfo.magcardtrack[i].data);
							}
						}
					}
					lcd_printf(ALG_LEFT, "Please swip card....");
					lcd_flip();
				}
				
			}else {
				usleep(100000);
			}

			if (kb_hit()){
				if (kb_getkey() == 0x1B)
					break;
			}
		}
		magstripe_close(ifd);
	}
	
}
