
//      ******************************************************************
//      *                                                                *
//      *             Header file for TinyStepper_28BYJ_48.c             *
//      *                                                                *
//      *               Copyright (c) S. Reifel & Co, 2017               *
//      *                                                                *
//      ******************************************************************


#ifndef TinyStepper_28BYJ_48_h
#define TinyStepper_28BYJ_48_h

#include <arduino.h>
#include <stdlib.h>


//
// the TinyStepper_28BYJ_48 class
//
class TinyStepper_28BYJ_48
{
  public:
    //
    // public functions
    //
    TinyStepper_28BYJ_48();
    void connectToPins(byte in1PinNumber, byte in2PinNumber, byte in3PinNumber, byte in4PinNumber);
    void setCurrentPositionInSteps(long currentPositionInSteps);
    long getCurrentPositionInSteps();
    void setupStop();
    void setSpeedInStepsPerSecond(float speedInStepsPerSecond);
    void setAccelerationInStepsPerSecondPerSecond(float accelerationInStepsPerSecondPerSecond);
    void moveRelativeInSteps(long distanceToMoveInSteps);
    void setupRelativeMoveInSteps(long distanceToMoveInSteps);
    void moveToPositionInSteps(long absolutePositionToMoveToInSteps);
    void setupMoveInSteps(long absolutePositionToMoveToInSteps);
    bool motionComplete();
    float getCurrentVelocityInStepsPerSecond(); 
    bool processMovement(void);
    void disableMotor();


  private:
    //
    // private functions
    //
    void setNextFullStep(int direction);

    
    //
    // private member variables
    //
    byte in1Pin = 0;
    byte in2Pin = 0;
    byte in3Pin = 0;
    byte in4Pin = 0;
    float desiredSpeed_InStepsPerSecond;
    float acceleration_InStepsPerSecondPerSecond;
    long targetPosition_InSteps;
    bool startNewMove;
    float desiredStepPeriod_InUS;
    long decelerationDistance_InSteps;
    int direction_Scaler;
    float ramp_InitialStepPeriod_InUS;
    float ramp_NextStepPeriod_InUS;
    unsigned long ramp_LastStepTime_InUS;
    float acceleration_InStepsPerUSPerUS;
    float currentStepPeriod_InUS;
    long currentPosition_InSteps;
    int stepPhase;
};

// ------------------------------------ End ---------------------------------
#endif

