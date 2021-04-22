
//      ******************************************************************
//      *                                                                *
//      *   Simple example for controlling the 28BYJ-48 stepper motor    *
//      *                                                                *
//      *            S. Reifel & Co.                6/24/2018            *
//      *                                                                *
//      ******************************************************************


// This is the simplest example of how to run a 28BYJ-48 stepper motor.  
//
// Documentation for this library can be found at:
//    https://github.com/Stan-Reifel/TinyStepper_28BYJ_48
//
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
  // Note: This example uses "relative" motions.  This means that each
  // command will move the number of steps given, starting from it's 
  // current position.
  //

  //
  // set the speed and acceleration rates for the stepper motor
  //
  stepper.setSpeedInStepsPerSecond(256);
  stepper.setAccelerationInStepsPerSecondPerSecond(512);

  //
  // Rotate the motor in the forward direction one revolution (2048 steps). 
  // This function call will not return until the motion is complete.
  //
  stepper.moveRelativeInSteps(2048);
  
  //
  // now that the rotation has finished, delay 1 second before starting 
  // the next move
  //
  delay(1000);

  //
  // rotate backward 1 rotation, then wait 1 second
  //
  stepper.moveRelativeInSteps(-2048);
  delay(1000);

  //
  // This time speedup the motor, turning 5 revolutions.  Note if you
  // tell a stepper motor to go faster than it can, it just stops.
  //
  stepper.setSpeedInStepsPerSecond(500);
  stepper.setAccelerationInStepsPerSecondPerSecond(1000);
  stepper.moveRelativeInSteps(2048 * 5);
  delay(2000);
}



