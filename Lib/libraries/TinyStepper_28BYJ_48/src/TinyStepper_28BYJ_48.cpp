
//      ******************************************************************
//      *                                                                *
//      *                 Unipolar Stepper Motor Library                 *
//      *            for the 28BYJ-48 motor and ULN2003 driver	         *
//      *                                                                *
//      *            Stan Reifel                     11/8/2017           *
//      *               Copyright (c) S. Reifel & Co, 2017               *
//      *                                                                *
//      ******************************************************************

//
// This library is used to control one or more unipolar 28BYJ-48 stepper motors    
// and requires a ULN2003 driver board.  The motors are accelerated and decelerated 
// as they travel to the given position.
//
// A limitation of this driver is that once a motion starts, you can NOT change the 
// target position, speed or rate of acceleration until the motion has completed.  
// The only exception to this is that you can issue a "Stop" at any point in time,
// which will cause the motor to decelerate until stopped.
//
// The 28BYJ-48 stepper motor has 2048 steps/revolution.
//
// This stepper motor driver is based on Aryeh Elderman's paper "Real Time Stepper  
// Motor Linear Ramping Just By Addition and Multiplication".  See: 
//                          www.hwml.com/LeibRamp.pdf
//
// Usage:
//    Near the top of the program, add:
//        include "TinyStepper_28BYJ_48.h"
//
//    For each stepper, delcare a global object outside of all functions as follows:
//        TinyStepper_28BYJ_48 stepper1;
//        TinyStepper_28BYJ_48 stepper2;
//
//    In Setup(), assign stepper pin numbers:
//        stepper1.connectToPins(2, 3, 4, 5);
//        stepper2.connectToPins(6, 7, 8, 9);
//
//    Example moving one motor:
//        //
//        // set the speed in steps/second and acceleration in steps/second/second
//        //
//        stepper1.setSpeedInStepsPerSecond(256);
//        stepper1.setAccelerationInStepsPerSecondPerSecond(512);
//
//        //
//        // move 2048 steps (one revolution) in the backward direction
//        //
//        stepper1.moveRelativeInSteps(-2048);
//
//        //
//        // move to an absolute position of 2048 steps
//        //
//        stepper1.moveToPositionInSteps(2048);
//
//
//    Move two motors in units of steps:
//        //
//        // set the speed in rotations/second and acceleration in 
//        // steps/second/second
//        //
//        stepper1.setSpeedInStepsPerSecond(300);
//        stepper1.setAccelerationInStepsPerSecondPerSecond(1000);
//        stepper2.setSpeedInStepsPerSecond(300);
//        stepper2.setAccelerationInStepsPerSecondPerSecond(1000);
//
//        //
//        // setup motor 1 to move backward 1.5 revolutions, this step does not 
//        // actually move the motor
//        //
//        stepper1.setupRelativeMoveInSteps(2048 * -1.5);
//
//        //
//        // setup motor 2 to move forward 3.0 revolutions, this step does not 
//        // actually move the motor
//        //
//        stepper2.setupRelativeMoveInRevolutions(2048 * 3.0);
//
//        //
//        // execute the moves
//        //
//        while((!stepper1.motionComplete()) || (!stepper2.motionComplete()))
//        {
//          stepper1.processMovement();
//          stepper2.processMovement();
//        }
//

#include "TinyStepper_28BYJ_48.h"

// ---------------------------------------------------------------------------------

//
// constructor for the stepper class
//
TinyStepper_28BYJ_48::TinyStepper_28BYJ_48()
{
  //
  // initialize constants
  //
  in1Pin = 0;
  in2Pin = 0;
  in3Pin = 0;
  in4Pin = 0;
  currentPosition_InSteps = 0;
  desiredSpeed_InStepsPerSecond = 2048.0 / 8.0;
  acceleration_InStepsPerSecondPerSecond = 2048.0 / 4;
  currentStepPeriod_InUS = 0.0;
  stepPhase = 0;
}



//
// connect the stepper object to the IO pins
//  Enter:  in1PinNumber = IO pin number for motor wire 1 (blue)
//          in2PinNumber = IO pin number for motor wire 2 (pink)
//          in3PinNumber = IO pin number for motor wire 3 (yellow)
//          in4PinNumber = IO pin number for motor wire 4 (orange)
//
void TinyStepper_28BYJ_48::connectToPins(byte in1PinNumber, byte in2PinNumber, 
                                         byte in3PinNumber, byte in4PinNumber)
{
  //
  // remember the pin numbers
  //
  in1Pin = in1PinNumber;
  in2Pin = in2PinNumber;
  in3Pin = in3PinNumber;
  in4Pin = in4PinNumber;
  
  //
  // configure the IO bits
  //
  pinMode(in1Pin, OUTPUT);
  digitalWrite(in1Pin, LOW);

  pinMode(in2Pin, OUTPUT);
  digitalWrite(in2Pin, LOW);

  pinMode(in3Pin, OUTPUT);
  digitalWrite(in3Pin, LOW);

  pinMode(in4Pin, OUTPUT);
  digitalWrite(in4Pin, LOW);
}



//
// set the current position of the motor in steps, this does not move the motor
// Note: This function should only be called when the motor is stopped
//    Enter:  currentPositionInSteps = the new position of the motor in steps
//
void TinyStepper_28BYJ_48::setCurrentPositionInSteps(long currentPositionInSteps)
{
  currentPosition_InSteps = currentPositionInSteps;
}



//
// get the current position of the motor in steps, this functions is updated
// while the motor moves
//  Exit:  a signed motor position in steps returned
//
long TinyStepper_28BYJ_48::getCurrentPositionInSteps()
{
  return(currentPosition_InSteps);
}



//
// setup a "Stop" to begin the process of decelerating from the current velocity to  
// zero, decelerating requires calls to processMove() until the move is complete
//
void TinyStepper_28BYJ_48::setupStop()
{
  //
  // move the target position so that the motor will begin deceleration now
  //
  if (direction_Scaler > 0)
    targetPosition_InSteps = currentPosition_InSteps + decelerationDistance_InSteps;
  else
    targetPosition_InSteps = currentPosition_InSteps - decelerationDistance_InSteps;
}



//
// set the maximum speed in steps/second, this is the maximum speed reached  
// while accelerating
// Note: this can only be called when the motor is stopped
//  Enter:  speedInStepsPerSecond = speed to accelerate up to, units in steps/second
//
void TinyStepper_28BYJ_48::setSpeedInStepsPerSecond(float speedInStepsPerSecond)
{
  desiredSpeed_InStepsPerSecond = speedInStepsPerSecond;
}



//
// set the rate of acceleration in steps/second/second
// Note: this can only be called when the motor is stopped
//  Enter:  accelerationInStepsPerSecondPerSecond = rate of acceleration, units in 
//          steps/second/second
//
void TinyStepper_28BYJ_48::setAccelerationInStepsPerSecondPerSecond(
                           float accelerationInStepsPerSecondPerSecond)
{
    acceleration_InStepsPerSecondPerSecond = accelerationInStepsPerSecondPerSecond;
}



//
// move relative to the current position in steps, this function does not return 
// until the move is complete
//  Enter:  distanceToMoveInSteps = signed distance to move relative to the current  
//          position in steps
//
void TinyStepper_28BYJ_48::moveRelativeInSteps(long distanceToMoveInSteps)
{
  setupRelativeMoveInSteps(distanceToMoveInSteps);
  
  while(!processMovement())
    ;
}



//
// setup a move relative to the current position, units are in steps, no motion  
// occurs until processMove() is called
// Note: this can only be called when the motor is stopped
//  Enter:  distanceToMoveInSteps = signed distance to move relative to the current  
//          position in steps
//
void TinyStepper_28BYJ_48::setupRelativeMoveInSteps(long distanceToMoveInSteps)
{
  setupMoveInSteps(currentPosition_InSteps + distanceToMoveInSteps);
}



//
// move to the given absolute position in steps, this function does not return until
// the move is complete
//  Enter:  absolutePositionToMoveToInSteps = signed absolute position to move to in  
//          units of steps
//
void TinyStepper_28BYJ_48::moveToPositionInSteps(long absolutePositionToMoveToInSteps)
{
  setupMoveInSteps(absolutePositionToMoveToInSteps);
  
  while(!processMovement())
    ;
}



//
// setup a move with units are in steps, no motion occurs until processMove() is called
// Note: this can only be called when the motor is stopped
//  Enter:  absolutePositionToMoveToInSteps = signed absolute position to move to in 
//          units of steps
//
void TinyStepper_28BYJ_48::setupMoveInSteps(long absolutePositionToMoveToInSteps)
{
  long distanceToTravel_InSteps;
  
  
  //
  // save the target location
  //
  targetPosition_InSteps = absolutePositionToMoveToInSteps;
  

  //
  // determine the period in US of the first step
  //
  ramp_InitialStepPeriod_InUS = 
    1000000.0 / sqrt(2.0 * acceleration_InStepsPerSecondPerSecond);
    
    
  //
  // determine the period in US between steps when going at the desired velocity
  //
  desiredStepPeriod_InUS = 1000000.0 / desiredSpeed_InStepsPerSecond;


  //
  // determine the number of steps needed to go from the desired velocity down to a 
  // velocity of 0,  Steps = Velocity^2 / (2 * Accelleration)
  //
  decelerationDistance_InSteps = (long) round((desiredSpeed_InStepsPerSecond * 
    desiredSpeed_InStepsPerSecond) / (2.0 * acceleration_InStepsPerSecondPerSecond));
  
  
  //
  // determine the distance and direction to travel
  //
  distanceToTravel_InSteps = targetPosition_InSteps - currentPosition_InSteps;
  if (distanceToTravel_InSteps < 0) 
  {
    distanceToTravel_InSteps = -distanceToTravel_InSteps;
    direction_Scaler = -1;
  }
  else
  {
    direction_Scaler = 1;
  }


  //
  // check if travel distance is too short to accelerate up to the desired velocity
  //
  if (distanceToTravel_InSteps <= (decelerationDistance_InSteps * 2L))
    decelerationDistance_InSteps = (distanceToTravel_InSteps / 2L);


  //
  // start the acceleration ramp at the beginning
  //
  ramp_NextStepPeriod_InUS = ramp_InitialStepPeriod_InUS;
  acceleration_InStepsPerUSPerUS = acceleration_InStepsPerSecondPerSecond / 1E12;
  startNewMove = true;
}



//
// if it is time, move one step
//  Exit:  true returned if movement complete, false returned not a final target 
//           position yet
//
bool TinyStepper_28BYJ_48::processMovement(void)
{ 
  unsigned long currentTime_InUS;
  unsigned long periodSinceLastStep_InUS;
  long distanceToTarget_InSteps;

  //
  // check if already at the target position
  //
  if (currentPosition_InSteps == targetPosition_InSteps)
    return(true);

  //
  // check if this is the first call to start this new move
  //
  if (startNewMove)
  {    
    ramp_LastStepTime_InUS = micros();
    startNewMove = false;
  }
    
  //
  // determine how much time has elapsed since the last step (Note 1: this method   
  // works even if the time has wrapped. Note 2: all variables must be unsigned)
  //
  currentTime_InUS = micros();
  periodSinceLastStep_InUS = currentTime_InUS - ramp_LastStepTime_InUS;

  //
  // if it is not time for the next step, return
  //
  if (periodSinceLastStep_InUS < (unsigned long) ramp_NextStepPeriod_InUS)
    return(false);

  //
  // determine the distance from the current position to the target
  //
  distanceToTarget_InSteps = targetPosition_InSteps - currentPosition_InSteps;
  if (distanceToTarget_InSteps < 0) 
    distanceToTarget_InSteps = -distanceToTarget_InSteps;

  //
  // test if it is time to start decelerating, if so change from accelerating to 
  // decelerating
  //
  if (distanceToTarget_InSteps == decelerationDistance_InSteps)
    acceleration_InStepsPerUSPerUS = -acceleration_InStepsPerUSPerUS;
  
  //
  // execute the step on the rising edge
  //
  setNextFullStep(direction_Scaler);
  
  //
  // update the current position and speed
  //
  currentPosition_InSteps += direction_Scaler;
  currentStepPeriod_InUS = ramp_NextStepPeriod_InUS;


  //
  // compute the period for the next step
  // StepPeriodInUS = 
  //   LastStepPeriodInUS * (1 - AccelerationInStepsPerUSPerUS * LastStepPeriodInUS^2)
  //
  ramp_NextStepPeriod_InUS = ramp_NextStepPeriod_InUS * 
                (1.0 - acceleration_InStepsPerUSPerUS * ramp_NextStepPeriod_InUS * 
                 ramp_NextStepPeriod_InUS);
 
 
  //
  // clip the speed so that it does not accelerate beyond the desired velocity
  //
  if (ramp_NextStepPeriod_InUS < desiredStepPeriod_InUS)
    ramp_NextStepPeriod_InUS = desiredStepPeriod_InUS;


  //
  // update the acceleration ramp
  //
  ramp_LastStepTime_InUS = currentTime_InUS;
 
 
  //
  // check if the move has reached its final target position, return true if all 
  // done
  //
  if (currentPosition_InSteps == targetPosition_InSteps)
  {
    currentStepPeriod_InUS = 0.0;
    return(true);
  }
    
  return(false);
}



//
// update the IO pins for the next full step
//  Enter:  direction = 1 to step forward, -1 to step backward 
//
void TinyStepper_28BYJ_48::setNextFullStep(int direction)
{
  //
  // compute the next phase number
  //
  stepPhase += (-direction);
  
  if (stepPhase <= -1)
    stepPhase = 3;
    
  if (stepPhase >= 4)
    stepPhase = 0;

  //
  // set the coils for this phase
  //
  switch(stepPhase)
  {
    case 0:
      digitalWrite(in1Pin, LOW); 
      digitalWrite(in2Pin, LOW);
      digitalWrite(in3Pin, HIGH);
      digitalWrite(in4Pin, HIGH);
      break; 
    case 1:
      digitalWrite(in1Pin, LOW); 
      digitalWrite(in2Pin, HIGH);
      digitalWrite(in3Pin, HIGH);
      digitalWrite(in4Pin, LOW);
      break; 
    case 2:
      digitalWrite(in1Pin, HIGH); 
      digitalWrite(in2Pin, HIGH);
      digitalWrite(in3Pin, LOW);
      digitalWrite(in4Pin, LOW);
      break; 
    case 3:
      digitalWrite(in1Pin, HIGH); 
      digitalWrite(in2Pin, LOW);
      digitalWrite(in3Pin, LOW);
      digitalWrite(in4Pin, HIGH);
      break; 
   }
}



//
// disable the motor, all the drive coils are turned off to save power
// and reduce heat when motor is not in motion, any movement command will
// automatically renable the stepper
//
void TinyStepper_28BYJ_48::disableMotor()
{
  digitalWrite(in1Pin, LOW); 
  digitalWrite(in2Pin, LOW);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
}



//
// Get the current velocity of the motor in steps/second.  This functions is updated
// while it accelerates up and down in speed.  This is not the desired speed, but  
// the speed the motor should be moving at the time the function is called.  This  
// is a signed value and is negative when the motor is moving backwards.
// Note: This speed will be incorrect if the desired velocity is set faster than
// this library can generate steps, or if the load on the motor is too great for
// the amount of torque that it can generate.
//  Exit:  velocity speed in steps per second returned, signed
//
float TinyStepper_28BYJ_48::getCurrentVelocityInStepsPerSecond()
{
  if (currentStepPeriod_InUS == 0.0)
    return(0);
  else
  {
    if (direction_Scaler > 0)
      return(1000000.0 / currentStepPeriod_InUS);
    else
      return(-1000000.0 / currentStepPeriod_InUS);
  }
}



//
// check if the motor has competed its move to the target position
//  Exit:  true returned if the stepper is at the target position
//
bool TinyStepper_28BYJ_48::motionComplete()
{
  if (currentPosition_InSteps == targetPosition_InSteps)
    return(true);
  else
    return(false);
}

// -------------------------------------- End --------------------------------------

