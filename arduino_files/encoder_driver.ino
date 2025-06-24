/* *************************************************************
   Encoder definitions
   
   Add an "#ifdef" block to this file to include support for
   a particular encoder board or library. Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.
   
   ************************************************************ */
   
#ifdef USE_BASE

#ifdef ROBOGAIA
  /* The Robogaia Mega Encoder shield */
  #include "MegaEncoderCounter.h"

  /* Create the encoder shield object */
  MegaEncoderCounter encoders = MegaEncoderCounter(4); // Initializes the Mega Encoder Counter in the 4X Count mode
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return encoders.YAxisGetCount();
    else return encoders.XAxisGetCount();
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT) return encoders.YAxisReset();
    else return encoders.XAxisReset();
  }
#elif defined(ARDUINO_ENC_COUNTER)
  volatile long left_enc_pos = 0L;
  volatile long right_enc_pos = 0L;
  static const int8_t ENC_STATES [] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};  //encoder lookup table

  /*
  ENC_STATES indexed by what? enc_last, lest significant four bits.
  last bit is pin 2 state, second last bit is pin 3 state
  We have basically
  p3 p2 c3 c2 (p3p2c3c2), these are our bits, which represent: p3=previous pin 3, c3 = current value pin 3.
  0000 --> 0, no change 3rd last bit to last bit, both are zero
  0001 --> 1, change from 0 to 1 on pin 2
  0010 --> -1, change from 0 to 1 on pin 3
  0011 --> 0, change from 0 to 1 on both pins (not sure if this sate is possible for quadrature encoders)
  0100 --> -1, change from 1 to 0 on pin 2. 
  0101 --> 0, no change on either (pin 2 at 1, pin 3 at 0)
  0110 --> 0, change on both pins, not sure if this is possible.
  0111 --> 1, pin 3 change from 0 to 1, pin 2 high
  1000 --> 1, pin 3 change from 1 to 0, pin 2 low
  1001
  1010
  1011 --> -1, pin 2 change from 0 to 1, with pin 3 high.
  */
    
  /* Interrupt routine for LEFT encoder, taking care of actual counting */
  ISR (PCINT2_vect){
  	static uint8_t enc_last=0;
        
	enc_last <<=2; //shift previous state two places
	enc_last |= (PIND & (3 << 2)) >> 2; //read the current state into lowest 2 bits
  // Serial.println("interrupt");
  
  	left_enc_pos += ENC_STATES[(enc_last & 0x0f)];
  }
  
  /* Interrupt routine for RIGHT encoder, taking care of actual counting */
  ISR (PCINT1_vect){
        static uint8_t enc_last=0;
          	
	enc_last <<=2; //shift previous state two places
	enc_last |= (PINC & (3 << 4)) >> 4; //read the current state into lowest 2 bits
  
  	right_enc_pos += ENC_STATES[(enc_last & 0x0f)];
    
  }
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return left_enc_pos;
    else return right_enc_pos;
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT){
      left_enc_pos=0L;
      return;
    } else { 
      right_enc_pos=0L;
      return;
    }
  }
#else
  #error A encoder driver must be selected!
#endif

/* Wrap the encoder reset function */
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

#endif

