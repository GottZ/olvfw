#ifndef _BOARD_H_
#define _BOARD_H_

#define BOARD_SE_LIVEVIEW_MN800
#define BOARD_NAME              "SE LiveView MN800"

#define STM32F10X_HD

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 *
 * The digits have the following meaning:
 *   0 - Analog input.
 *   1 - Push Pull output 10MHz.
 *   2 - Push Pull output 2MHz.
 *   3 - Push Pull output 50MHz.
 *   4 - Digital input.
 *   5 - Open Drain output 10MHz.
 *   6 - Open Drain output 2MHz.
 *   7 - Open Drain output 50MHz.
 *   8 - Digital input with PullUp or PullDown resistor depending on ODR.
 *   9 - Alternate Push Pull output 10MHz.
 *   A - Alternate Push Pull output 2MHz.
 *   B - Alternate Push Pull output 50MHz.
 *   C - Reserved.
 *   D - Alternate Open Drain output 10MHz.
 *   E - Alternate Open Drain output 2MHz.
 *   F - Alternate Open Drain output 50MHz.
 * Please refer to the STM32 Reference Manual for details.
 */

#define VAL_GPIOACRL		0xb4b343b4
#define VAL_GPIOACRH		0x888443b4
#define VAL_GPIOAODR		0x0000a412
                                  
#define VAL_GPIOBCRL		0xffb84443
#define VAL_GPIOBCRH		0xbbb34444
#define VAL_GPIOBODR		0x00001030
                                  
#define VAL_GPIOCCRL		0x48034383
#define VAL_GPIOCCRH		0x40434334
#define VAL_GPIOCODR		0x00000456
                                  
#define VAL_GPIODCRL		0x44444344
#define VAL_GPIODCRH		0x44444444
#define VAL_GPIODODR		0x00000004

#define VAL_GPIOECRL		0x44444444
#define VAL_GPIOECRH		0x44444444
#define VAL_GPIOEODR		0x00000000

#define VAL_GPIOFCRL		0x44444444
#define VAL_GPIOFCRH		0x44444444
#define VAL_GPIOFODR		0x00000000

#define VAL_GPIOGCRL		0x44444444
#define VAL_GPIOGCRH		0x44444444
#define VAL_GPIOGODR		0x00000000

#define GPIOA_LED_RED_ENABLE	1
#define GPIOA_SPI1_NSS		4
#define GPIOA_LED_BLUE_ENABLE	9
#define GPIOA_USB_ENABLE	10

#define GPIOB_OLED_12V_ENABLE	0
#define GPIOB_LED_GREEN_ENABLE	5

#define GPIOC_VIBRATOR_ENABLE	0
#define GPIOC_CHARGE_DISABLED	1
#define GPIOC_USB_DETECT	6
#define GPIOC_SPI1_CD		10

#define GPIOD_OLED_ENABLE	2

#define I2C1_EXTGPIO_ADDRESS	0x2C

#define usb_lld_connect_bus(usbp) palClearPad(GPIOA, GPIOA_USB_ENABLE);
#define usb_lld_disconnect_bus(usbp) palSetPad(GPIOA, GPIOA_USB_ENABLE);

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif
#endif

