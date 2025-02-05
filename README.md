# 📱 GSM-Based Home Automation Using Arduino  

## 🚀 Project Overview  
This project presents a **GSM-based Home Automation System** that allows users to remotely control home appliances using SMS commands. The system leverages the **Arduino Mega microcontroller**, **SIM900A GSM module**, and other components to enhance convenience and energy efficiency for home users.  

## ✨ Key Features  
- **Remote Control via SMS:** Users can turn appliances ON/OFF through SMS commands.  
- **Scheduling Timer:** Automate switching of appliances using RTC (Real-Time Clock) module.  
- **Mobile App Integration:** Developed with MIT App Inventor for a simple control interface.  
- **SMS Notifications:** Feedback messages indicate the status of appliances.  
- **Energy Efficiency:** Helps reduce energy wastage by remotely managing appliances.  

## 🛠️ System Flowchart  
Below is a graphical representation of the execution flow for the system, illustrating key processes such as message reception, appliance control, and user notifications:  

![System Flowchart](./images/system-flowchart.png)  

> _Ensure the file `system-flowchart.png` is added to the `/images/` folder in your repository._  

## 🔧 System Components  
- **Arduino Mega 2560**: The main microcontroller board.  
- **SIM900A GSM Module**: Handles SMS communication.  
- **RTC Module (DS3231)**: Provides scheduling capabilities.  
- **Relay Module (HL-54S)**: Manages appliance switching.  
- **16x04 LCD Module**: Displays appliance status.  

## 📋 Installation and Usage  
1. Clone this repository:  
   ```bash
   git clone <repository-url>

2. Open the source code in Arduino IDE and upload it to the Arduino Mega board.
3. Connect the hardware components according to the schematic provided.
4. Test the system by sending SMS commands:
    - **"ON1"**: Turns on Light
    - **"OFF1"**: Turns off Light
    - **"ON2"**: Turns on Fan
    - **"OFF2"**: Turns off Fan
    - **"ON3"**: Turns on TV
    - **"OFF3"**: Turns off TV
5. Monitor appliance status on the LCD or mobile notifications.
  > Any appliance will do as long as it supports the relay load

## 🚀 Future Improvements
- Enhance system security to prevent unauthorized access.
- Add features like brightness control for lights and speed control for fans.
- Develop a more comprehensive utility mobile application.

## ✍️ Authors
Daniel Patrick Punzalan
Jesus Luigi Sison

