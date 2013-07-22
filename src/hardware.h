
static PWMConfig pwm1_cfg = {
  10000,
  10000,
  NULL,
  {
   {PWM_OUTPUT_ACTIVE_LOW, NULL},
   {PWM_OUTPUT_ACTIVE_LOW, NULL},
   {PWM_OUTPUT_ACTIVE_LOW, NULL},
   {PWM_OUTPUT_ACTIVE_LOW, NULL}
  },
  0,
};

static PWMConfig pwm3_cfg = {
  10000,
  10000,
  NULL,
  {
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_ACTIVE_LOW, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
};

static PWMConfig pwm5_cfg = {
  10000,
  10000,
  NULL,
  {
   {PWM_OUTPUT_ACTIVE_LOW, NULL},
   {PWM_OUTPUT_ACTIVE_LOW, NULL},
   {PWM_OUTPUT_ACTIVE_LOW, NULL},
   {PWM_OUTPUT_ACTIVE_LOW, NULL}
  },
  0,
};

static const I2CConfig i2c1_cfg = {
    OPMODE_I2C,
    100000,
    FAST_DUTY_CYCLE_2,
};

static void ext_irq_charge_state_handler(EXTDriver *extp, expchannel_t channel) {
  (void)extp;
  (void)channel;

  if (palReadPad(GPIOC, GPIOC_CHARGE_DISABLED)) {
    pwmDisableChannel(&PWMD5, 1);

    pwmEnableChannel(&PWMD3, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000));
  } else {
    pwmDisableChannel(&PWMD3, 1);

    pwmEnableChannel(&PWMD5, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000));
  }
};

static void ext_irq_usb_detect_handler(EXTDriver *extp, expchannel_t channel) {
  (void)extp;
  (void)channel;
};

static void ext_irq_handler(EXTDriver *extp, expchannel_t channel) {
  (void)extp;
  (void)channel;

  //chprintf((BaseSequentialStream *)&SDU1, "irq %u\r\n", channel);
};

static const EXTConfig ext_cfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL},
/* battery charge #enabled */
    {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC, ext_irq_charge_state_handler},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},

    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
/* usb connect/disconnect */
    {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC, ext_irq_usb_detect_handler},
    {EXT_CH_MODE_DISABLED, NULL},

/* unknown */
    {EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB, ext_irq_handler},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},

    {EXT_CH_MODE_DISABLED, NULL},
/* i2c ext gpio. i2c ext gpio controller need reset after event but unknown how */
    {EXT_CH_MODE_RISING_EDGE  | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOC, ext_irq_handler},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
  }
};

