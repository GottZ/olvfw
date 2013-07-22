# List of all the STM32F3xx platform files.
PLATFORMSRC = ${CHIBIOS}/os/hal/platforms/SAM4L/hal_lld.c \
              ${CHIBIOS}/os/hal/platforms/SAM4L/asf/flashcalw.c

# Required include directories
PLATFORMINC = ${CHIBIOS}/os/hal/platforms/SAM4L \
              ${CHIBIOS}/os/hal/platforms/SAM4L/asf
