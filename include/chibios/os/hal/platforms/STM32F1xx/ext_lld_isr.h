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
 * @file    STM32F1xx/ext_lld_isr.h
 * @brief   STM32F1xx EXT subsystem low level driver ISR header.
 *
 * @addtogroup EXT
 * @{
 */

#ifndef _EXT_LLD_ISR_H_
#define _EXT_LLD_ISR_H_

#if HAL_USE_EXT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   EXTI0 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI0_IRQ_PRIORITY        6
#endif

/**
 * @brief   EXTI1 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI1_IRQ_PRIORITY        6
#endif

/**
 * @brief   EXTI2 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI2_IRQ_PRIORITY        6
#endif

/**
 * @brief   EXTI3 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI3_IRQ_PRIORITY        6
#endif

/**
 * @brief   EXTI4 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI4_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI4_IRQ_PRIORITY        6
#endif

/**
 * @brief   EXTI9..5 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI5_9_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI5_9_IRQ_PRIORITY      6
#endif

/**
 * @brief   EXTI15..10 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI10_15_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI10_15_IRQ_PRIORITY    6
#endif

/**
 * @brief   EXTI16 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI16_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI16_IRQ_PRIORITY       6
#endif

/**
 * @brief   EXTI17 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI17_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI17_IRQ_PRIORITY       6
#endif

/**
 * @brief   EXTI18 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI18_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI18_IRQ_PRIORITY       6
#endif

/**
 * @brief   EXTI19 interrupt priority level setting.
 */
#if !defined(STM32_EXT_EXTI19_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EXT_EXTI19_IRQ_PRIORITY       6
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void ext_lld_exti_irq_enable(void);
  void ext_lld_exti_irq_disable(void);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_EXT */

#endif /* _EXT_LLD_ISR_H_ */

/** @} */
