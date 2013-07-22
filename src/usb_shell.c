#include "usb_shell.h"
#include "project.h"

#include "chprintf.h"
#include <stdlib.h>

static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  size_t n, size;
  int i;

  if (argc > 0) {
    chprintf(chp, "Usage: mem\r\n");

    return;
  };

  n = chHeapStatus(NULL, &size);

  chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
  chprintf(chp, "heap fragments   : %u\r\n", n);
  chprintf(chp, "heap free total  : %u bytes\r\n", size);

  // will return the bluetooth address and serial number within the device.
  chprintf(chp, "HW Informations:\r\n");

  for (i = 0; i < 43; i++) {
    chprintf(chp, "%c", ((char*)0x0803E800)[i]);
    // internal offset: 134473728 = 0x0803E800
    // fw.bin offset: 243712 = 0x0003B800
  }

  chprintf(chp, "\r\n");
};

static void cmd_set_time(BaseSequentialStream *chp, int argc, char *argv[]) {
  if (argc != 3) {
    chprintf(chp, "Usage: set_time hh mm ss\r\n");
    return;
  }

  // yes, it does not check if the input is a valid number.
  // on the other side: its not much code.
  time_offset = ((atoi(argv[0])+12) *60 +atoi(argv[1])) *60 +atoi(argv[2]);
  time_offset *= 1000;
  time_offset -= chTimeNow();
};

static void cmd_vibrator_enable(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)chp;
  (void)argc;
  (void)argv;

  palSetPad(GPIOC, GPIOC_VIBRATOR_ENABLE);
};

static void cmd_vibrator_disable(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)chp;
  (void)argc;
  (void)argv;

  palClearPad(GPIOC, GPIOC_VIBRATOR_ENABLE);
};

static void cmd_vibrator_toggle(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)chp;
  (void)argc;
  (void)argv;

  palTogglePad(GPIOC, GPIOC_VIBRATOR_ENABLE);
};

static void cmd_led_enable(BaseSequentialStream *chp, int argc, char *argv[]) {
	if (argc != 2) {
		chprintf(chp, "this command requires two arguments:\r\n");
		chprintf(chp, "the first defines the LED: 1 = R, 2 = G, 4 = B or combined: 5 = RB\r\n");
		chprintf(chp, "the second argument defines the delay the led is enabled\r\n");
		chprintf(chp, "keep in mind that this sequence will continue after one second.\r\n");
		return;
	}
	int colors = atoi(argv[0]);
	uint16_t delay = PWM_PERCENTAGE_TO_WIDTH(&PWMD3, atoi(argv[1]));

	if (colors & 1) {
		// red
		pwmEnableChannel(&PWMD5, 1, delay);
	}
	if (colors >> 1 & 1) {
		// green
		pwmEnableChannel(&PWMD3, 1, delay);
	}
	if (colors >> 2 & 1) {
		// blue
		pwmEnableChannel(&PWMD1, 1, delay);
	}
};

static void cmd_led_disable(BaseSequentialStream *chp, int argc, char *argv[]) {
	if (argc != 1) {
		chprintf(chp, "this command requires one arguments:\r\n");
		chprintf(chp, "the argument defines the LED: 1 = R, 2 = G, 4 = B or combined: 5 = RB\r\n");
		return;
	}
	int colors = atoi(argv[0]);
	
	if (colors & 1) {
		// red
		pwmDisableChannel(&PWMD5, 1);
	}
	if (colors >> 1 & 1) {
		// green
		pwmDisableChannel(&PWMD3, 1);
	}
	if (colors >> 2 & 1) {
		// blue
		pwmDisableChannel(&PWMD1, 1);
	}
};

static const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"set_time", cmd_set_time},
  {"vibrator_enable", cmd_vibrator_enable},
  {"vibrator_disable", cmd_vibrator_disable},
  {"vibrator_toggle", cmd_vibrator_toggle},
  {"led_enable", cmd_led_enable},
  {"led_disable", cmd_led_disable},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SDU1,
  commands
};

