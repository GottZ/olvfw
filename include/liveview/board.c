#include "ch.h"
#include "hal.h"

#if HAL_USE_PAL
const PALConfig pal_default_config =
{
  {VAL_GPIOAODR, VAL_GPIOACRL, VAL_GPIOACRH},
  {VAL_GPIOBODR, VAL_GPIOBCRL, VAL_GPIOBCRH},
  {VAL_GPIOCODR, VAL_GPIOCCRL, VAL_GPIOCCRH},
  {VAL_GPIODODR, VAL_GPIODCRL, VAL_GPIODCRH},
  {VAL_GPIOEODR, VAL_GPIOECRL, VAL_GPIOECRH},
  {VAL_GPIOFODR, VAL_GPIOFCRL, VAL_GPIOFCRH},
  {VAL_GPIOGODR, VAL_GPIOGCRL, VAL_GPIOGCRH},
};
#endif

void __early_init(void) {
  stm32_clock_init();
}

void boardInit(void) {
}

