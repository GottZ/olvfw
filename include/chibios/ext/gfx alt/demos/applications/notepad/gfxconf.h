/**
 * This file has a different license to the rest of the GFX system.
 * You can copy, modify and distribute this file as you see fit.
 * You do not need to publish your source modifications to this file.
 * The only thing you are not permitted to do is to relicense it
 * under a different license.
 */

#ifndef _GFXCONF_H
#define _GFXCONF_H

/* GFX sub-systems to turn on */
#define GFX_USE_GDISP			TRUE
#define GFX_USE_GWIN			FALSE
#define GFX_USE_GEVENT			TRUE
#define GFX_USE_GTIMER			TRUE
#define GFX_USE_GINPUT			TRUE

/* Features for the GDISP sub-system. */
#define GDISP_NEED_VALIDATION	TRUE
#define GDISP_NEED_CLIP			TRUE
#define GDISP_NEED_TEXT			TRUE
#define GDISP_NEED_CIRCLE		TRUE
#define GDISP_NEED_ELLIPSE		FALSE
#define GDISP_NEED_ARC			FALSE
#define GDISP_NEED_SCROLL		FALSE
#define GDISP_NEED_PIXELREAD	FALSE
#define GDISP_NEED_CONTROL		TRUE
#define GDISP_NEED_MULTITHREAD	TRUE
#define GDISP_NEED_ASYNC		FALSE
#define GDISP_NEED_MSGAPI		FALSE

/* Builtin Fonts */
#define GDISP_INCLUDE_FONT_SMALL		FALSE
#define GDISP_INCLUDE_FONT_LARGER		FALSE
#define GDISP_INCLUDE_FONT_UI1			FALSE
#define GDISP_INCLUDE_FONT_UI2			TRUE
#define GDISP_INCLUDE_FONT_LARGENUMBERS	TRUE

/* Features for the GINPUT sub-system. */
#define GINPUT_NEED_MOUSE		TRUE

#endif /* _GFXCONF_H */
