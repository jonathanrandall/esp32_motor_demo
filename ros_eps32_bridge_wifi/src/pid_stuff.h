/* Functions and type-defs for PID control.

   Taken mostly from Mike Ferguson's ArbotiX code which lives at:

   http://vanadium-ros-pkg.googlecode.com/svn/trunk/arbotix/
*/
#ifndef pid_stuff_
#define pid_stuff_
#include "motor_driver_stuff.h"
#include "encoder_stuff.h"
#include "commands.h"


#define BAUDRATE 9600

#define PID_RATE 10 // Hz 30

#define MAX_PWM 255
/* Convert the rate into an interval */
const int PID_INTERVAL = 1000 / PID_RATE;

/* Track the next time we make a PID calculation */
unsigned long nextPID = PID_INTERVAL;

/* PID setpoint info For a Motor */
typedef struct
{
    String nm;
    double TargetTicksPerFrame; // target speed in ticks per frame
    long Encoder;               // encoder count
    long PrevEnc;               // last encoder count

    /*
     * Using previous input (PrevInput) instead of PrevError to avoid derivative kick,
     * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-derivative-kick/
     */
    int PrevInput; // last input
    // int PrevErr;                   // last error

    /*
     * Using integrated term (ITerm) instead of integrated error (Ierror),
     * to allow tuning changes,
     * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
     */
    // int Ierror;
    int ITerm; // integrated term

    long output; // last motor setting
} SetPointInfo;

SetPointInfo leftPID, rightPID;

/* PID Parameters */
int Kp = 15;//20;
int Kd = 6; //12;
int Ki = 0;
int Ko = 50;

unsigned char moving = 0; // is the base in motion?

/*
 * Initialize PID variables to zero to prevent startup spikes
 * when turning PID on to start moving
 * In particular, assign both Encoder and PrevEnc the current encoder value
 * See http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-initialization/
 * Note that the assumption here is that PID is only turned on
 * when going from stop to moving, that's why we can init everything on zero.
 */
void resetPID()
{
    // Serial.println("resetting PID");
    leftPID.TargetTicksPerFrame = 0.0;
    leftPID.Encoder = -1*readEncoder(LEFT);
    leftPID.PrevEnc = leftPID.Encoder;
    leftPID.output = 0;
    leftPID.PrevInput = 0;
    leftPID.ITerm = 0;

    rightPID.TargetTicksPerFrame = 0.0;
    rightPID.Encoder = -1*readEncoder(RIGHT);
    rightPID.PrevEnc = rightPID.Encoder;
    rightPID.output = 0;
    rightPID.PrevInput = 0;
    rightPID.ITerm = 0;
}

/* PID routine to compute the next motor commands */
void doPID(SetPointInfo *p)
{
    long Perror;
    long output;
    int input;

    // Serial.println((p->nm).c_str());

    
    // Perror = p->TargetTicksPerFrame - (p->Encoder - p->PrevEnc);
    // Serial.print("target: ");
    // Serial.println(p->TargetTicksPerFrame );
    input = p->Encoder - p->PrevEnc;
    Perror = p->TargetTicksPerFrame - input;
    // Serial.print("Perror: ");
    // Serial.println(Perror);

    /*
     * Avoid derivative kick and allow tuning changes,
     * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-derivative-kick/
     * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
     */
    // output = (Kp * Perror + Kd * (Perror - p->PrevErr) + Ki * p->Ierror) / Ko;
    //  p->PrevErr = Perror;
    output = (Kp * Perror - Kd * (input - p->PrevInput) + p->ITerm) / Ko;
    p->PrevEnc = p->Encoder;
    // Serial.print("output before: ");
    // Serial.println(output);
    output += p->output;
    // Serial.print("output after: ");
    // Serial.println(output);
    // Accumulate Integral error *or* Limit output.
    // Stop accumulating when output saturates
    if (output >= MAX_PWM)
        output = MAX_PWM;
    else if (output <= -MAX_PWM)
        output = -MAX_PWM;
    else
        /*
         * allow turning changes, see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
         */
        p->ITerm += Ki * Perror;

    p->output = output;
    // Serial.print((p->nm).c_str());
    // Serial.print(": ");
    // Serial.println(p->output);

    p->PrevInput = input;
}

/* Read the encoder values and call the PID routine */
void updatePID()
{
    /* Read the encoders */
<<<<<<< HEAD
    
=======
    leftPID.Encoder = -1*readEncoder(LEFT);
    rightPID.Encoder = -1*readEncoder(RIGHT);
    // Serial.print("left: ");
    // Serial.println(leftPID.Encoder);
    // Serial.print("right: ");
    // Serial.println(rightPID.Encoder);

>>>>>>> be610b04e16e62b780a15877dc335f6db23eb2eb

    /* If we're not moving there is nothing more to do */
    if (!moving)
    {
        // Serial.println("not moving");
        /*
         * Reset PIDs once, to prevent startup spikes,
         * see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-initialization/
         * PrevInput is considered a good proxy to detect
         * whether reset has already happened
         */
        if (leftPID.PrevInput != 0 || rightPID.PrevInput != 0)
            resetPID();
        return;
    }

    
    rightPID.Encoder = readEncoder(RIGHT);

    /* Compute PID update for each motor */
<<<<<<< HEAD
    doPID(&rightPID);

    leftPID.Encoder = readEncoder(LEFT);
=======
    
>>>>>>> be610b04e16e62b780a15877dc335f6db23eb2eb
    doPID(&leftPID);
    // rightPID.Encoder = readEncoder(RIGHT);
    doPID(&rightPID);
    

    /* Set the motor speeds accordingly */
    setMotorSpeeds(leftPID.output, rightPID.output);
}

#endif
