<!--
Copyright lowRISC Contributors.
SPDX-License-Identifier: Apache-2.0
-->
# Basic GPIO handling example

This code demostrates how to access the various hardware IO by:
1. Colour cycling the RGB LED
2. Falshing the on board LEDs
3. Responding to the Joystick
4. Switching GPIO27 of the Raspberry Pi header on and off at a slow rate to demostrate writing to the RPi header
5. GPIO12 of the Raspberry Pi header will periodically (every time the RGB LED goes dark purple) switch between 0% (off), 50% (1.5V) and 100% (3.3V). The demonstrate the use of the pinmux (pin multiplexor) and the PWM module.

The pinmux is particularly odd because it doesn't use MMIO_CAPABILITY, insetad you just create a new instance of the class. However, the pinmux is due to be upgraded shortly.

