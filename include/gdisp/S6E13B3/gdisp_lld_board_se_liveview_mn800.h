#ifndef _GDISP_LLD_BOARD_H
#define _GDISP_LLD_BOARD_H

#include "framebuffer_draw.h"

uint8_t framebuffer_active = 0;

static uint8_t framebuffer_changed = 1;

// the internal memory has been split into two regions.
// the framebuffer points to the second region which it is defined for.
static void *framebuffer = (void *) 0x20008000;

//static Thread *framebuffer_update_thread;
//static WORKING_AREA(wa_framebuffer_update_thread, 128);
//static msg_t framebuffer_update_thread_worker(void *);

// static void framebuffer_update(GPTDriver *);

//static __inline void framebuffer_update_start(void);
//static __inline void framebuffer_update_stop(void);

static const SPIConfig spi1_cfg = {
  NULL,
  GPIOA,
  GPIOA_SPI1_NSS,
  0,
};

static __inline void init_board(void) {
  spiStart(&SPID1, &spi1_cfg);

  palSetPad(GPIOD, GPIOD_OLED_ENABLE);
};

static __inline void acquire_bus(void) {
  spiAcquireBus(&SPID1);

  spiSelect(&SPID1);
};

static __inline void release_bus(void) {
  spiUnselect(&SPID1);

  spiReleaseBus(&SPID1);
};

static __inline void write_cmd(unsigned char *cmd, size_t length) {
  acquire_bus();

  palClearPad(GPIOC, GPIOC_SPI1_CD);

  spiSend(&SPID1, length, cmd);

  palSetPad(GPIOC, GPIOC_SPI1_CD);

  release_bus();
};

static __inline void write_data(unsigned char *data, size_t length) {
  acquire_bus();

  spiSend(&SPID1, length, data);

  release_bus();
};

static __inline void write_cmd_data(unsigned char *cmd_buffer, size_t cmd_length, unsigned char *data_buffer, size_t data_length) {
  write_cmd(cmd_buffer, cmd_length);

  write_data(data_buffer, data_length);
};

static __inline void set_backlight(uint8_t percent) {
  unsigned char display_enable_cmd = 0x02;
  unsigned char display_enable_data;

  if (percent) {
    palSetPad(GPIOB, GPIOB_OLED_12V_ENABLE);
    display_enable_data = 0x01;
  } else {
    palClearPad(GPIOB, GPIOB_OLED_12V_ENABLE);
    display_enable_data = 0x00;
  };

  write_cmd_data(&display_enable_cmd, sizeof(display_enable_cmd), &display_enable_data, sizeof(display_enable_data));
};

static __inline void setviewport(coord_t x, coord_t y, coord_t cx, coord_t cy) {
  unsigned char viewport_cmd = 0x0A;
  unsigned char viewport_data[8];

  viewport_data[0] = (x >> 4) & 0x0F;
  viewport_data[1] = x & 0x0F;
  viewport_data[2] = ((x + cx - 1) >> 4) & 0x0F;
  viewport_data[3] = (x + cx - 1) & 0x0F;
  viewport_data[4] = (y >> 4) & 0x0F;
  viewport_data[5] = y & 0x0F;
  viewport_data[6] = ((y + cy - 1) >> 4) & 0x0F;
  viewport_data[7] = (y + cy - 1) & 0x0F;

  write_cmd_data(&viewport_cmd, sizeof(viewport_cmd), viewport_data, sizeof(viewport_data));
};

static unsigned char draw_pixel_cmd = 0x0C;

/*static msg_t framebuffer_update_thread_worker(void *arg) {
  (void) arg;

  chRegSetThreadName("framebuffer_update_thread");

  // while (TRUE) {

    setviewport(0, 0, GDISP.Width, GDISP.Height);

    write_cmd_data(&draw_pixel_cmd, sizeof(draw_pixel_cmd), (unsigned char *) framebuffer, GDISP.Width * GDISP.Height * 2);
    // chThdSleep(1);
  // };

  return 0;
};*/

/*static __inline void framebuffer_update_start(void) {
	framebuffer_active = 1;
  framebuffer_update_thread = chThdCreateStatic(wa_framebuffer_update_thread, sizeof(wa_framebuffer_update_thread), NORMALPRIO, framebuffer_update_thread_worker, NULL);
};

static __inline void framebuffer_update_stop(void) {
	framebuffer_active = 0;
  // chThdTerminate(framebuffer_update_thread);
};*/

void framebuffer_draw (void) {
	if (!framebuffer_active || !framebuffer_changed) return;
  framebuffer_changed = 0;
	setviewport(0, 0, GDISP.Width, GDISP.Height);
	write_cmd_data(&draw_pixel_cmd, sizeof(draw_pixel_cmd), (unsigned char *) framebuffer, GDISP.Width * GDISP.Height * 2);
}

#endif

