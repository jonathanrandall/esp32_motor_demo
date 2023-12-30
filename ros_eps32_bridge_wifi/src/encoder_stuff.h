/* *************************************************************
   Encoder driver function definitions - by James Nugen
   ************************************************************ */
   
   
#ifndef ENCODERS_STUFF_
#define ENCODERS_STUFF_

// wheel encoder interrupts
#include <Arduino.h>
#include "commands.h"
#define encoder0PinA 33      // encoder b Left
#define encoder0PinB 4       // encoder a left

#define encoder1PinA 16     // encoder b right
#define encoder1PinB 17     // endoder a right



const float ppr = 753.2; //pulses per revolution
const float wheel_diameter = 10;//cm
const float circum = 31.73; //cm
const float pulse_dist = circum/ppr;
const float robot_width = 34.0; //cm
const float pi = 3.14159265359;
const float turning_circle = 34.0*pi;

const float one_m = 100/pulse_dist; //number of pulses to travel 1m

void doEncoderA();

void doEncoderB();

// ************** encoder 2 *********************

void doEncoderC();

void doEncoderD();

void init_encoders();

// filter funcition
// float filter(float prevValue, float currentValue, int filter) {  
//   float lengthFiltered =  (prevValue + (currentValue * filter)) / (filter + 1);  
//   return lengthFiltered;  
// }
long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();


#endif   


