#ifndef _GDISP_LLD_CONFIG_H
#define _GDISP_LLD_CONFIG_H

#if GFX_USE_GDISP

#define GDISP_DRIVER_NAME				"S6E13B3"
#define GDISP_LLD(x)					gdisp_lld_##x##_S6E13B3

#define GDISP_HARDWARE_FILLS		FALSE
#define GDISP_HARDWARE_BITFILLS		FALSE
#define GDISP_HARDWARE_SCROLL		FALSE
#define GDISP_HARDWARE_READPIXEL	TRUE

#define GDISP_HARDWARE_CONTROL		TRUE

#define GDISP_PIXELFORMAT				GDISP_PIXELFORMAT_RGB565

#endif

#endif
