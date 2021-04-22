
//      ******************************************************************
//      *                                                                *
//      *     Example of running two or more motors at the same time     *
//      *                                                                *
//      *            S. Reifel & Co.                6/24/2018            *
//      *                                                                *
//      ******************************************************************


// This example shows how to run two motors at the same time.  The previous 
// examples used function calls that were "blocking", meaning that they did 
// not return until the motion was complete.  This example show how to use
// a "polling" method instead, thus allowing you to do two or more things 
// at once.
//
//
// Documentation at:
//    https://github.com/Stan-Reifel/TinyStepper_28BYJ_48

// ***********************************************************************


#include <TinyStepper_28BYJ_48.h>


//
// pin assignments, any digital pins can be used
//
const int LED_PIN = 13;

const int MOTORX_IN1_PIN = 11;
const int MOTORX_IN2_PIN = 10;
const int MOTORX_IN3_PIN = 6;
const int MOTORX_IN4_PIN = 5;

const int MOTORY_IN1_PIN = 7;
const int MOTORY_IN2_PIN = 4;
const int MOTORY_IN3_PIN = 3;
const int MOTORY_IN4_PIN = 2;


const int STEPS_PER_REVOLUTION = 2048;


//
// create two stepper motor objects, one for each motor
//
TinyStepper_28BYJ_48 stepperX;
TinyStepper_28BYJ_48 stepperY;



void setup() 
{
  //
  // setup the LED pin and enable print statements
  //
  pinMode(LED_PIN, OUTPUT);   
  Serial.begin(9600);


  //
  // connect and configure the stepper motors to their IO pins
  //
  stepperX.connectToPins(MOTORX_IN1_PIN, MOTORX_IN2_PIN, MOTORX_IN3_PIN, MOTORX_IN4_PIN);
  stepperY.connectToPins(MOTORY_IN1_PIN, MOTORY_IN2_PIN, MOTORY_IN3_PIN, MOTORY_IN4_PIN);
}


void loop() 
{
  //
  // setup the speed, acceleration and number of steps to move for the 
  // X motor, note: these commands do not start moving yet
  //
  stepperX.setSpeedInStepsPerSecond(300);
  stepperX.setAccelerationInStepsPerSecondPerSecond(500);
  stepperX.setupRelativeMoveInSteps(2048);


  //
  // setup the speed, acceleration and number of steps to move for the 
  // Y motor
  //
  stepperY.setSpeedInStepsPerSecond(300);
  stepperY.setAccelerationInStepsPerSecondPerSecond(500);
  stepperY.setupRelativeMoveInSteps(-2048);


  //
  // now execute the moves, looping until both motors have finished
  //
  while((!stepperX.motionComplete()) || (!stepperY.motionComplete()))
  {
    stepperX.processMovement();
    stepperY.processMovement();
  }


  //
  // now that the rotations have finished, delay 1 second before starting 
  // the next move
  //
  delay(1000);


  //
  // use the function below to move two motors with speed coordination
  // so that both stop at the same time, even if one moves farther than
  // the other
  //
  long stepsX = -2048 * 1;
  long stepsY = 2048 * 5;
  float speedInStepsPerSecond = 400;
  float accelerationInStepsPerSecondPerSecond = 1000;
  moveXYWithCoordination(stepsX, stepsY, speedInStepsPerSecond, accelerationInStepsPerSecondPerSecond);
  delay(3000);
}



//
// move both X & Y motors together in a coordinated way, such that they each 
// start and stop at the same time, even if one motor moves a greater distance
//
void moveXYWithCoordination(long stepsX, long stepsY, float speedInStepsPerSecond, float accelerationInStepsPerSecondPerSecond)
{
  float speedInStepsPerSecond_X;
  float accelerationInStepsPerSecondPerSecond_X;
  float speedInStepsPerSecond_Y;
  float accelerationInStepsPerSecondPerSecond_Y;
  long absStepsX;
  long absStepsY;

  //
  // setup initial speed and acceleration values
  //
  speedInStepsPerSecond_X = speedInStepsPerSecond;
  accelerationInStepsPerSecondPerSecond_X = accelerationInStepsPerSecondPerSecond;
  
  speedInStepsPerSecond_Y = speedInStepsPerSecond;
  accelerationInStepsPerSecondPerSecond_Y = accelerationInStepsPerSecondPerSecond;


  //
  // determine how many steps each motor is moving
  //
  if (stepsX >= 0)
    absStepsX = stepsX;
  else
    absStepsX = -stepsX;
 
  if (stepsY >= 0)
    absStepsY = stepsY;
  else
    absStepsY = -stepsY;


  //
  // determine which motor is traveling the farthest, then slow down the
  // speed rates for the motor moving the shortest distance
  //
  if ((absStepsX > absStepsY) && (stepsX != 0))
  {
    //
    // slow down the motor traveling less far
    //
    float scaler = (float) absStepsY / (float) absStepsX;
    speedInStepsPerSecond_Y = speedInStepsPerSecond_Y * scaler;
    accelerationInStepsPerSecondPerSecond_Y = accelerationInStepsPerSecondPerSecond_Y * scaler;
  }
  
  if ((absStepsY > absStepsX) && (stepsY != 0))
  {
    //
    // slow down the motor traveling less far
    //
    float scaler = (float) absStepsX / (float) absStepsY;
    speedInStepsPerSecond_X = speedInStepsPerSecond_X * scaler;
    accelerationInStepsPerSecondPerSecond_X = accelerationInStepsPerSecondPerSecond_X * scaler;
  }

  
  //
  // setup the motion for the X motor
  //
  stepperX.setSpeedInStepsPerSecond(speedInStepsPerSecond_X);
  stepperX.setAccelerationInStepsPerSecondPerSecond(accelerationInStepsPerSecondPerSecond_X);
  stepperX.setupRelativeMoveInSteps(stepsX);


  //
  // setup the motion for the Y motor
  //
  stepperY.setSpeedInStepsPerSecond(speedInStepsPerSecond_Y);
  stepperY.setAccelerationInStepsPerSecondPerSecond(accelerationInStepsPerSecondPerSecond_Y);
  stepperY.setupRelativeMoveInSteps(stepsY);


  //
  // now execute the moves, looping until both motors have finished
  //
  while((!stepperX.motionComplete()) || (!stepperY.motionComplete()))
  {
    stepperX.processMovement();
    stepperY.processMovement();
  }
}


