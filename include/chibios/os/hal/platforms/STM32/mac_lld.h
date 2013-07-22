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
 * @file    STM32/mac_lld.h
 * @brief   STM32 low level MAC driver header.
 *
 * @addtogroup MAC
 * @{
 */

#ifndef _MAC_LLD_H_
#define _MAC_LLD_H_

#if HAL_USE_MAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    RDES0 constants
 * @{
 */
#define STM32_RDES0_OWN             0x80000000
#define STM32_RDES0_AFM             0x40000000
#define STM32_RDES0_FL_MASK         0x3FFF0000
#define STM32_RDES0_ES              0x00008000
#define STM32_RDES0_DESERR          0x00004000
#define STM32_RDES0_SAF             0x00002000
#define STM32_RDES0_LE              0x00001000
#define STM32_RDES0_OE              0x00000800
#define STM32_RDES0_VLAN            0x00000400
#define STM32_RDES0_FS              0x00000200
#define STM32_RDES0_LS              0x00000100
#define STM32_RDES0_IPHCE           0x00000080
#define STM32_RDES0_LCO             0x00000040
#define STM32_RDES0_FT              0x00000020
#define STM32_RDES0_RWT             0x00000010
#define STM32_RDES0_RE              0x00000008
#define STM32_RDES0_DE              0x00000004
#define STM32_RDES0_CE              0x00000002
#define STM32_RDES0_PCE             0x00000001
/** @} */

/**
 * @name    RDES1 constants
 * @{
 */
#define STM32_RDES1_DIC             0x80000000
#define STM32_RDES1_RBS2_MASK       0x1FFF0000
#define STM32_RDES1_RER             0x00008000
#define STM32_RDES1_RCH             0x00004000
#define STM32_RDES1_RBS1_MASK       0x00001FFF
/** @} */

/**
 * @name    TDES0 constants
 * @{
 */
#define STM32_TDES0_OWN             0x80000000
#define STM32_TDES0_IC              0x40000000
#define STM32_TDES0_LS              0x20000000
#define STM32_TDES0_FS              0x10000000
#define STM32_TDES0_DC              0x08000000
#define STM32_TDES0_DP              0x04000000
#define STM32_TDES0_TTSE            0x02000000
#define STM32_TDES0_LOCKED          0x01000000 /* NOTE: Pseudo flag.        */
#define STM32_TDES0_CIC_MASK        0x00C00000
#define STM32_TDES0_CIC(n)          ((n) << 22)
#define STM32_TDES0_TER             0x00200000
#define STM32_TDES0_TCH             0x00100000
#define STM32_TDES0_TTSS            0x00020000
#define STM32_TDES0_IHE             0x00010000
#define STM32_TDES0_ES              0x00008000
#define STM32_TDES0_JT              0x00004000
#define STM32_TDES0_FF              0x00002000
#define STM32_TDES0_IPE             0x00001000
#define STM32_TDES0_LCA             0x00000800
#define STM32_TDES0_NC              0x00000400
#define STM32_TDES0_LCO             0x00000200
#define STM32_TDES0_EC              0x00000100
#define STM32_TDES0_VF              0x00000080
#define STM32_TDES0_CC_MASK         0x00000078
#define STM32_TDES0_ED              0x00000004
#define STM32_TDES0_UF              0x00000002
#define STM32_TDES0_DB              0x00000001
/** @} */

/**
 * @name    TDES1 constants
 * @{
 */
#define STM32_TDES1_TBS2_MASK       0x1FFF0000
#define STM32_TDES1_TBS1_MASK       0x00001FFF
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Number of available transmit buffers.
 */
#if !defined(STM32_MAC_TRANSMIT_BUFFERS) || defined(__DOXYGEN__)
#define STM32_MAC_TRANSMIT_BUFFERS  2
#endif

/**
 * @brief   Number of available receive buffers.
 */
#if !defined(STM32_MAC_RECEIVE_BUFFERS) || defined(__DOXYGEN__)
#define STM32_MAC_RECEIVE_BUFFERS   4
#endif

/**
 * @brief   Maximum supported frame size.
 */
#if !defined(STM32_MAC_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define STM32_MAC_BUFFERS_SIZE      1522
#endif

/**
 * @brief   PHY detection timeout.
 * @details Timeout, in milliseconds, for PHY address detection, if a PHY
 *          is not detected within the timeout then the driver halts during
 *          initialization. This setting applies only if the PHY address is
 *          not explicitly set in the board header file using
 *          @p BOARD_PHY_ADDRESS. A zero value disables the timeout and a
 *          single search path is performed.
 */
#if !defined(STM32_MAC_PHY_TIMEOUT) || defined(__DOXYGEN__)
#define STM32_MAC_PHY_TIMEOUT       100
#endif

/**
 * @brief   Change the PHY power state inside the driver.
 */
#if !defined(STM32_ETH1_CHANGE_PHY_STATE) || defined(__DOXYGEN__)
#define STM32_ETH1_CHANGE_PHY_STATE TRUE
#endif

/**
 * @brief   ETHD1 interrupt priority level setting.
 */
#if !defined(STM32_ETH1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_ETH1_IRQ_PRIORITY     13
#endif

/**
 * @brief   IP checksum offload.
 * @details The following modes are available:
 *          - 0 Function disabled.
 *          - 1 Only IP header checksum calculation and insertion are enabled.
 *          - 2 IP header checksum and payload checksum calculation and
 *              insertion are enabled, but pseudo-header checksum is not
 *              calculated in hardware.
 *          - 3 IP Header checksum and payload checksum calculation and
 *              insertion are enabled, and pseudo-header checksum is
 *              calculated in hardware.
 *          .
 */
#if !defined(STM32_IP_CHECKSUM_OFFLOAD) || defined(__DOXYGEN__)
#define STM32_IP_CHECKSUM_OFFLOAD   0
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (STM32_MAC_PHY_TIMEOUT > 0) && !HAL_IMPLEMENTS_COUNTERS
#error "STM32_MAC_PHY_TIMEOUT requires the realtime counter service"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of an STM32 Ethernet receive descriptor.
 */
typedef struct {
  volatile uint32_t     rdes0;
  volatile uint32_t     rdes1;
  volatile uint32_t     rdes2;
  volatile uint32_t     rdes3;
} stm32_eth_rx_descriptor_t;

/**
 * @brief   Type of an STM32 Ethernet transmit descriptor.
 */
typedef struct {
  volatile uint32_t     tdes0;
  volatile uint32_t     tdes1;
  volatile uint32_t     tdes2;
  volatile uint32_t     tdes3;
} stm32_eth_tx_descriptor_t;

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  /**
   * @brief MAC address.
   */
  uint8_t               *mac_address;
  /* End of the mandatory fields.*/
} MACConfig;

/**
 * @brief   Structure representing a MAC driver.
 */
struct MACDriver {
  /**
   * @brief Driver state.
   */
  macstate_t            state;
  /**
   * @brief Current configuration data.
   */
  const MACConfig       *config;
  /**
   * @brief Transmit semaphore.
   */
  Semaphore             tdsem;
  /**
   * @brief Receive semaphore.
   */
  Semaphore             rdsem;
#if MAC_USE_EVENTS || defined(__DOXYGEN__)
  /**
   * @brief Receive event.
   */
  EventSource           rdevent;
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Link status flag.
   */
  bool_t                link_up;
  /**
   * @brief PHY address (pre shifted).
   */
  uint32_t phyaddr;
  /**
   * @brief Receive next frame pointer.
   */
  stm32_eth_rx_descriptor_t *rxptr;
  /**
   * @brief Transmit next frame pointer.
   */
  stm32_eth_tx_descriptor_t *txptr;
};

/**
 * @brief   Structure representing a transmit descriptor.
 */
typedef struct {
  /**
   * @brief Current write offset.
   */
  size_t                    offset;
  /**
   * @brief Available space size.
   */
  size_t                    size;
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the physical descriptor.
   */
  stm32_eth_tx_descriptor_t *physdesc;
} MACTransmitDescriptor;

/**
 * @brief   Structure representing a receive descriptor.
 */
typedef struct {
  /**
   * @brief Current read offset.
   */
  size_t                offset;
  /**
   * @brief Available data size.
   */
  size_t                size;
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the physical descriptor.
   */
  stm32_eth_rx_descriptor_t *physdesc;
} MACReceiveDescriptor;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern MACDriver ETHD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void mac_lld_init(void);
  void mac_lld_start(MACDriver *macp);
  void mac_lld_stop(MACDriver *macp);
  msg_t mac_lld_get_transmit_descriptor(MACDriver *macp,
                                        MACTransmitDescriptor *tdp);
  size_t mac_lld_write_transmit_descriptor(MACTransmitDescriptor *tdp,
                                           uint8_t *buf,
                                           size_t size);
  void mac_lld_release_transmit_descriptor(MACTransmitDescriptor *tdp);
  msg_t mac_lld_get_receive_descriptor(MACDriver *macp,
                                       MACReceiveDescriptor *rdp);
  size_t mac_lld_read_receive_descriptor(MACReceiveDescriptor *rdp,
                                         uint8_t *buf,
                                         size_t size);
  void mac_lld_release_receive_descriptor(MACReceiveDescriptor *rdp);
  bool_t mac_lld_poll_link_status(MACDriver *macp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_MAC */

#endif /* _MAC_LLD_H_ */

/** @} */
