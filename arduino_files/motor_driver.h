/***************************************************************
   Motor driver function definitions - by James Nugen
   *************************************************************/

#ifdef L298_MOTOR_DRIVER
  #define RIGHT_MOTOR_BACKWARD 5
  #define LEFT_MOTOR_BACKWARD  6
  #define RIGHT_MOTOR_FORWARD  9
  #define LEFT_MOTOR_FORWARD   10
  #define RIGHT_MOTOR_ENABLE 12
  #define LEFT_MOTOR_ENABLE 13

#elif defined CYTRON_MOTOR_DRIVER
  //the ctyron motor driver has pins for speed and direction.
  #define LEFT_MOTOR_PWM 5
  #define RIGHT_MOTOR_PWM 6
  #define RIGHT_MOTOR_DIR 7
  #define LEFT_MOTOR_DIR 4
  

#endif

void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);
