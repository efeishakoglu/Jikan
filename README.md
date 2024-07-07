Overview
========
<img src="https://raw.githubusercontent.com/ushumgigal/hierodule/main/hierodule.png" />
<br><br>Hierodule is a utility module set for STM32 MCUs.
<br>Link to detailed documentation: https://ushumgigal.github.io/hierodule

Aim and Scope of the Project
============================
- The primary focus is to put together a set of peripheral utility routines for STM32 MCUs to make it simpler and more managable to develop on STM devices, especially during a prototyping phase.
- The project aims to:
  - Support multiple devices (with priority given to popular ones).
  - Seamlessly integrate into projects that utilize low-level libraries, such as HAL and LL.
- The project does not aim to:
  - Mimic the low-level functionality of available libraries.
  - Replace HAL, LL or any other low-level library, since the goal is to provide a relatively higher level approach to implement practical use of peripherals.
- Additional modules and devices will be integrated as the project continues to develop. Here's a progress chart of modules per supported device.

|             |  Timer |  USART  |  ADC   |  I2C  |  SPI  |  FREERTOS  |  USB  |
|:-----------:|:------:|:-------:|:------:|:-----:|:-----:|:----------:|:-----:|
| STM32F103xB | ≥1.0.0 |  ≥1.2.0 | ≥1.3.0 |  WIP  |  TBA  |    TBA     |  TBA  |
| STM32F401xC | ≥1.0.0 |  ≥1.2.0 | ≥1.3.0 |  WIP  |  TBA  |    TBA     |  TBA  |
| STM32F030x6 | ≥1.1.0 |  ≥1.2.0 | ≥1.3.0 |  WIP  |  TBA  |    TBA     |  TBA  |

Compatibility
=============
The modules are designed for compatibility with existing device driver libraries included in the embedded software packages owned and maintained by [STMicroelectronics](https://github.com/STMicroelectronics).
|                            Package                               | Compatible Package Version |
|:----------------------------------------------------------------:|:--------------------------:|
| [STM32CubeF0](https://github.com/STMicroelectronics/STM32CubeF0) |          v1.11.5           |
| [STM32CubeF1](https://github.com/STMicroelectronics/STM32CubeF1) |          v1.8.5            |
| [STM32CubeF4](https://github.com/STMicroelectronics/STM32CubeF4) |          v1.28.0           |

Attention
=========
- The source code is copyrighted (2024) by [ushumgigal](https://github.com/ushumgigal) under MIT License, a copy of which may be found in the root folder of the [repository](https://github.com/ushumgigal/hierodule).
- This is not a derivative work and does not include or modify any priorly copyrighted work in source or object form.

Contact
=======
atilla_aricioglu@yahoo.com
