#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <directfb.h>

#include <posapi.h>

#include "string_convert.h"
#define font_file_xx  "/usr/share/fonts/wqy-microhei.ttf"

extern IDirectFB				*dfb;


static IDirectFBSurface *load_image1(IDirectFB *dfb, const char *filename);

void Display_login_screen(const char * title,const char * line1,const char * line2,const char * line3,const char * line4,const char * line5)
{
	int retval = 0;
	IDirectFB				*dfb 	= NULL;
	IDirectFBDisplayLayer	*layer	= NULL;
	IDirectFBWindow			*window	= NULL;
	IDirectFBEventBuffer 	*events	= NULL;
	IDirectFBSurface        *main_surface = NULL;
	IDirectFBFont		    *font_16 = NULL;
	IDirectFBFont		    *font_24 = NULL;
	IDirectFBFont		    *font_32 = NULL;
	int width = 0, height = 0;
	const char * utf8text = NULL;
	const char * utf8text1 = NULL;
	const char * utf8text2 = NULL;
	const char * utf8text3 = NULL;
	const char * utf8text4 = NULL;
	int rectangle_x, rectangle_y, rectangle_width, rectangle_height;
	int text_len = 0;
	int text_len1 = 0;

	DFBWindowDescription	desc;
	DFBDisplayLayerConfig	config;
	DFBFontDescription	    fdesc;

	int x, y;
	

	main_surface->DrawString(main_surface, line2,strlen(line2) ,rectangle_x, y, DSTF_BOTTOMLEFT);

	if (0 != DirectFBCreate(&dfb)){
		printf("DirectFBCreate Failed\r\n");
		retval = -1;
	}
	else if (0 != dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &layer)){
		printf("GetDisplayLayer Failed\r\n");
		retval = -1;			
	}else if (0 != layer->GetConfiguration(layer, &config)){
		printf("GetConfiguration Failed\r\n");
		retval = -1;
	}
	else {
		desc.flags  = DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS;
		desc.posx	= 0;
		desc.posy	= sys_get_status_bar_height();
		desc.width  = config.width;
		desc.height = config.height - desc.posy;
		printf("Screen size %d X %d\r\n", desc.width, desc.height);
		desc.caps	= DWCAPS_ALPHACHANNEL;
	}

	if (0 == retval){
		if (0 !=  layer->CreateWindow(layer, &desc, &window)){
			printf("Create Window Failed\r\n");	
			retval = -1;
		}
		else if (0 !=  window->CreateEventBuffer(window, &events)){
			printf("GetConfiguration Failed\r\n");
			retval = -1;
		}else if (0 != window->GetSurface(window, &main_surface)){
			printf("GetSurface Failed\r\n");
			retval = -1;
		}else if (0 != main_surface->GetSize(main_surface, &width, &height)){
			printf("GetSize Failed\r\n");
			retval = -1;		
		}	
	}
	
	if (0 == retval){
		fdesc.flags  = DFDESC_HEIGHT;
		fdesc.height = 16;	
		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_16);
		if (NULL == font_16){
			printf("craete font16 failed.\r\n");
			retval = -1;
		}
		
		fdesc.height = 24;	
		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_24);
		if (NULL == font_24){
			printf("craete font24 failed.\r\n");
			retval = -1;
		}
		
		fdesc.height = 32;	
		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_32);
		if (NULL == font_32){
			printf("craete font32 failed.\r\n");
			retval = -1;
		}

	}

	if (0 == retval){
		
		window->SetOptions(window, DWOP_ALPHACHANNEL);
		window->SetOpacity(window, 0xFF);
		main_surface->Clear(main_surface, 0xFF, 0xff, 0xff, 0xff);
		printf("Hello");
		utf8text = string_covert(title, strlen(title));
		

		//output text on the screen
		x = config.width/2;
		y = 25;
		text_len = string_len(utf8text);
		
		rectangle_x = 5;
		rectangle_y = y - 10;
		rectangle_width = 195;
		rectangle_height = 40;
		printf("Hello");
		main_surface->SetFont(main_surface, font_32);
		main_surface->SetColor(main_surface, 0x00, 0x00, 0xFF, 0xff);
		main_surface->DrawString(main_surface, utf8text, text_len, x, y, DSTF_CENTER);
		font_32->GetHeight(font_32, &height);
		y+= height;
		
		main_surface->SetFont(main_surface, font_24);
		font_24->GetHeight(font_24, &height);
		main_surface->SetColor(main_surface, 0x00, 0x00, 0x00, 0xff);
		main_surface->DrawString(main_surface, "__________________________________", 40, rectangle_x, y, DSTF_BOTTOMLEFT);
		y+= height;


		utf8text1 = string_covert(line1, strlen(line1));
		text_len1 = string_len(utf8text1);


		printf("Hello");
		main_surface->SetColor(main_surface, 0x00, 0x9C, 0xFB, 0xff);
		main_surface->DrawString(main_surface, utf8text1, text_len1, rectangle_x, y, DSTF_BOTTOMLEFT);
		font_24->GetHeight(font_24, &height);
		y+= rectangle_height + 1;
		printf("Hello");

		//load_image_for_screen(dfb, "logo.png" ,main_surface );

		//rectangle_height = y - rectangle_y;
		main_surface->SetColor(main_surface, 0x00, 0x9C, 0xFB, 0xff);
		main_surface->DrawString(main_surface, line2,strlen(line2) ,rectangle_x, y, DSTF_BOTTOMLEFT);
		main_surface->DrawString(main_surface, line4,strlen(line4) ,rectangle_x+116, y, DSTF_BOTTOMLEFT);
		main_surface->DrawRectangle(main_surface, rectangle_x+115, y-rectangle_height ,
			rectangle_width, rectangle_height );
		y+= rectangle_height + 2;
		printf("Hello");
		main_surface->SetColor(main_surface, 0x00, 0x9C, 0xFB, 0xff);
		main_surface->DrawString(main_surface,line3,strlen(line3) , rectangle_x, y, DSTF_BOTTOMLEFT);
		main_surface->DrawString(main_surface,line5,strlen(line5) , rectangle_x+116, y, DSTF_BOTTOMLEFT);
		main_surface->DrawRectangle(main_surface, rectangle_x+115, y-rectangle_height ,
			rectangle_width, rectangle_height );
		y+= rectangle_height + 2;

		printf("Hello");
		
		main_surface->Flip(main_surface, NULL, 0);
	}

	if (0 == retval){
		DFBWindowEvent  windowEvent;
		
		while(1){			
			if (events->HasEvent(events) == DFB_OK){
				if (events->PeekEvent(events, DFB_EVENT(&windowEvent)) == DFB_OK) {
					events->GetEvent(events, DFB_EVENT(&windowEvent));
					if (windowEvent.type == DWET_KEYUP){
						//window->Close(window);
						//printf("key up********************%02x\r\n", windowEvent.key_symbol);
					}else if (windowEvent.type == DWET_CLOSE){
						window->Destroy(window);
						break;
					}					
				}
			} 
		}
	}
	
	if (NULL != events)
		events->Release(events);
	
	if (NULL != main_surface)
		main_surface->Release(main_surface);
	
	if (NULL != window)
		window->Release(window);
	
	if (NULL != layer)
		layer->Release(layer);

	
	
	
	if (NULL != font_16)
		font_16->Release(font_16);
	
	if (NULL != font_24)
		font_24->Release(font_24);
	
	if (NULL != font_32)
		font_32->Release(font_32);	
	
	if (NULL != dfb)
		dfb->Release(dfb);
	
	return ;
}

void display_request_for_srvce_details(const char * title,const char * line1)
{
	int retval = 0;
	IDirectFB				*dfb 	= NULL;
	IDirectFBDisplayLayer	*layer	= NULL;
	IDirectFBWindow			*window	= NULL;
	IDirectFBEventBuffer 	*events	= NULL;
	IDirectFBSurface        *main_surface = NULL;
	IDirectFBFont		    *font_16 = NULL;
	IDirectFBFont		    *font_24 = NULL;
	IDirectFBFont		    *font_32 = NULL;
	int width = 0, height = 0;
	const char * utf8text = NULL;
	const char * utf8text1 = NULL;
	const char * utf8text2 = NULL;
	const char * utf8text3 = NULL;
	const char * utf8text4 = NULL;
	int text_len = 0;
	int text_len1 = 0;

	DFBWindowDescription	desc;
	DFBDisplayLayerConfig	config;
	DFBFontDescription	    fdesc;
	char * line4 = "Please enter the value of ";
	char  * line = "[                            ]";
	int x, y;

	if (0 != DirectFBCreate(&dfb)){
		printf("DirectFBCreate Failed\r\n");
		retval = -1;
	}
	else if (0 != dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &layer)){
		printf("GetDisplayLayer Failed\r\n");
		retval = -1;
	}else if (0 != layer->GetConfiguration(layer, &config)){
		printf("GetConfiguration Failed\r\n");
		retval = -1;
	}
	else {
		desc.flags  = DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS;
		desc.posx	= 0;
		desc.posy	= sys_get_status_bar_height();
		desc.width  = config.width;
		desc.height = config.height - desc.posy;
		printf("Screen size %d X %d\r\n", desc.width, desc.height);
		desc.caps	= DWCAPS_ALPHACHANNEL;
	}

	if (0 == retval){
		if (0 !=  layer->CreateWindow(layer, &desc, &window)){
			printf("Create Window Failed\r\n");
			retval = -1;
		}
		else if (0 !=  window->CreateEventBuffer(window, &events)){
			printf("GetConfiguration Failed\r\n");
			retval = -1;
		}else if (0 != window->GetSurface(window, &main_surface)){
			printf("GetSurface Failed\r\n");
			retval = -1;
		}else if (0 != main_surface->GetSize(main_surface, &width, &height)){
			printf("GetSize Failed\r\n");
			retval = -1;
		}
	}

	if (0 == retval){
		fdesc.flags  = DFDESC_HEIGHT;
		fdesc.height = 16;
		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_16);
		if (NULL == font_16){
			printf("craete font16 failed.\r\n");
			retval = -1;
		}

		fdesc.height = 24;
		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_24);
		if (NULL == font_24){
			printf("craete font24 failed.\r\n");
			retval = -1;
		}

		fdesc.height = 32;
		dfb->CreateFont(dfb, font_file_xx, &fdesc, &font_32);
		if (NULL == font_32){
			printf("craete font32 failed.\r\n");
			retval = -1;
		}
	}

	if (0 == retval){

		window->SetOptions(window, DWOP_ALPHACHANNEL);
		window->SetOpacity(window, 0xFF);
		main_surface->Clear(main_surface, 0xFF, 0xff, 0xff, 0xff);

		utf8text = string_covert(title, strlen(title));


		//output text on the screen
		x = config.width/2;
		y = 25;
		text_len = string_len(utf8text);


		main_surface->SetFont(main_surface, font_24);
		main_surface->SetColor(main_surface, 0xFF, 0x00, 0x00, 0xff);
		main_surface->DrawString(main_surface, utf8text, text_len, x, y, DSTF_CENTER);
		font_24->GetHeight(font_24, &height);
		y+= height + 1;

		utf8text4 = string_covert(line4,strlen(line4));
		text_len1 = string_len(utf8text4);
		main_surface->SetFont(main_surface, font_16);
		main_surface->SetColor(main_surface,0xFF,0xFF ,0xFF ,0xFF );
		main_surface->DrawString(main_surface, utf8text4, text_len1, x, y, DSTF_CENTER);
		font_16->GetHeight(font_16, &height);
		y+= height + 1;

		utf8text1 = string_covert(line1, strlen(line1));
		text_len1 = string_len(utf8text1);
		main_surface->SetFont(main_surface, font_24);
		main_surface->SetColor(main_surface, 0x00, 0x00, 0xFF, 0xff);
		main_surface->DrawString(main_surface, utf8text1, text_len1, x, y, DSTF_CENTER);
		font_24->GetHeight(font_24, &height);
		y+= height + 1;

		utf8text1 = string_covert(line, strlen(line));
		text_len1 = string_len(utf8text1);
		main_surface->SetFont(main_surface, font_24);
		main_surface->SetColor(main_surface, 0x00, 0x00, 0xFF, 0xff);
		main_surface->DrawString(main_surface, utf8text1, text_len1, x, y, DSTF_CENTER);
		font_24->GetHeight(font_24, &height);
		y+= height + 1;
	///	const char name = "logo.png";
		//main_surface = load_image1(dfb,name);
		main_surface->Flip(main_surface, NULL, 0);

		char *filename = "logo.png";
		IDirectFBImageProvider *provider;
		IDirectFBSurface *imgsurf = NULL;
		DFBSurfaceDescription dsc;
		DFBResult err;
		int retval = 0;
		int x , y, height;
		err = dfb->CreateImageProvider(dfb, filename, &provider);
		if (err != DFB_OK) {
			fprintf( stderr, "Couldn't load image from file '%s': %s\n", filename,DirectFBErrorString(err));
			//return NULL;
		}

		provider->GetSurfaceDescription(provider, &dsc);
		dsc.flags = DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT;
		dsc.pixelformat = DSPF_ARGB;
		if (dfb->CreateSurface(dfb, &dsc, &imgsurf) == DFB_OK)
			provider->RenderTo(provider, imgsurf, NULL);

		imgsurf->Flip(imgsurf,NULL,0);
		provider->Release(provider);
	}

	if (0 == retval){
		DFBWindowEvent  windowEvent;

		while(1){
			if (events->HasEvent(events) == DFB_OK){
				if (events->PeekEvent(events, DFB_EVENT(&windowEvent)) == DFB_OK) {
					events->GetEvent(events, DFB_EVENT(&windowEvent));
					if (windowEvent.type == DWET_KEYUP){
						window->Close(window);
						printf("key up********************%02x\r\n", windowEvent.key_symbol);
					}else if (windowEvent.type == DWET_CLOSE){
						window->Destroy(window);

						break;
					}
				}
			}
		}
	}

	if (NULL != events)
		events->Release(events);

	if (NULL != main_surface)
		main_surface->Release(main_surface);

	if (NULL != window)
		window->Release(window);

	if (NULL != layer)
		layer->Release(layer);




	if (NULL != font_16)
		font_16->Release(font_16);

	if (NULL != font_24)
		font_24->Release(font_24);

	if (NULL != font_32)
		font_32->Release(font_32);

	if (NULL != dfb)
		dfb->Release(dfb);

	return ;
}

static IDirectFBSurface *load_image1(IDirectFB *dfb, const char *filename) {
	IDirectFBImageProvider *provider;
	IDirectFBSurface *imgsurf = NULL;
	DFBSurfaceDescription dsc;
	DFBResult err;
	int retval = 0;
	int x , y, height;
//	IDirectFBFont *font_32 = NULL;


	dsc.height = 32;
/*	dfb->CreateFont(dfb, font_file_xx, &dsc, &font_32);
	if (NULL == font_32) {
		lcd_printf(ALG_LEFT, "craete font16 failed.");
		lcd_flip();
		retval = -1;
	}*/


	err = dfb->CreateImageProvider(dfb, filename, &provider);
	if (err != DFB_OK) {
		fprintf( stderr, "Couldn't load image from file '%s': %s\n", filename,DirectFBErrorString(err));
		return NULL;
	}

	provider->GetSurfaceDescription(provider, &dsc);
	dsc.flags = DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT;
	dsc.pixelformat = DSPF_ARGB;
	if (dfb->CreateSurface(dfb, &dsc, &imgsurf) == DFB_OK)
		provider->RenderTo(provider, imgsurf, NULL);


	provider->Release(provider);

	return imgsurf;
}

