#include "ch.h"
#include "hal.h"
#include "gdisp.h"

#if GFX_USE_GDISP

#include "lld/gdisp/emulation.c"

#if defined(GDISP_USE_S6E13B3)
	#include "S6E13B3.h"
#else
	#error "gdispS6E13B3: Either GDISP_USE_S6E13B3 must be defined depending on your controller"
#endif

#if defined(GDISP_SCREEN_HEIGHT)
	#undef GDISP_SCREEN_HEIGHT
#endif

#if defined(GDISP_SCREEN_WIDTH)
	#undef GDISP_SCREEN_WIDTH
#endif

#define GDISP_SCREEN_HEIGHT		128
#define GDISP_SCREEN_WIDTH		128

#define GDISP_INITIAL_CONTRAST	38
#define GDISP_INITIAL_BACKLIGHT	100

#if defined(GDISP_USE_CUSTOM_BOARD) && GDISP_USE_CUSTOM_BOARD
	#include "gdisp_lld_board.h"
#elif defined(BOARD_SE_LIVEVIEW_MN800)
	#include "gdisp_lld_board_se_liveview_mn800.h"
#else
	#include "gdisp_lld_board.h"
#endif

bool_t GDISP_LLD(init)(void) {
  unsigned char init00_cmd = 0x01;
  unsigned char init01_cmd = 0x03;
  unsigned char init01_data[] = { 0x00 };
  unsigned char init02_cmd = 0x07;
  unsigned char init02_data[] = { 0x00, 0x00, 0x07, 0x0F, 0x00, 0x00, 0x07, 0x0F };
  unsigned char init03_cmd = 0x05;
  unsigned char init03_data[] = { 0x00 };
  unsigned char init04_cmd = 0x06;
  unsigned char init04_data[] = { 0x00 };
  unsigned char init05_cmd = 0x04;
  unsigned char init05_data[] = { 0x02 };
  unsigned char init06_cmd = 0x1F;
  unsigned char init06_data[] = { 0x30 };
  unsigned char init07_cmd = 0x30;
  unsigned char init07_data[] = { 0x12 };
  unsigned char init08_cmd = 0x1C;
  unsigned char init08_data[] = { 0x0F };
  unsigned char init09_cmd = 0x1E;
  unsigned char init09_data[] = { 0x00 };
  unsigned char init10_cmd = 0x0E ;
  unsigned char init10_data[] = { 0x05, 0x01, 0x02, 0x05, 0x03, 0x02 };
  unsigned char init11_cmd = 0x0F;
  unsigned char init11_data[] = { 0x06, 0x06, 0x06 };
  unsigned char init12_cmd = 0x1D;
  unsigned char init12_data[] = { 0x0A, 0x0A, 0x0A };
  unsigned char init13_cmd = 0x08;
  unsigned char init13_data[] = { 0x01 };
  unsigned char init14_cmd = 0x02;
  unsigned char init14_data[] = { 0x01 };

  palSetPad(GPIOD, GPIOD_OLED_ENABLE);

  init_board();

  write_cmd(&init00_cmd, sizeof(init00_cmd));
  write_cmd_data(&init01_cmd, sizeof(init01_cmd), init01_data, sizeof(init01_data));
  write_cmd_data(&init02_cmd, sizeof(init02_cmd), init02_data, sizeof(init02_data));
  write_cmd_data(&init03_cmd, sizeof(init03_cmd), init03_data, sizeof(init03_data));
  write_cmd_data(&init04_cmd, sizeof(init04_cmd), init04_data, sizeof(init04_data));
  write_cmd_data(&init05_cmd, sizeof(init05_cmd), init05_data, sizeof(init05_data));
  write_cmd_data(&init06_cmd, sizeof(init06_cmd), init06_data, sizeof(init06_data));
  write_cmd_data(&init07_cmd, sizeof(init07_cmd), init07_data, sizeof(init07_data));
  write_cmd_data(&init08_cmd, sizeof(init08_cmd), init08_data, sizeof(init08_data));
  write_cmd_data(&init09_cmd, sizeof(init09_cmd), init09_data, sizeof(init09_data));
  write_cmd_data(&init10_cmd, sizeof(init10_cmd), init10_data, sizeof(init10_data));
  write_cmd_data(&init11_cmd, sizeof(init11_cmd), init11_data, sizeof(init11_data));
  write_cmd_data(&init12_cmd, sizeof(init12_cmd), init12_data, sizeof(init12_data));
  write_cmd_data(&init13_cmd, sizeof(init13_cmd), init13_data, sizeof(init13_data));
  write_cmd_data(&init14_cmd, sizeof(init14_cmd), init14_data, sizeof(init14_data));

  set_backlight(GDISP_INITIAL_BACKLIGHT);

  GDISP.Width = GDISP_SCREEN_WIDTH;
  GDISP.Height = GDISP_SCREEN_HEIGHT;
  GDISP.Orientation = GDISP_ROTATE_0;
  GDISP.Powermode = powerOn;
  GDISP.Backlight = GDISP_INITIAL_BACKLIGHT;
  GDISP.Contrast = GDISP_INITIAL_CONTRAST;

  #if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
    GDISP.clipx0 = 0;
    GDISP.clipy0 = 0;
    GDISP.clipx1 = GDISP.Width;
    GDISP.clipy1 = GDISP.Height;
  #endif

  //framebuffer_update_start();
  framebuffer_active = 1;

  return TRUE;
};

color_t GDISP_LLD(getpixelcolor)(coord_t x, coord_t y) {
  return (((unsigned char *) framebuffer)[((y * GDISP.Height) + x) * 2] << 8 & 0xFF00) | ((unsigned char *) framebuffer)[((y * GDISP.Height) + x) * 2 + 1];
  //return ((color_t*)framebuffer)[(y * GDISP.Height) + x];
};

void GDISP_LLD(drawpixel)(coord_t x, coord_t y, color_t color) {
  #if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
    if (x < GDISP.clipx0 || y < GDISP.clipy0 || x >= GDISP.clipx1 || y >= GDISP.clipy1) return;
  #endif

  if (framebuffer_changed == 0) {
    if (((((unsigned char *) framebuffer)[((y * GDISP.Height) + x) * 2] << 8 & 0xFF00) | ((unsigned char *) framebuffer)[((y * GDISP.Height) + x) * 2 + 1]) != color) {
      framebuffer_changed = 1;
    } else {
      return;
    }
  }

  ((unsigned char *) framebuffer)[((y * GDISP.Height) + x) * 2] = (color >> 8) & 0xFF;
  ((unsigned char *) framebuffer)[((y * GDISP.Height) + x) * 2 + 1] = color & 0xFF;
  //((color_t*)framebuffer)[(y * GDISP.Height) + x] = color;
};

void GDISP_LLD(control)(unsigned what, void *value) {
  unsigned char display_enable_cmd = 0x02;
  unsigned char display_enable_data;

  switch(what) {
    case GDISP_CONTROL_POWER:
      if (GDISP.Powermode == (gdisp_powermode_t)value)
        return;

      switch((gdisp_powermode_t)value) {
        case powerOff:
          //framebuffer_update_stop();
          framebuffer_active = 0;

          palClearPad(GPIOB, GPIOB_OLED_12V_ENABLE);
          palClearPad(GPIOD, GPIOD_OLED_ENABLE);
        break;

        case powerOn:
          if (GDISP.Powermode != powerSleep) {
            palSetPad(GPIOD, GPIOD_OLED_ENABLE);

            GDISP_LLD(init)();

          } else {
            palSetPad(GPIOB, GPIOB_OLED_12V_ENABLE);

            //framebuffer_update_start();
            framebuffer_active = 1;
          }

          display_enable_data = 0x01;

          write_cmd_data(&display_enable_cmd, sizeof(display_enable_cmd), &display_enable_data, sizeof(display_enable_data));
        break;

        case powerSleep:
          //framebuffer_update_stop();
          framebuffer_active = 0;

          display_enable_data = 0x00;

          write_cmd_data(&display_enable_cmd, sizeof(display_enable_cmd), &display_enable_data, sizeof(display_enable_data));

          palClearPad(GPIOB, GPIOB_OLED_12V_ENABLE);
        break;

        default:
        return;
      };

      GDISP.Powermode = (gdisp_powermode_t)value;

      return;

    case GDISP_CONTROL_BACKLIGHT:
      if ((unsigned)value > 100) value = (void *)100;
      set_backlight((unsigned)value);

      GDISP.Backlight = (unsigned)value;
      return;
  };
};

#endif

