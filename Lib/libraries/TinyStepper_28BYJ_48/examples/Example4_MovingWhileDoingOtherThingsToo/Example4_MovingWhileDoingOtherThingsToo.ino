
//      ******************************************************************
//      *                                                                *
//      *    Example shows how to move while testing other conditions    *
//      *    at the same time                                            *
//      *                                                                *
//      *            S. Reifel & Co.                6/24/2018            *
//      *                                                                *
//      ******************************************************************


// This sketch shows how you can do other things while the motor is running.  
// Examples of "other things" might be to:  1) Decelerate to a stop once a 
// button is press.  2) Turn on a solenoid when the motor moves past 
// position 850.  3) Flash a strobe every 200 steps.  4) Run continuously as 
// long as a temperature is below 125 degrees.
//
// There are some limitations to consider:
// 
//    1) The code that you run while the stepper is moving needs to 
//    execute VERY fast.  Perhaps no longer than 0.05 milliseconds.  
//
//    2) The only change that you can make to a motion once it starts 
//    moving is to decelerate to a stop.  This library does not allow 
//    changing the target position or speed while the motor is moving.  
//  
//
// Documentation at:
//         https://github.com/Stan-Reifel/TinyStepper_28BYJ_48

// ***********************************************************************


#include <TinyStepper_28BYJ_48.h>


//
// pin assignments
//
const int LED_PIN = 13;

const int MOTOR_IN1_PIN = 11;
const int MOTOR_IN2_PIN = 10;
const int MOTOR_IN3_PIN = 6;
const int MOTOR_IN4_PIN = 5;

const int STOP_BUTTON_PIN = 9;


//
// create the stepper motor object
//
TinyStepper_28BYJ_48 stepper;



void setup() 
{
  //
  // setup the LED pin, stop button pin and enable print statements
  //
  pinMode(LED_PIN, OUTPUT);   
  pinMode(STOP_BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);


  //
  // connect and configure the stepper motor to its IO pins
  //
 stepper.connectToPins(MOTOR_IN1_PIN, MOTOR_IN2_PIN, MOTOR_IN3_PIN, MOTOR_IN4_PIN);
}



void loop() 
{
  //
  // set the speed and acceleration rates for the stepper motor
  //
  stepper.setSpeedInStepsPerSecond(400);
  stepper.setAccelerationInStepsPerSecondPerSecond(600);


  //
  // set the motor's current positon to 0 and turn off the LED
  //
  stepper.setCurrentPositionInSteps(0);
  digitalWrite(LED_PIN, LOW);
  bool stopFlag = false;


  //
  // setup the motor so that it will rotate 10 turns, note: this 
  // command does not start moving yet
  //
  stepper.setupMoveInSteps(2048 * 10);
  

  //
  // now execute the move, looping until the motor has finished
  //
  while(!stepper.motionComplete())
  {
    //
    // Note: The code added to this loop must execute VERY fast.  
    // Perhaps no longer than 0.05 milliseconds.
    //
    
    //
    // process motor steps
    //
    stepper.processMovement();

    
    //
    // check if motor has moved past position 2048, if so turn On the LED
    //
    if (stepper.getCurrentPositionInSteps() == 2048)
      digitalWrite(LED_PIN, HIGH);


    //
    // check if the user has pressed the "Stop" button, if so decelerate to a stop
    //
    if ((digitalRead(STOP_BUTTON_PIN) == LOW) && (stopFlag == false))
    {
      stepper.setupStop();
      stopFlag = true;
    }
  }


  //
  // delay before starting again
  //
  delay(4000);
}

