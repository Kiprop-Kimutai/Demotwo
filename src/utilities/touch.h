#ifndef TOUCH_H_
#define TOUCH_H_
#include <directfb.h>
#include <linux/fb.h>



struct TOUCH_CTX{
	int             fbfd;
	struct fb_fix_screeninfo fix;
	struct fb_var_screeninfo var;
	int    bytes_per_pixel;
	unsigned char * fbuffer;
	unsigned char **line_addr;
	int    area_x1;
	int    area_y1;
	int    area_x2;
	int    area_y2;
};

int touch_init(struct TOUCH_CTX * ctx);
void touch_uninit(struct TOUCH_CTX * ctx);
void touch_clear(struct TOUCH_CTX * ctx,
				unsigned int color);
void touch_pixel (struct TOUCH_CTX * ctx,
				  int x,
				  int y,
				  unsigned int color,
				  unsigned char xormode);
void touch_line(struct TOUCH_CTX * ctx,
				int x1, 
				int y1, 
				int x2, 
				int y2,
				unsigned int color,
				unsigned char xormode);
void touch_line_ex (struct TOUCH_CTX * ctx,
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int color,
		unsigned char xormode);
IDirectFBSurface *  touch_get_draw(struct TOUCH_CTX * ctx);
					 



#endif
