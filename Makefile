

INCLUDE= ./include
CHIBIOS= $(INCLUDE)/chibios
GCC_ARM= $(INCLUDE)/gcc-arm-none-eabi-4_7-2012q4

BSP= $(INCLUDE)/liveview

BUILDDIR= ./bin
SRC= ./src
PROJECT= olv_fw

ifeq ($(USE_OPT),)
  USE_OPT = -O2 -ggdb -fomit-frame-pointer -falign-functions=16
endif

ifeq ($(USE_COPT),)
  USE_COPT = 
endif

ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -fno-rtti
endif

ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

ifeq ($(USE_THUMB),)
  USE_THUMB = yes
endif

ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = no
endif

ifeq ($(USE_FWLIB),)
  USE_FWLIB = no
endif

include $(BSP)/board.mk

include $(CHIBIOS)/os/hal/platforms/STM32F1xx/platform.mk
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/ports/GCC/ARMCMx/STM32F1xx/port.mk
include $(CHIBIOS)/os/kernel/kernel.mk
include $(CHIBIOS)/test/test.mk

include $(CHIBIOS)/ext/gfx/gfx.mk
include $(INCLUDE)/gdisp/S6E13B3/gdisp_lld.mk

include $(SRC)/olv_fw.mk

LDSCRIPT = $(BSP)/se_liveview_mn800.ld

CSRC = $(PORTSRC) \
       $(KERNSRC) \
       $(TESTSRC) \
       $(HALSRC) \
       $(PLATFORMSRC) \
       $(BOARDSRC) \
       $(CHIBIOS)/os/various/shell.c \
       $(CHIBIOS)/os/various/chprintf.c \
       $(GFXSRC) \
			 $(OLVSRC)

CPPSRC =

ACSRC =

ACPPSRC =

TCSRC =

TCPPSRC =

ASMSRC = $(PORTASM)

INCDIR = $(PORTINC) $(KERNINC) $(TESTINC) \
         $(HALINC) $(PLATFORMINC) $(BOARDINC) \
         $(CHIBIOS)/os/various $(GFXINC) $(OLVINC)

MCU  = cortex-m3

#TRGT = arm-elf-
TRGT = $(GCC_ARM)/bin/arm-none-eabi-
CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
LD   = $(TRGT)gcc
#LD   = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
OD   = $(TRGT)objdump
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

AOPT =
TOPT = -mthumb -DTHUMB

CWARN = -Wall -Wextra -Wstrict-prototypes
CPPWARN = -Wall -Wextra

DDEFS =
DADEFS =
DINCDIR =
DLIBDIR =
DLIBS =

UDEFS =
UADEFS =
UINCDIR =
ULIBDIR =
ULIBS =

ifeq ($(USE_FWLIB),yes)
  include $(CHIBIOS)/ext/stm32lib/stm32lib.mk
  CSRC += $(STM32SRC)
  INCDIR += $(STM32INC)
  USE_OPT += -DUSE_STDPERIPH_DRIVER
endif

include $(CHIBIOS)/os/ports/GCC/ARMCMx/rules.mk

copy:
	sudo dfu-util -d 0fce:f0fa -c 1 -i 0 -a 0 -s 0x08003000 -v -D ./bin/olv_fw.bin

orig:
	sudo dfu-util -d 0fce:f0fa -c 1 -i 0 -a 0 -s 0x08003000 -v -D ./original/firmware.bin

dump:
	sudo dfu-util -d 0fce:f0fa -c 1 -i 0 -a 0 -s 0x08000000 -v -U ./original/boot.bin
	sudo dfu-util -d 0fce:f0fa -c 1 -i 0 -a 0 -s 0x08003000 -v -U ./original/firmware.bin

term:
	sudo minicom -D /dev/ttyACM3

