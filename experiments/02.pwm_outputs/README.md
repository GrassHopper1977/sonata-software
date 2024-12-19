<!--
Copyright lowRISC Contributors.
SPDX-License-Identifier: Apache-2.0
-->
# PWM Outputs example

This code configures the PWM outputs at different duty cycles and outputs them to as many devices as possible using the pinmux. We know the expected voltage so we can them measure this at the output with a volt meter and see what we get.

|       -      |     PWM0      |     PWM1      |     PWM2      |     PWM3      |     PWM4      |     PWM5      |
| ------------ | ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
|      RPi Out |    rph_g12    |    rph_g13    |    rph_g18    |    rph_g19    |    rph_g20    |    rph_g21    |
|  Arduino Out |   ah_tmpio3   |   ah_tmpio5   |   ah_tmpio6   |   ah_tmpio9   |  ah_tmpio10   |  ah_tmpio11   |
|     mikroBUS |     mb10      |               |       -       |       -       |       -       |       -       |
|        PMOD0 |       -       |    pmod0_2    |    pmod0_8    |       -       |       -       |       -       |
|        PMOD1 |       -       |       -       |       -       |    pmod1_2    |    pmod1_8    |       -       |
|        PMODC |       -       |       -       |       -       |       -       |       -       |       -       |
|  Duty Cycle  |      33       |      66       |      99       |      132      |      165      |      198      |
|     Voltage  |    0.5445     |    1.0890     |    1.6335     |    2.1780     |    2.7225     |    3.2670     |

## Results
|       -      |  PWM0   |  PWM1   |  PWM2   |  PWM3   |  PWM4   |  PWM5   |
| ------------ | ------- | ------- | ------- | ------- | ------- | ------- |
|     Expected |  0.54V  |  1.09V  |  1.63V  |  2.18V  |  2.72V  |  3.27V  |
|      RPi Out |  0.55V  |  0.30V  |  0.30V  |  0.29V  |  0.31V  |  0.28V  |
|  Arduino Out |  0.54V  |  0.33V  |  0.30V  |  0.30V  |  0.26V  |  0.29V  |
|     mikroBUS |  0.55V  |    -    |    -    |    -    |    -    |    -    |
|        PMOD0 |    -    |  0.32V  |  0.30V  |    -    |    -    |    -    |
|        PMOD1 |    -    |    -    |    -    |  0.30V  |  0.30V  |    -    |
|        PMODC |    -    |    -    |    -    |    -    |    -    |    -    |

## Conclusion
There looks to be an issue with the PWM module. Alex Jones (whi is working on the Sonata) says it's an RTL bug and will be fixed in V1.1.