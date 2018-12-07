#include <posapi.h>
#include <stdio.h>
#include <tslib.h>
#include <printer.h>

#include "../src/utilities/lcd.h"
#include "../src/utilities/touch.h"

#define printer_device_name "/dev/printer0"
extern  IDirectFBSurface               *main_surface ;
void touch_demo(void){
	int ifd = -1;
	struct TOUCH_CTX ctx;
	TSAPI struct tsdev * dev  = NULL;
	IDirectFBSurface * fbdraw = NULL;
	int pressure = 0;
	int x = 0, y = 0;
	struct ts_sample sample;
	printer_status_t status;
	int loop = 1;
	lcd_clean();
	lcd_printf(ALG_LEFT, "Open device....");
	lcd_flip();
	dev = ts_open(getenv("TSLIB_TSDEVICE"), 1);
	if (NULL != dev){
		ts_config(dev);
		lcd_printf(ALG_LEFT, "Open device success");
		
		lcd_printf(ALG_LEFT, "Press Enter to start draw");
		lcd_flip();
		
		if (kb_getkey() == 0x0D){
			lcd_save();
			touch_init(&ctx);
			touch_clear(&ctx, COLOR_WITE);
			while (loop){
				
				 if (ts_read(dev, &sample, 1) > 0){
				 	
				 	//lcd_draw_point(sample.x, sample.y, COLOR_BLACK);
				 	
				 	if (pressure){
						touch_line_ex(&ctx, x, y, sample.x, sample.y, COLOR_BLACK, 0);
				 	}else{
				 		touch_line_ex(&ctx, sample.x, sample.y, sample.x, sample.y, COLOR_BLACK, 0);
				 	}

					pressure = sample.pressure;
					x = sample.x;
					y = sample.y;
					/*
				 	printf("x:%d y:%d pressure:%d min:%ld sec:%ld\r\n",
					 	sample.x,
						sample.y,
						sample.pressure,
						sample.tv.tv_sec,
						sample.tv.tv_usec);
					*/

				 }else{
				 	if (kb_hit()){
						switch(kb_getkey()){
							case 0x1B:
								loop = 0;
								break;
							case 0x0D:
								//print current screen
								fbdraw = touch_get_draw(&ctx);
								lcd_restore();
								lcd_printf(ALG_LEFT, "printing...");
								lcd_flip();
								ifd = printer_open(printer_device_name, O_WRONLY | O_NONBLOCK);
								
								print_surface(ifd, fbdraw, 320+300);
								
								fbdraw->Release(fbdraw);
								fbdraw = NULL;
								do
								{
									usleep(100000);
									printer_get_status(ifd, &status);
									
									if (((status.status  >> PRINTER_STATUS_BUSY) & 0x01) == 0x01)
										lcd_printf(ALG_LEFT, "printer status = busy");
									else if (((status.status  >> PRINTER_STATUS_HIGHT_TEMP) & 0x01) == 0x01)
										lcd_printf(ALG_LEFT, "printer status = tempreture high");
									else if (((status.status  >> PRINTER_STATUS_PAPER_LACK) & 0x01) == 0x01)
										lcd_printf(ALG_LEFT, "printer status = no paper");
									else if (((status.status  >> PRINTER_STATUS_FEED) & 0x01) == 0x01)
										lcd_printf(ALG_LEFT, "printer status = feed paper");
									else if (((status.status  >> PRINTER_STATUS_PRINT) & 0x01) == 0x01)
										lcd_printf(ALG_LEFT, "printer status = printing");
									else if (((status.status  >> PRINTER_STATUS_FORCE_FEED) & 0x01) == 0x01)
											lcd_printf(ALG_LEFT, "printer status = force feed paper");
									else if (((status.status  >> PRINTER_STATUS_POWER_ON) & 0x01) == 0x01)
										lcd_printf(ALG_LEFT, "printer status = power on");
									lcd_flip();
									

								}while (status.status != 0);
								printer_close(ifd);
								lcd_printf(ALG_LEFT, "Finished.");
								lcd_flip();
								kb_getkey();
								loop = 0;
								break;
							default:
								break;
						}
				 	}
				 	usleep(1000);
				 }
			}
			touch_uninit(&ctx);
		}
	}else{
		lcd_printf(ALG_LEFT, "Open device failed.");
		lcd_flip();
		kb_getkey();
	}

	if (NULL != dev){
		ts_close(dev);
		dev = NULL;
	}
	lcd_set_bk_color(COLOR_BLACK);
	
}

