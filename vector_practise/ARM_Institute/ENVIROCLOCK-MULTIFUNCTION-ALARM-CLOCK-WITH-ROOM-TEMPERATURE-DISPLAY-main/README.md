🕒 ARM Embedded Project

A collection of ARM7 (LPC2138/LPC2148) based embedded system programs, including RTC, LCD, ADC, Keypad, Alarm, and Temperature Sensor interfacing.
Built using Keil µVision and tested on Proteus simulation and real hardware.

⚙️ Features

Real-Time Clock (RTC) integration with alarm

LCD display interface (16x2)

Keypad input handling

ADC-based temperature sensing using LM35

Custom symbol generation on LCD (alarm icon, etc.)

Modular C code with function-based structure

Works on LPC2138 / LPC2148 microcontrollers

🧩 **Project Structure**

📂 **ARM_Project**

┣ 📜 **mini_project.c** – Main program

┣ 📜 **lcd.c, lcd.h** – LCD driver

┣ 📜 **rtc.c, rtc.h** – RTC configuration

┣ 📜 **keypad.c, keypad.h** – Keypad scanning logic

┣ 📜 **adc.c, adc.h** – ADC interfacing (LM35)

┣ 📜 **alarm.c, alarm.h** – Alarm setup and control

┣ 📜 **pin_connect_block.c, pin_connect_block.h** – Pin configuration

┣ 📜 **defines.c, defines.h** – Macros

┗ 📜 **readme.md**


🔌 **Hardware Connections**

**Component** – **LPC2138/2148 Pin** – **Description**

**LCD Data** (D0–D7) – P0.8–P0.15 – 8-bit interface

**LCD RS** – P0.16 – Register Select

**LCD RW** – P0.18 – Read/Write

**LCD EN** – P0.17 – Enable

**KPM** - P1.16-P1.23

**LM35** – P0.27 (AD0.0) – Temperature input

**Alarm Buzzer** – P0.5 – Output

**Alarm Stop Switch** – P0.6 – Input

**RTC Battery** – VBAT Pin – Backup power


🧠 How It Works

RTC runs continuously, powered by backup battery.

Main loop displays time on LCD and checks for alarm match.

When alarm time == RTC time → buzzer rings, LCD shows alarm symbol.

Alarm Stop switch stops the alarm and resets flag.

🧰 Tools Used

Keil µVision5 – Code development

Proteus 8 – Simulation

Flash Magic – Flashing program to LPC2138/2148

ARM7TDMI-S – Microcontroller core

🚀 How to Run

Clone this repo:

git clone https://github.com/ajaykumar1806/ENVIROCLOCK-MULTIFUNCTION-ALARM-CLOCK-WITH-ROOM-TEMPERATURE-DISPLAY.git

Open project in Keil µVision.

Build and load .hex file to your LPC2138/2148 board using Flash Magic.

Power on → LCD shows time → set alarm → enjoy!

📸 Demo

Example:
<img width="1039" height="739" alt="image" src="https://github.com/user-attachments/assets/857a29c1-bd35-47a6-8818-ca4425aa498a" />

RTC and LCD display

Alarm symbol on trigger

Temperature readout
