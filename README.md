OpenLiveview Firmware
=====

OpenLiveView, Open Source projects for Sony Ericsson's LiveView smartwatch: http://openliveview.com | XDA-developers thread: http://forum.xda-developers.com/showthread.php?t=1422106

tools you need to install in order to make use of it:
(it should work from scratch on almost any linux distribution)

- make
- gcc-arm-none-eabi
- dfu-util
- minicom
  - (well.. actually you do not really need it unless you want to make use of the currently disabled usb terminal)


do not forget to do `make dump` before flashing a new firmware. please take a look at the bottom of `Makefile`
