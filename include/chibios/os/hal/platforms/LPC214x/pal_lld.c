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
 * @file    LPC214x/pal_lld.c
 * @brief   LPC214x FIO low level driver code.
 *
 * @addtogroup PAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   LPC214x I/O ports configuration.
 * @details FIO units and PINSEL registers initialization.
 *
 * @param[in] config    the LPC214x ports configuration
 *
 * @notapi
 */
void _pal_lld_init(const PALConfig *config) {

  /* Enables the access through the fast registers.*/
  SCS = 3;

  /* I/O pads initial assignment, device drivers may change this setup at a
   * later time.*/
  PINSEL0 = config->pinsel0;
  PINSEL1 = config->pinsel1;
  PINSEL2 = config->pinsel2;

  /* I/O pads direction initial setting.*/
  FIO0Base->FIO_MASK = 0;
  FIO0Base->FIO_PIN = config->P0Data.pin;
  FIO0Base->FIO_DIR = config->P0Data.dir;
  FIO1Base->FIO_MASK = 0;
  FIO1Base->FIO_PIN = config->P1Data.pin;
  FIO1Base->FIO_DIR = config->P1Data.dir;
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    @p PAL_MODE_UNCONNECTED is implemented as push pull output with
 *          high state.
 * @note    This function does not alter the @p PINSELx registers. Alternate
 *          functions setup must be handled by device-specific code.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] mode      the mode
 *
 * @notapi
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           iomode_t mode) {

  switch (mode) {
  case PAL_MODE_RESET:
  case PAL_MODE_INPUT:
    port->FIO_DIR &= ~mask;
    break;
  case PAL_MODE_UNCONNECTED:
    port->FIO_PIN |= mask;
  case PAL_MODE_OUTPUT_PUSHPULL:
    port->FIO_DIR |= mask;
    break;
  }
}

#endif /* HAL_USE_PAL */

/** @} */
