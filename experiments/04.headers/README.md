<!--
Copyright lowRISC Contributors.
SPDX-License-Identifier: Apache-2.0
-->
# Basic GPIO handling example

This is a simple test to create an oscillating On/Off pattern on the GPIO on teh headers to test that they work. The motivation is that we're not seeing all the signals on the RPi Header when I'm trying to get SPI working (see our 03.basic_spi" example). We also had some weirdness with our "01.general_io" example which required us to move our test output pin, so maybe there is an issue here. Tghis shoudl help us find any potential issues.

The plan is to set all the RPi header GPIO to outputs. We will then output a square wave on GPIO0. The joystick left and right will be used to change which output we are driving so that they can all be tested to check that they all work. We will test the outputs with a multimeter or logic analyser.

| RPi GPIO |  RPi Header # | Output? |
| -------- | ------------- | ------- |
|     0    |       27      |   3V3   | 
|     1    |       28      |   3V3   | 	
|     2    |        3      |   3V3   | 	
|     3    |        5      |   3V3   |	
|     4    |        7      |   3V3   |	
|     5    |       29      |   3V3   |	
|     6    |       31      |   Yes   |	
|     7    |       26      |   3V3   |	
|     8    |       24      |   3V3   |	
|     9    |       21      |    0V   |	
|    10    |       19      |    0V   |	
|    11    |       23      |    0V   |	
|    12    |       32      |   Yes   |	
|    13    |       33      |   Yes   |	
|    14    |        8      |   3V3   |	
|    15    |       10      |   Yes   |	
|    16    |       36      |   3V3   |	
|    17    |       11      |   3V3   |	
|    18    |       12      |   3V3   |	
|    19    |       35      |   Yes   |	
|    20    |       38      |    0V   |	
|    21    |       40      |    0V   |	
|    22    |       15      |   Yes   |	
|    23    |       16      |    0V   |	
|    24    |       18      |   Yes   |	
|    25    |       22      |   Yes   |	
|    26    |       37      |   3V3   |	
|    27    |       13      |   Yes   |

