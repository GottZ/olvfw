/*
    ChibiOS/GFX - Copyright (C) 2012
			Joel Bodenmann aka Tectu <joel@unormal.org>

    This file is part of ChibiOS/GFX.

    ChibiOS/GFX is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/GFX is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SSD1963_H
#define SSD1963_H

#include "gdisp_lld_panel.h"

#if defined(GDISP_USE_GPIO)
	#define Set_CS		palSetPad(GDISP_CMD_PORT, GDISP_CS);
	#define Clr_CS		palClearPad(GDISP_CMD_PORT, GDISP_CS);
	#define Set_RS		palSetPad(GDISP_CMD_PORT, GDISP_RS);
	#define Clr_RS		palClearPad(GDISP_CMD_PORT, GDISP_RS);
	#define Set_WR		palSetPad(GDISP_CMD_PORT, GDISP_WR);
	#define Clr_WR		palClearPad(GDISP_CMD_PORT, GDISP_WR);
	#define Set_RD		palSetPad(GDISP_CMD_PORT, GDISP_RD);
	#define Clr_RD		palClearPad(GDISP_CMD_PORT, GDISP_RD);
#endif

#if defined(GDISP_USE_FSMC)
	/* Using FSMC A16 as RS */
	#define GDISP_REG              (*((volatile uint16_t *) 0x60000000)) /* RS = 0 */
	#define GDISP_RAM              (*((volatile uint16_t *) 0x60020000)) /* RS = 1 */
#endif

#define mHIGH(x) (x >> 8)
#define mLOW(x) (x & 0xFF)

/* SSD1963 commands */

#define SSD1963_NOP						0x0000
#define SSD1963_SOFT_RESET  			0x0001
#define SSD1963_GET_POWER_MODE 			0x000A
#define SSD1963_GET_ADDRESS_MODE		0x000B
#define SSD1963_GET_DISPLAY_MODE		0x000D
#define SSD1963_GET_TEAR_EFFECT_STATUS 	0x000E
#define SSD1963_ENTER_SLEEP_MODE		0x0010
#define SSD1963_EXIT_SLEEP_MODE			0x0011
#define SSD1963_ENTER_PARTIAL_MODE		0x0012
#define SSD1963_ENTER_NORMAL_MODE		0x0013
#define SSD1963_EXIT_INVERT_MODE		0x0020
#define SSD1963_ENTER_INVERT_MODE		0x0021
#define SSD1963_SET_GAMMA_CURVE			0x0026
#define SSD1963_SET_DISPLAY_OFF			0x0028
#define SSD1963_SET_DISPLAY_ON			0x0029
#define SSD1963_SET_COLUMN_ADDRESS		0x002A
#define SSD1963_SET_PAGE_ADDRESS		0x002B
#define SSD1963_WRITE_MEMORY_START		0x002C
#define SSD1963_READ_MEMORY_START		0x002E
#define SSD1963_SET_PARTIAL_AREA		0x0030
#define SSD1963_SET_SCROLL_AREA			0x0033
#define SSD1963_SET_TEAR_OFF			0x0034
#define SSD1963_SET_TEAR_ON				0x0035
#define SSD1963_SET_ADDRESS_MODE		0x0036
#define SSD1963_SET_SCROLL_START		0x0037
#define SSD1963_EXIT_IDLE_MODE			0x0038
#define SSD1963_ENTER_IDLE_MODE			0x0039
#define SSD1963_SET_PIXEL_FORMAT		0x003A
#define SSD1963_WRITE_MEMORY_CONTINUE	0x003C
#define SSD1963_READ_MEMORY_CONTINUE	0x003E
#define SSD1963_SET_TEAR_SCANLINE		0x0044
#define SSD1963_GET_SCANLINE			0x0045
#define SSD1963_READ_DDB				0x00A1
#define SSD1963_SET_GDISP_MODE			0x00B0
#define SSD1963_GET_GDISP_MODE			0x00B1
#define SSD1963_SET_HORI_PERIOD			0x00B4
#define SSD1963_GET_HORI_PERIOD			0x00B5
#define SSD1963_SET_VERT_PERIOD			0x00B6
#define SSD1963_GET_VERT_PERIOD			0x00B7
#define SSD1963_SET_GPIO_CONF			0x00B8
#define SSD1963_GET_GPIO_CONF			0x00B9
#define SSD1963_SET_GPIO_VALUE			0x00BA
#define SSD1963_GET_GPIO_STATUS			0x00BB
#define SSD1963_SET_POST_PROC			0x00BC
#define SSD1963_GET_POST_PROC			0x00BD
#define SSD1963_SET_PWM_CONF			0x00BE
#define SSD1963_GET_PWM_CONF			0x00BF
#define SSD1963_GET_GDISP_GEN0			0x00C0
#define SSD1963_SET_GDISP_GEN0			0x00C1
#define SSD1963_GET_GDISP_GEN1			0x00C2
#define SSD1963_SET_GDISP_GEN1			0x00C3
#define SSD1963_GET_GDISP_GEN2			0x00C4
#define SSD1963_SET_GDISP_GEN2			0x00C5
#define SSD1963_GET_GDISP_GEN3			0x00C6
#define SSD1963_SET_GDISP_GEN3			0x00C7
#define SSD1963_SET_GPIO0_ROP			0x00C8
#define SSD1963_GET_GPIO0_ROP			0x00C9
#define SSD1963_SET_GPIO1_ROP			0x00CA
#define SSD1963_GET_GPIO1_ROP			0x00CB
#define SSD1963_SET_GPIO2_ROP			0x00CC
#define SSD1963_GET_GPIO2_ROP			0x00CD
#define SSD1963_SET_GPIO3_ROP			0x00CE
#define SSD1963_GET_GPIO3_ROP			0x00CF
#define SSD1963_SET_DBC_CONF			0x00D0
#define SSD1963_GET_DBC_CONF			0x00D1
#define SSD1963_SET_DBC_TH				0x00D4
#define SSD1963_GET_DBC_TH				0x00D5
#define SSD1963_SET_PLL					0x00E0
#define SSD1963_SET_PLL_MN				0x00E2
#define SSD1963_GET_PLL_MN				0x00E3
#define SSD1963_GET_PLL_STATUS			0x00E4
#define SSD1963_SET_DEEP_SLEEP			0x00E5
#define SSD1963_SET_LSHIFT_FREQ			0x00E6
#define SSD1963_GET_LSHIFT_FREQ			0x00E7
#define SSD1963_SET_PIXEL_DATA_INTERFACE 0x00F0
	#define SSD1963_PDI_8BIT			0
	#define SSD1963_PDI_12BIT			1
	#define SSD1963_PDI_16BIT			2
	#define SSD1963_PDI_16BIT565		3
	#define SSD1963_PDI_18BIT			4
	#define SSD1963_PDI_24BIT			5
	#define SSD1963_PDI_9BIT			6
#define SSD1963_GET_PIXEL_DATA_INTERFACE 0x00F1

#endif

