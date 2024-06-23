# Change Log {#ChangeLog}
All notable changes to this project will be documented in this file.
 
The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [1.0.1] - 2024-06-23

### Added

- Enable check functions for timer interrupts. (timer module)

### Fixed

- Convenient IRQ implementations now use if-else-if conditional statements instead of successive if statements for multiple interrupts. (timer module)
- Convenient IRQ implementations now check if the interrupts are enabled as well, if multiple interrupts are handled in the body. (timer module)
 
## [1.0.0] - 2024-06-22
  
### Added
 
- Timer/interrupt utility module for STM32F401xC and STM32F103xB.
- MIT License.
