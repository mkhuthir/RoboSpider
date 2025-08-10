#ifndef MAIN_H
#define MAIN_H

  // Main.h - Main header file for SpiderBot project

  // Define DEBUG to enable debug output
  //#define DEBUG 
  #undef DEBUG 

  // ------------------------
  // SERIAL PORTS
  // ------------------------

  #define DEBUG_SERIAL Serial     // Serial port for debugging output
  #define RC100_SERIAL Serial1    // Serial port for RC100 remote controller
  #define DXL_SERIAL ""           // OpenCR Dynamixel is on Serial1(USART1)

  // ------------------------
  // SERIAL BAUD RATES
  // ------------------------

  #define DXL_BAUD_RATE         1000000   // Default baud rate for Dynamixel servos
  #define DEBUG_BAUD_RATE       115200    // Baud rate for debugging output
  #define RC100_BAUD_RATE       115200    // Baud rate for RC controller communication

#endif  // MAIN_H