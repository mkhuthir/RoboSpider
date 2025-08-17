#ifndef MAIN_H
#define MAIN_H

  // Main.h - Main header file for SpiderBot project

  #define DEBUG_SERIAL          Serial      // Serial port for debugging output
  #define DEBUG_BAUD_RATE       115200      // Baud rate for debugging output
  
  #define DXL_SERIAL            ""          // OpenCR Dynamixel is on Serial1(USART1)
  #define DXL_BAUD_RATE         1000000     // Default baud rate for Dynamixel servos
  #define DXL_PROTOCOL_VERSION  1.0f        // Protocol version for Dynamixel servos
  
  #define RC100_SERIAL          Serial1     // Serial port for RC100 remote controller
  #define RC100_BAUD_RATE       115200      // Baud rate for RC controller communication

#endif  // MAIN_H