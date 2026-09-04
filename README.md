# Car-Black-Box
Embedded C based Car Black Box using PIC16F877A

## 📌 Project Overview

The **Car Black Box** is an embedded system developed using the **PIC16F877A microcontroller** to monitor vehicle speed, record important events, and store the recorded information for later viewing.

The system uses a **16×2 Character LCD, digital keypad, RTC, EEPROM, ADC, UART, and timers** to provide real-time monitoring and event logging.

## 🛠️ Technologies & Hardware

* **Embedded C**
* **PIC16F877A Microcontroller**
* **MPLAB X IDE**
* **XC8 Compiler**
* **Makefile**
* **16×2 Character LCD**
* **Digital Keypad**
* **ADC**
* **UART**
* **I2C Communication**
* **DS1307 RTC**
* **External EEPROM**
* **Timer0**
* **Interrupt Service Routine (ISR)**

## ⚙️ Features

* 🚗 Real-time vehicle speed monitoring
* 🕒 Date and time display using DS1307 RTC
* 📝 Event detection and logging
* 💾 Stores event logs in external EEPROM
* 🔐 Password-protected menu
* 📋 View stored logs
* ⌨️ Keypad-based menu navigation
* 📺 16×2 LCD-based user interface
* 📡 UART communication
* ⚡ Interrupt-based event handling

## 🔄 System Working

1. The **PIC16F877A** initializes the required peripherals.
2. Vehicle speed is obtained using the **ADC**.
3. The **DS1307 RTC** provides the current date and time through I2C communication.
4. Important vehicle events are detected by the system.
5. The event information along with speed and time is stored in **external EEPROM**.
6. **Interrupt Service Routines (ISR)** handle time-critical events.
7. The **UART** interface is used for serial communication/debugging.
8. The user navigates through the menu using the digital keypad.
9. Stored logs are retrieved from EEPROM and displayed on the LCD.

## 🧩 Main Modules

| Module                  | Description                                     |
| ----------------------- | ----------------------------------------------- |
| `main.c`                | Main application and program flow               |
| `cbb.c / cbb.h`         | Car Black Box application functions             |
| `digital_keypad.c / .h` | Digital keypad driver                           |
| `clcd.c / .h`           | Character LCD driver                            |
| `ADC.c / .h`            | ADC configuration and speed reading             |
| `DS1307.c / .h`         | RTC communication                               |
| `i2c.c / .h`            | I2C communication                               |
| `eeprom.c / .h`         | External EEPROM read/write operations           |
| `uart.c / .h`           | UART initialization and serial communication    |
| `timers.c / .h`         | Timer configuration                             |
| `isr.c / .h`            | Interrupt Service Routine handling              |
| `Makefile`              | Automates project compilation and build process |

## 💾 EEPROM & Event Logging

The system stores important vehicle information in **external EEPROM** so that the recorded data is retained even after power is removed.

Each log contains information such as:

```text
Time | Event | Speed
```

The system can:

* Write event information to EEPROM
* Read previously stored events
* Navigate between stored logs
* Display logs on the LCD

## ⚡ Interrupt Service Routine (ISR)

The **ISR** handles interrupt-based events that require immediate processing.

Interrupts help the system respond to time-critical events without continuously polling every hardware peripheral in the main application.

## 📡 UART Communication

UART is used to provide **serial communication** between the microcontroller and an external serial device.

It can also be useful for:

* Debugging
* Monitoring system information
* Sending event information
* Testing communication

## ⏱️ Timer

The Timer module is used for timing-related operations and interrupt generation.

Timer-based interrupts allow the system to perform periodic operations without blocking the main application.

## 🔐 Password Protection

The system includes a password-protected menu to prevent unauthorized access to stored vehicle logs and configuration options.

The digital keypad is used to enter and verify the password.

## 🎮 User Interface

The digital keypad provides menu navigation such as:

* Password entry
* Menu selection
* Viewing logs
* Moving between log entries
* Returning to the main screen

The **16×2 LCD** displays system status, speed, time, events, and stored logs.

## 🔨 Build System

The project uses a **Makefile** to automate the compilation and build process.

The Makefile helps to:

* Compile source files
* Manage multiple modules
* Generate the required output files
* Reduce manual compilation steps

## 📚 Concepts Learned

Through this project, I gained practical experience in:

* Embedded C programming
* PIC16F877A microcontroller
* ADC programming
* Timer programming
* Interrupt Service Routines
* UART communication
* I2C communication
* RTC interfacing
* External EEPROM interfacing
* Digital keypad interfacing
* LCD interfacing
* Modular driver development
* Makefile-based project builds
* Debugging embedded systems

## 🎯 Challenges

* Implementing reliable keypad-based navigation
* Handling multiple peripherals simultaneously
* Implementing interrupt-based operations
* Storing and retrieving event logs efficiently from EEPROM
* Managing I2C communication with RTC and EEPROM
* Maintaining a modular and reusable Embedded C code structure

## 🚀 Future Improvements

* Add GPS for vehicle location tracking
* Add CAN communication for real vehicle parameters
* Add wireless connectivity for remote monitoring
* Add a larger display for improved log visualization

## 👨‍💻 Author

**Mohamed Ihushan**

Electronics & Communication Engineering
Embedded Systems Enthusiast

---

⭐ **This project demonstrates practical experience in Embedded C, microcontroller programming, peripheral interfacing, interrupt handling, communication protocols, EEPROM data storage, and embedded software development.**

