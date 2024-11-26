# USB-C Power Profiler

An AVR Assembly-based project to monitor and analyze power usage in real-time through USB-C. 
This tool is ideal for embedded developers who need detailed insights into voltage, current, and power consumption for battery powered devices.

## Features
- Monitors voltage and current at 125 Hz using the INA219 sensor.
- Measures up to **32V @ 32A**.
- Identifies the power flow direction across the USB-C connection.

## Hardware
- ATmega328p
- INA219 sensor
- USB-C breakout boards
- 16x2 LCD
- [SerialPlot](https://github.com/hyOzd/serialplot) 
  - Data Format: Custom Frame
  - Frame Start: 0D 0A
  - No. Channel: 2
  - Frame Size: 4
  - Number Type: int16
  - Little Endian
  

<img src="https://github.com/shreyask21/usbcpowprof/blob/main/hw_photo.jpg?raw=true" alt="Hardware Setup" width="500" />
