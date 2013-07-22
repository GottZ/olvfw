/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    LPC11xx/serial_lld.h
 * @brief   LPC11xx low level serial driver header.
 *
 * @addtogroup SERIAL
 * @{
 */

#ifndef _SERIAL_LLD_H_
#define _SERIAL_LLD_H_

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define IIR_SRC_MASK    0x0F
#define IIR_SRC_NONE    0x01
#define IIR_SRC_MODEM   0x00
#define IIR_SRC_TX      0x02
#define IIR_SRC_RX      0x04
#define IIR_SRC_ERROR   0x06
#define IIR_SRC_TIMEOUT 0x0C

#define IER_RBR         1
#define IER_THRE        2
#define IER_STATUS      4

#define LCR_WL5         0
#define LCR_WL6         1
#define LCR_WL7         2
#define LCR_WL8         3
#define LCR_STOP1       0
#define LCR_STOP2       4
#define LCR_NOPARITY    0
#define LCR_PARITYODD   0x08
#define LCR_PARITYEVEN  0x18
#define LCR_PARITYONE   0x28
#define LCR_PARITYZERO  0x38
#define LCR_BREAK_ON    0x40
#define LCR_DLAB        0x80

#define FCR_ENABLE      1
#define FCR_RXRESET     2
#define FCR_TXRESET     4
#define FCR_TRIGGER0    0
#define FCR_TRIGGER1    0x40
#define FCR_TRIGGER2    0x80
#define FCR_TRIGGER3    0xC0

#define LSR_RBR_FULL    1
#define LSR_OVERRUN     2
#define LSR_PARITY      4
#define LSR_FRAMING     8
#define LSR_BREAK       0x10
#define LSR_THRE        0x20
#define LSR_TEMT        0x40
#define LSR_RXFE        0x80

#define TER_ENABLE      0x80

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   UART0 driver enable switch.
 * @details If set to @p TRUE the support for UART0 is included.
 * @note    The default is @p TRUE .
 */
#if !defined(LPC11xx_SERIAL_USE_UART0) || defined(__DOXYGEN__)
#define LPC11xx_SERIAL_USE_UART0            TRUE
#endif

/**
 * @brief   FIFO preload parameter.
 * @details Configuration parameter, this values defines how many bytes are
 *          preloaded in the HW transmit FIFO for each interrupt, the maximum
 *          value is 16 the minimum is 1.
 * @note    An high value reduces the number of interrupts generated but can
 *          also increase the worst case interrupt response time because the
 *          preload loops.
 */
#if !defined(LPC11xx_SERIAL_FIFO_PRELOAD) || defined(__DOXYGEN__)
#define LPC11xx_SERIAL_FIFO_PRELOAD         16
#endif

/**
 * @brief   UART0 PCLK divider.
 */
#if !defined(LPC11xx_SERIAL_UART0CLKDIV) || defined(__DOXYGEN__)
#define LPC11xx_SERIAL_UART0CLKDIV          1
#endif

/**
 * @brief   UART0 interrupt priority level setting.
 */
#if !defined(LPC11xx_SERIAL_UART0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC11xx_SERIAL_UART0_IRQ_PRIORITY   3
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (LPC11xx_SERIAL_UART0CLKDIV < 1) || (LPC11xx_SERIAL_UART0CLKDIV > 255)
#error "invalid LPC11xx_SERIAL_UART0CLKDIV setting"
#endif

#if (LPC11xx_SERIAL_FIFO_PRELOAD < 1) || (LPC11xx_SERIAL_FIFO_PRELOAD > 16)
#error "invalid LPC11xx_SERIAL_FIFO_PRELOAD setting"
#endif

/**
 * @brief   UART0 clock.
 */
#define  LPC11xx_SERIAL_UART0_PCLK                                          \
  (LPC11xx_MAINCLK / LPC11xx_SERIAL_UART0CLKDIV)

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   LPC11xx Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 */
typedef struct {
  /**
   * @brief Bit rate.
   */
  uint32_t                  sc_speed;
  /**
   * @brief Initialization value for the LCR register.
   */
  uint32_t                  sc_lcr;
  /**
   * @brief Initialization value for the FCR register.
   */
  uint32_t                  sc_fcr;
} SerialConfig;

/**
 * @brief   @p SerialDriver specific data.
 */
#define _serial_driver_data                                                 \
  _base_asynchronous_channel_data                                           \
  /* Driver state.*/                                                        \
  sdstate_t                 state;                                          \
  /* Input queue.*/                                                         \
  InputQueue                iqueue;                                         \
  /* Output queue.*/                                                        \
  OutputQueue               oqueue;                                         \
  /* Input circular buffer.*/                                               \
  uint8_t                   ib[SERIAL_BUFFERS_SIZE];                        \
  /* Output circular buffer.*/                                              \
  uint8_t                   ob[SERIAL_BUFFERS_SIZE];                        \
  /* End of the mandatory fields.*/                                         \
  /* Pointer to the USART registers block.*/                                \
  LPC_UART_TypeDef        *uart;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if LPC11xx_SERIAL_USE_UART0 && !defined(__DOXYGEN__)
extern SerialDriver SD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp, const SerialConfig *config);
  void sd_lld_stop(SerialDriver *sdp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SERIAL */

#endif /* _SERIAL_LLD_H_ */

/** @} */
