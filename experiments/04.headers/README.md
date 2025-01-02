<!--
Copyright lowRISC Contributors.
SPDX-License-Identifier: Apache-2.0
-->
# Basic GPIO handling example

This is a simple test to create an oscillating On/Off pattern on the GPIO on teh headers to test that they work. The motivation is that we're not seeing all the signals on the RPi Header when I'm trying to get SPI working (see our 03.basic_spi" example). We also had some weirdness with our "01.general_io" example which required us to move our test output pin, so maybe there is an issue here. Tghis shoudl help us find any potential issues.

The plan is to set all the RPi header GPIO to outputs. We will then output a square wave on GPIO0. The joystick left and right will be used to change which output we are driving so that they can all be tested to check that they all work. We will test the outputs with a multimeter or logic analyser.

This table records if we got our expected squarewave output or something else instead. We then repeated teh test, this time we explicitly set the pinmux to GPIO for each output.

| RPi GPIO |  RPi Header # | No pinmux | With Pinmux |
| -------- | ------------- | ------- | ------ |
|     0    |       27      |   3V3   |   Yes  |
|     1    |       28      |   3V3   | 	 Yes  |
|     2    |        3      |   3V3   | 	 Yes  |
|     3    |        5      |   3V3   |	 Yes  |
|     4    |        7      |   3V3   |	 Yes  |
|     5    |       29      |   3V3   |	 Yes  |
|     6    |       31      |   Yes   |	 Yes  |
|     7    |       26      |   3V3   |	 Yes  |
|     8    |       24      |   3V3   |	 Yes  |
|     9    |       21      |    0V   |	 Yes  |
|    10    |       19      |    0V   |	 Yes  |
|    11    |       23      |    0V   |	 Yes  |
|    12    |       32      |   Yes   |	 Yes  |
|    13    |       33      |   Yes   |	 Yes  |
|    14    |        8      |   3V3   |	 Yes  |
|    15    |       10      |   Yes   |	 Yes  |
|    16    |       36      |   3V3   |	 Yes  |
|    17    |       11      |   3V3   |	 Yes  |
|    18    |       12      |   3V3   |	 Yes  |
|    19    |       35      |   Yes   |	 Yes  |
|    20    |       38      |    0V   |	 Yes  |
|    21    |       40      |    0V   |	 Yes  |
|    22    |       15      |   Yes   |	 Yes  |
|    23    |       16      |    0V   |	 Yes  |
|    24    |       18      |   Yes   |	 Yes  |
|    25    |       22      |   Yes   |	 Yes  |
|    26    |       37      |   3V3   |	 Yes  |
|    27    |       13      |   Yes   |   Yes  |

You can see that, if we don't use the pinmux, the pins do not function as expected. With the pinmux it all functions correctly.