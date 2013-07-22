
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
// #include "test.h"

// #include "usb_cdc.h"

#include "gdisp.h"
#include "gwin.h"
#include "graph.h"
// #include "math.h"

#include "aclock/aclock.h"
#include "framebuffer_draw.h"

#include "hardware.h"

#include "project.h"
unsigned long time_offset = 0;

#include "usb_hw.h"
#include "usb_shell.h"

#define SHELL_WA_SIZE   THD_WA_SIZE(4096)
#define TEST_WA_SIZE    THD_WA_SIZE(512)

// =======================================

typedef unsigned long (*olv_event_func)(void);

typedef struct {
  unsigned char enabled;
  unsigned long ttn;
  olv_event_func func;
} olv_event_pool;

//used by aclock:
static unsigned int beep = 0;

// used by button:
static uint8_t olv_buttonbits = 0;
static unsigned long olv_buttons[6] = {0};
static uint8_t olv_buttonflags[6] = {0,0,0,0,0,2};
static uint8_t olv_buttonticks[6] = {0};
static unsigned long olv_disp_timeout = 0;
#define OLV_DISP_TIMEOUT 20000

// used by menu:
static uint8_t olv_menu_state = 0;

// do not forget to add prototypes here.
// if you forget it, the olv_event array will fail epicly.
static unsigned long olv_button (void);
static unsigned long olv_menu (void);
static unsigned long olv_set_time (void);
static unsigned long olv_aclock (void);
static unsigned long olv_debug_info (void);

// do not forget to change the defines above the olv_ methods.
#define OLV_EVENT_COUNT 5

#define OLV_EVENT_BUTTON 0
#define OLV_EVENT_MENU 1
#define OLV_EVENT_SET_TIME 2
#define OLV_EVENT_ACLOCK 3
#define OLV_EVENT_DEBUG_INFO 4

// ALWAYS keep them as index.
// theese defines help avoiding to go through the event_pool for searching.
// efficiency will come when i have time to clean this up.

// first: unsigned char "enabled". this is to detect if the loop should or should not ignore it.
// third: unsigned long "ttn". this shows a timestamp pointing to the next execution time.
// fourth: "func". this is a pointer to the actual method.
static olv_event_pool olv_event[] = {
  {1, 0, olv_button},
  {1, 0, olv_menu},
  {0, 0, olv_set_time},
  {1, 0, olv_aclock},
  {1, 0, olv_debug_info}
};

static unsigned long olv_event_time; // current tick's time


static unsigned long olv_button (void) {

  static uint8_t i2c_buffer[2];
  static uint8_t i;
  uint8_t draw = framebuffer_active;

  i2cAcquireBus(&I2CD1);

  olv_buttonbits = \
    i2cMasterReceiveTimeout(&I2CD1, I2C1_EXTGPIO_ADDRESS, i2c_buffer, 2, 10) \
    == RDY_OK ? i2c_buffer[1] : 0;

  i2cReleaseBus(&I2CD1);

  // set button states into olv_buttons and olv_buttonflags
  // in future, olv_buttonflags will be used for certain states
  for (i = 0; i < 6; i++) {
    // pressed button detected
    if ((olv_buttonbits >> i) & 1) {
      if (olv_buttons[i] == 0) {
        olv_buttons[i] = olv_event_time;
        olv_buttonticks[i] = 0;
      }
      // auto repeat button press if hold for a certain time
      else if (olv_buttonflags[i]== 1 && olv_buttons[i] +(240 -(olv_buttonticks[i] > 60 ? 60 : olv_buttonticks[i]++)*4) < olv_event_time) {
        olv_buttons[i] = olv_event_time;
        olv_buttonflags[i] = 0;
      }
      // turn screen off after a timeout
      olv_disp_timeout = olv_event_time + OLV_DISP_TIMEOUT;
    // check if button has been released
    } else if (olv_buttons[i] != 0) {
      olv_buttonflags[i] = olv_buttonflags[i] == 2 ? 1 : 0;
      olv_buttonticks[i] = 0;
      olv_buttons[i] = 0;
    }
  }

  // check if power button has been pressed or the display has a timeout because no buttons have been pressed in a while
  if ((!olv_buttons[5] && !olv_buttonflags[5]) || (draw == 1 && olv_disp_timeout < olv_event_time)) {
    draw = draw ? 0 : 1;
    olv_buttonflags[5] = 1;

    // enable / disable following events depending on sleep state.
    olv_event[OLV_EVENT_MENU].enabled = draw; // not cleanly implemented yet
    //olv_event[OLV_EVENT_SET_TIME].enabled = draw; // this shall be a menu item in future.
    olv_event[OLV_EVENT_ACLOCK].enabled = draw;
    olv_event[OLV_EVENT_DEBUG_INFO].enabled = draw;
    
    if (draw) {
      // when device is awakened, change all events to fire within this tick
      olv_event[OLV_EVENT_MENU].ttn = 0;
      // olv_event[OLV_EVENT_SET_TIME].ttn = 0; the menu shall decide to enable this.
      olv_event[OLV_EVENT_ACLOCK].ttn = 0;
      olv_event[OLV_EVENT_DEBUG_INFO].ttn = 0;
    }
    else {
      // when device goes to bed (disable set_time in order to turn menu on)
      olv_event[OLV_EVENT_SET_TIME].enabled = 0; // not cleanly implemented yet
    }

    gdispControl(GDISP_CONTROL_POWER, draw ? powerOn : powerOff);
  }

  // disables screen timeout when the clock is charging
  if (draw && SDU1.config->usbp->state == USB_ACTIVE) {
    olv_disp_timeout = olv_event_time + OLV_DISP_TIMEOUT;
  }

  // powersaving - the stupid way :/
  return draw ? 20 : 250;
};


static unsigned long olv_menu (void) {

  (void)olv_menu_state;
  if (olv_buttons[4] && !olv_buttonflags[4]) {
    olv_buttonflags[4] = 1;
    olv_event[OLV_EVENT_MENU].enabled = 0;
    olv_event[OLV_EVENT_SET_TIME].enabled = 1;
    olv_event[OLV_EVENT_SET_TIME].ttn = 0;
  }

  return olv_event[OLV_EVENT_BUTTON].ttn -olv_event_time;
};


static unsigned long olv_set_time (void) {
 
  // turn vibration on
  if (olv_buttons[4] && olv_buttons[2] && !olv_buttonflags[2]) {
    palSetPad(GPIOC, GPIOC_VIBRATOR_ENABLE);
		pwmEnableChannel(&PWMD1, 1, 10000);
    olv_buttonflags[2] = 1;
  }
  // turn vibration off
  if (olv_buttons[4] && olv_buttons[1] && !olv_buttonflags[1]) {
    palClearPad(GPIOC, GPIOC_VIBRATOR_ENABLE);
		pwmDisableChannel(&PWMD1, 1);
    olv_buttonflags[1] = 1;
  }

  // set second to now and plus 1
  if (olv_buttons[4] && olv_buttons[3] && !olv_buttonflags[3]) {
    time_offset = (time_offset & ~7) +1000 -(olv_event_time %1000);
    olv_buttonflags[3] = 1;
    olv_event[OLV_EVENT_ACLOCK].ttn = 0;
  }
  // set second -1
  if (olv_buttons[4] && olv_buttons[0] && !olv_buttonflags[0]) {
    //time_offset = (time_offset & ~7) -(olv_event_time %1000);
    time_offset -= 1000;
    olv_buttonflags[0] = 1;
    olv_event[OLV_EVENT_ACLOCK].ttn = 0;
  }
  // set minute +1
  if (olv_buttons[3] && !olv_buttonflags[3]) {
    time_offset += 60 *1000;
    olv_buttonflags[3] = 1;
    olv_event[OLV_EVENT_ACLOCK].ttn = 0;
  }
  // set minute -1
  if (olv_buttons[0] && !olv_buttonflags[0]) {
    time_offset -= 60 *1000;
    olv_buttonflags[0] = 1;
    olv_event[OLV_EVENT_ACLOCK].ttn = 0;
  }
  // set hour +1
  if (olv_buttons[2] && !olv_buttonflags[2]) {
    time_offset += 60 *60 *1000;
    olv_buttonflags[2] = 1;
    olv_event[OLV_EVENT_ACLOCK].ttn = 0;
  }
  // set hour -1
  if (olv_buttons[1] && !olv_buttonflags[1]) {
    time_offset -= 60 *60 *1000;
    olv_buttonflags[1] = 1;
    olv_event[OLV_EVENT_ACLOCK].ttn = 0;
  }

  return olv_event[OLV_EVENT_BUTTON].ttn -olv_event_time;
};


static unsigned long olv_aclock (void) {

	//unsigned long now = (unsigned long)chTimeNow() +time_offset;
	unsigned long now = olv_event_time +time_offset;

  // walking dot
  gdispDrawPixel(5 +beep /2 %2 *3, 6 -(beep +1) /2 %2 *3, Black);
  beep++;
  gdispDrawPixel(5 +beep /2 %2 *3, 6 -(beep +1) /2 %2 *3, White);

  // background
  aclock_print(2, 0);

  // hour
  aclock_print(0, now /1000 /60 /60 %12 *5 +now /1000 /60 %60 /12);

  // minute
  aclock_print(1, now /1000 /60 %60);

  // second (for testing)
  aclock_print(1, now /1000 %60);

  // timeout in ms to fire the next draw
  return 1000 -(now %1000);
};


static unsigned long olv_debug_info (void) {

  static uint8_t i;
  //if (olv_buttons[4] != 0 && olv_buttons[4] +3000 < evt) framebuffer_active = 0; else framebuffer_active = 1;

  for (i = 0; i < 6; i++) {
    gdispDrawPixel(5 +i *3, 126, ((olv_buttonbits >> i) &1) ? (olv_buttons[i] +10 < olv_event_time ? White : Red) : Blue);
  }

  return olv_event[OLV_EVENT_BUTTON].ttn -olv_event_time;
};

// =======================================

static WORKING_AREA(olvThreadWorkplace, 1024);

static msg_t olvThread (void *arg) {
  (void)arg;

  unsigned long smallest;
  static uint8_t i;
  olv_event_pool* ev;

  for (;;) {
    olv_event_time = (unsigned long)chTimeNow();
    smallest = 120000 + olv_event_time;

    for (i = 0; i < OLV_EVENT_COUNT; i++) {

      ev = &olv_event[i];

      if (ev->enabled) { 

        if (ev->ttn <= olv_event_time) {
          ev->ttn = ((olv_event_func)ev->func)() +olv_event_time;
        }

        if (ev->ttn < smallest) smallest = ev->ttn;

      }

    }

    framebuffer_draw();

    smallest = smallest -olv_event_time;
    
    /*if (smallest > 200 && SDU1.config->usbp->state != USB_ACTIVE) {
      chSysSuspend();
      chThdSleep(smallest);
      chSysEnable();
    } else {*/
    chThdSleep(smallest < 10 ? 10 : smallest);
    //}
  }
  return 1;
};

// =======================================

int main(void) {
	Thread *shelltp = NULL;

	halInit();
	chSysInit();

	/* partial remap tim3 */
	AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_1;

	pwmStart(&PWMD1, &pwm1_cfg);
	pwmStart(&PWMD3, &pwm3_cfg);
	pwmStart(&PWMD5, &pwm5_cfg);

	sduObjectInit(&SDU1);
	sduStart(&SDU1, &serusbcfg);

	usbDisconnectBus(serusbcfg.usbp);

  // works fine without it
	//chThdSleepMilliseconds(1000);

	usbStart(serusbcfg.usbp, &usbcfg);
	usbConnectBus(serusbcfg.usbp);

	i2cStart(&I2CD1, &i2c1_cfg);

	extStart(&EXTD1, &ext_cfg);

	shellInit();

	gdispInit();
	gdispSetOrientation(GDISP_ROTATE_90);

  gdispClear(Black);
  framebuffer_draw();

  (void)chThdCreateStatic(olvThreadWorkplace, sizeof(olvThreadWorkplace), HIGHPRIO, olvThread, NULL);

	while (TRUE) {
		if (!shelltp && (SDU1.config->usbp->state == USB_ACTIVE))
			shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
		else if (chThdTerminated(shelltp)) {
			chThdRelease(shelltp);
			shelltp = NULL;
		};

		chThdSleepMilliseconds(1000);
	};
};

