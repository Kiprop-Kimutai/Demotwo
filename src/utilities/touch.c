#include "touch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>

#include <linux/vt.h>
#include <linux/kd.h>
#include <linux/fb.h>
#include <posapi.h>


static const char *defaultfbdevice = "/dev/fb0";

int touch_init(struct TOUCH_CTX * ctx){
	int retval = 0;
	const char * fbdevice = NULL;
	int addr, y;
	memset(ctx, 0, sizeof(*ctx));
	if ((fbdevice = getenv ("TSLIB_FBDEVICE")) == NULL)
		fbdevice = defaultfbdevice;
	ctx->fbfd = open(fbdevice, O_RDWR);
	if (ctx->fbfd  == -1) {
		retval = -1;
	}else{
		if (ioctl(ctx->fbfd, FBIOGET_FSCREENINFO, &ctx->fix) < 0) {
			retval = -1;
		}else if (ioctl(ctx->fbfd, FBIOGET_VSCREENINFO, &ctx->var) < 0) {
			retval = -1;
		}
	}
	
	if (0 == retval){
		ctx->fbuffer = mmap(NULL, ctx->fix.smem_len, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, ctx->fbfd, 0);
		if (ctx->fbuffer == (unsigned char *)-1) {
			retval = -1;
		}
	}

	if (0 == retval){
		ctx->bytes_per_pixel = (ctx->var.bits_per_pixel + 7) / 8;
		ctx->line_addr = malloc (sizeof (__u32) * ctx->var.yres_virtual);
		addr = 0;
		for (y = 0; y < ctx->var.yres_virtual; y++, addr += ctx->fix.line_length)
			ctx->line_addr [y] = ctx->fbuffer + addr;
	}

	if (retval != 0)
		touch_uninit(ctx);
	else {
		ctx->area_x1 = 0;
		ctx->area_y1 = sys_get_status_bar_height();
		ctx->area_x2 =  ctx->var.xres_virtual;
		ctx->area_y2 = ctx->var.yres_virtual;
	}
	return retval;
}

void touch_uninit(struct TOUCH_CTX * ctx){

	if (NULL != ctx){
		if (NULL != ctx->fbuffer)
			munmap(ctx->fbuffer, ctx->fix.smem_len);
		close(ctx->fbfd);
		memset(ctx, 0, sizeof(*ctx));
	}
}

void touch_clear(struct TOUCH_CTX * ctx, unsigned int color){
	int x, y;
	for (y = 0; y < ctx->var.yres_virtual; y++)
	{
		for (x = 0; x < ctx->var.xres_virtual; x++)
		{
			touch_pixel(ctx, x, y, color, 0);
		}
	}


}

union multiptr {
	unsigned char *p8;
	unsigned short *p16;
	unsigned long *p32;
};


static inline void __setpixel (struct TOUCH_CTX * ctx, union multiptr loc, unsigned xormode, unsigned int color)
{
	switch(ctx->bytes_per_pixel) {
	case 1:
	default:
		if (xormode)
			*loc.p8 ^= color;
		else
			*loc.p8 = color;
		break;
	case 2:
		if (xormode)
			*loc.p16 ^= color;
		else
			*loc.p16 = color;
		break;
	case 4:
		if (xormode)
			*loc.p32 ^= color;
		else
			*loc.p32 = color;
		break;
	}
}



void touch_pixel (struct TOUCH_CTX * ctx,
				  int x,
				  int y,
				  unsigned int color,
				  unsigned char xormode)
{

	union multiptr loc;
	if ((x >= ctx->area_x1)&& (x <= ctx->area_x2) && (y >= ctx->area_y1) && (y <= ctx->area_y2)){
		if ((x < 0) || ((__u32)x >= ctx->var.xres_virtual) ||
			(y < 0) || ((__u32)y >= ctx->var.yres_virtual))
			return;

		loc.p8 = ctx->line_addr [y] + x * ctx->bytes_per_pixel;
		__setpixel (ctx, loc, xormode, color);
	}
}

void touch_line (struct TOUCH_CTX * ctx,
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int color,
		unsigned char xormode)
{
	int tmp;
	int dx = x2 - x1;
	int dy = y2 - y1;

	if (abs (dx) < abs (dy)) {
		if (y1 > y2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		x1 <<= 16;
		/* dy is apriori >0 */
		dx = (dx << 16) / dy;
		while (y1 <= y2) {
			touch_pixel (ctx, x1 >> 16, y1, color, xormode);
			x1 += dx;
			y1++;
		}
	} else {
		if (x1 > x2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		y1 <<= 16;
		dy = dx ? (dy << 16) / dx : 0;
		while (x1 <= x2) {
			touch_pixel (ctx, x1, y1 >> 16, color, 0);
			y1 += dy;
			x1++;
		}
	}
}


inline void swap_int(int *a, int *b)
{
	*a ^= *b;
	*b ^= *a;
	*a ^= *b;
}


void touch_line_ex (struct TOUCH_CTX * ctx,
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int color,
		unsigned char xormode)

{

	int ix, iy, cx, cy, n2dy, n2dydx, d;
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int yy = 0;

	if (dx < dy) {
		yy = 1;
		swap_int(&x1, &y1);
		swap_int(&x2, &y2);
		swap_int(&dx, &dy);
	} 

	ix = (x2 - x1) > 0 ? 1 : -1;
	iy = (y2 - y1) > 0 ? 1 : -1;
	cx = x1;
	cy = y1;
	n2dy = dy * 2;
	n2dydx = (dy - dx) * 2;
	d = dy * 2 - dx;
	
	if (yy) {

		touch_pixel(ctx, cy-1, cx-1, color, xormode);
		touch_pixel(ctx, cy,	cx-1, color, xormode);
		touch_pixel(ctx, cy+1, cx-1, color, xormode);
		while (cx != x2) {
			if (d < 0) { 
				d += n2dy;
			} else {
				cy += iy;
				d += n2dydx;
			}

			touch_pixel(ctx, cy-2, cx, color, xormode);
			touch_pixel(ctx, cy-1, cx, color, xormode);
			touch_pixel(ctx, cy,   cx, color, xormode);
			touch_pixel(ctx, cy+1, cx, color, xormode);
			touch_pixel(ctx, cy+2, cx, color, xormode);

			cx += ix;
		}
		touch_pixel(ctx, cy-1, cx, color, xormode);
		touch_pixel(ctx, cy,	cx, color, xormode);
		touch_pixel(ctx, cy+1, cx, color, xormode);
	} else {

		touch_pixel(ctx, cx-1, cy-1, color, xormode);
		touch_pixel(ctx, cx-1, cy,   color, xormode);
		touch_pixel(ctx, cx-1, cy+1, color, xormode);
		while (cx != x2) {
			if (d < 0) {
				d += n2dy;
			} else {
				cy += iy;
				d += n2dydx;
			}
			touch_pixel(ctx, cx, cy-2, color, xormode);
			touch_pixel(ctx, cx, cy-1, color, xormode);
			touch_pixel(ctx, cx, cy,   color, xormode);
			touch_pixel(ctx, cx, cy+1, color, xormode);
			touch_pixel(ctx, cx, cy+2, color, xormode);

			cx += ix;
		}
		touch_pixel(ctx, cx, cy-1, color, xormode);
		touch_pixel(ctx, cx, cy,   color, xormode);
		touch_pixel(ctx, cx, cy+1, color, xormode);
	}
}
extern IDirectFB				*dfb ;
IDirectFBSurface * touch_get_draw(struct TOUCH_CTX * ctx){
	DFBRectangle rect;
	IDirectFBSurface * fbsurface = NULL;
	IDirectFBSurface * fbdraw = NULL;
	DFBSurfaceDescription	surfdesc;
	void * ptr = NULL;
	int    pitch = 0;
	surfdesc.flags = DSDESC_CAPS | DSDESC_WIDTH | DSDESC_HEIGHT;
	surfdesc.caps  = DSCAPS_SYSTEMONLY;
	surfdesc.width = ctx->var.xres;
	surfdesc.height= ctx->var.yres;
    dfb->CreateSurface(dfb, &surfdesc, &fbsurface);
	fbsurface->Lock(fbsurface,
				  	 DSLF_WRITE,
  					&ptr,
  					&pitch);
	
	memcpy(ptr, ctx->fbuffer, pitch * surfdesc.height );
	fbsurface->Unlock(fbsurface);
	//return fbsurface;
	
	rect.x = ctx->area_x1;
	rect.y = ctx->area_y1;
	rect.w = ctx->area_x2 - ctx->area_x1;
	rect.h = ctx->area_y2 - ctx->area_y1;
	fbsurface->GetSubSurface(fbsurface, &rect, &fbdraw);
	fbsurface->Release(fbsurface);

	return fbdraw;
}


