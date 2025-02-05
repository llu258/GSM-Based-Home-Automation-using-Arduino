# 📱 GSM-Based Home Automation Using Arduino  

## 🚀 Project Overview  
This project presents a **GSM-based Home Automation System** that allows users to remotely control home appliances using SMS commands. The system leverages the **Arduino Mega microcontroller**, **SIM900A GSM module**, and other components to enhance convenience and energy efficiency for home users.  

## ✨ Key Features  
- **Remote Control via SMS:** Users can turn appliances ON/OFF through SMS commands.  
- **Scheduling Timer:** Automate switching of appliances using RTC (Real-Time Clock) module.  
- **Mobile App Integration:** Developed with MIT App Inventor for a simple control interface.  
- **SMS Notifications:** Feedback messages indicate the status of appliances.  
- **Energy Efficiency:** Helps reduce energy wastage by remotely managing appliances.

## ⚙️ Functions  
- **Turn Appliance ON/OFF:**  
  - Send `"ON#<appliance_number>"` to switch an appliance ON.  
  - Send `"OFF#<appliance_number>"` to switch an appliance OFF.  
- **Scheduling Timer:**  
  - Set automatic ON/OFF schedules using the built-in RTC module and push button switches.  
- **Status Notifications:**  
  - Receive SMS updates with the current status of controlled appliances.  
- **Manual Control:**  
  - Operate appliances directly using a physical switchbox.  

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

## 🚀 Future Improvements
- Enhance system security to prevent unauthorized access.
- Add features like brightness control for lights and speed control for fans.
- Develop a more comprehensive utility mobile application.

## ✍️ Authors
Daniel Patrick Punzalan
Jesus Luigi Sison

