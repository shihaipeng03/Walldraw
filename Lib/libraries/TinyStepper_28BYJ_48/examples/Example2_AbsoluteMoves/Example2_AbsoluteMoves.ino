
//      ******************************************************************
//      *                                                                *
//      *  Using "absolute" moves to position a 28BYJ-48 stepper motor   *
//      *                                                                *
//      *            S. Reifel & Co.                6/24/2018            *
//      *                                                                *
//      ******************************************************************


// This example is similar to Example 1 except that is uses "absolute" 
// moves instead of "relative" ones.  Relative moves will use a coordinate 
// system that is relative to the motor's current position.  Absolute moves 
// use a coordinate system that is referenced to the original position of 
// the motor when it is turned on.
//
// For example moving relative 2000 steps, then another 2000, then another  
// 2000 will turn 6000 steps in total (and end up at an absolute position of 
// 6000).
//
// However issuing an absolute move to position 2048 will rotate forward one   
// rotation.  Then running the next absolute move to position 4096 will turn  
// just one more revolution in the same direction.  Finally moving to position 0  
// will rotate backward two revolutions, back to the starting position.
//  
//
// Documentation for this library can be found at:
//    https://github.com/Stan-Reifel/TinyStepper_28BYJ_48
//
// This library requires that the stepper motor be connected to the Arduino 
// using ULN2003 driver board.  

// ***********************************************************************


#include <TinyStepper_28BYJ_48.h>


//
// pin assignments, any digital pins can be used
//
const int LED_PIN = 13;
const int MOTOR_IN1_PIN = 11;
const int MOTOR_IN2_PIN = 10;
const int MOTOR_IN3_PIN = 6;
const int MOTOR_IN4_PIN = 5;


const int STEPS_PER_REVOLUTION = 2048;


//
// create the stepper motor object
//
TinyStepper_28BYJ_48 stepper;



void setup() 
{
  //
  // setup the LED pin and enable print statements
  //
  pinMode(LED_PIN, OUTPUT);   
  Serial.begin(9600);


  //
  // connect and configure the stepper motor to its IO pins
  //
  stepper.connectToPins(MOTOR_IN1_PIN, MOTOR_IN2_PIN, MOTOR_IN3_PIN, MOTOR_IN4_PIN);
}



void loop() 
{
  //
  // Note: This example uses "absolute" motions, meaning the values
  // sent to the move commands use a coordinate system where 0 is the
  // initial position of the motor when it is first turned on.
  //

  //
  // set the speed and acceleration rates for the stepper motor
  //
  stepper.setSpeedInStepsPerSecond(150);
  stepper.setAccelerationInStepsPerSecondPerSecond(500);

  //
  // Rotate the motor to position 2048 (turning one revolution). This 
  // function call will not return until the motion is complete.
  //
  stepper.moveToPositionInSteps(2048);

  //
  // now that the rotation has finished, delay 1 second before starting 
  // the next move
  //
  delay(1000);

  //
  // rotate to position 4096 (turning just one revolution more since it is 
  // already at position 2048)
  //
  stepper.moveToPositionInSteps(4096);
  delay(1000);

  //
  // Now speedup the motor and return to it's home position of 0, resulting 
  // in rotating backward two turns.  Note if you tell a stepper motor to go 
  // faster than it can, it just stops.
  //
  stepper.setSpeedInStepsPerSecond(500);
  stepper.moveToPositionInSteps(0);
  delay(2000);
}



