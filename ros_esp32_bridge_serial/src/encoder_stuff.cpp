
#include "encoder_stuff.h"

volatile long encoder0Pos = 0;    // encoder 1
volatile long encoder1Pos = 0;    // encoder 2

void doEncoderA(){  

  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW) {  
      encoder0Pos = encoder0Pos + 1;         // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinB) == HIGH) {   
      encoder0Pos = encoder0Pos + 1;          // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }
 
}

void doEncoderB(){  

  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {   
   // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH) {  
      encoder0Pos = encoder0Pos + 1;         // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }
  // Look for a high-to-low on channel B
  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinA) == LOW) {   
      encoder0Pos = encoder0Pos + 1;          // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }
  

}

// ************** encoder 2 *********************

void doEncoderC(){  

  // look for a low-to-high on channel A
  if (digitalRead(encoder1PinA) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder1PinB) == LOW) {  
      encoder1Pos = encoder1Pos - 1;         // CW
    } 
    else {
      encoder1Pos = encoder1Pos + 1;         // CCW
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder1PinB) == HIGH) {   
      encoder1Pos = encoder1Pos - 1;          // CW
    } 
    else {
      encoder1Pos = encoder1Pos + 1;          // CCW
    }
  }
 
}

void doEncoderD(){  

  // look for a low-to-high on channel B
  if (digitalRead(encoder1PinB) == HIGH) {   
   // check channel A to see which way encoder is turning
    if (digitalRead(encoder1PinA) == HIGH) {  
      encoder1Pos = encoder1Pos - 1;         // CW
    } 
    else {
      encoder1Pos = encoder1Pos + 1;         // CCW
    }
  }
  // Look for a high-to-low on channel B
  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder1PinA) == LOW) {   
      encoder1Pos = encoder1Pos - 1;          // CW
    } 
    else {
      encoder1Pos = encoder1Pos + 1;          // CCW
    }
  }
  

}

void init_encoders(){
  pinMode(encoder0PinA, INPUT_PULLUP);    // encoder pins
  pinMode(encoder0PinB, INPUT_PULLUP);

  pinMode(encoder1PinA, INPUT_PULLUP); 
  pinMode(encoder1PinB, INPUT_PULLUP);

  attachInterrupt(encoder0PinA, doEncoderA, CHANGE);
  attachInterrupt(encoder0PinB, doEncoderB, CHANGE); 

  attachInterrupt(encoder1PinA, doEncoderC, CHANGE);
  attachInterrupt(encoder1PinB, doEncoderD, CHANGE); 
}

long readEncoder(int i) {
    if (i == LEFT) return encoder0Pos;
    else return encoder1Pos;
  }

/* Wrap the encoder reset function */
void resetEncoder(int i) {
  if (i == LEFT){
    encoder0Pos=0L;
    return;
  } else { 
    encoder1Pos=0L;
    return;
  }
}

/* Wrap the encoder reset function */
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}