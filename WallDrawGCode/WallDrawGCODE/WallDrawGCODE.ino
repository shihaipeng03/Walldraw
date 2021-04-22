#include "QH_Configuration.h"
#include "gcode_parser.h"
#include "QHStepper.h"

#include <Servo.h>

String gcode_command="";
float destination[XYZ] = {0,0,0};
float current_position[XYZ] = {0,0,0};
long current_steps_M1 = 0, current_steps_M2 = 0;

void setup() {
  Serial.begin(115200);
  stepper_init();
  delay(1200);
  Serial.println("Grbl 1.1h ['$' for help]");
  delay(1200);
  Serial.println("<Idle|MPos:0.000,0.000,0.000|FS:0,0|Ov:100,100,100>");
}

void loop() {
  if( get_command() > 0 ){
    process_parsed_command();
    gcode_command = "";
    Serial.println("ok");
  } 
}

byte get_command(){
  static int len = 0;
  static char tmpC = 0;
  while(Serial.available() > 0){
    tmpC = Serial.read();
    if(tmpC == 0x3f) continue;
    if(tmpC == 10 || len > 35 ){  len = 0; return 1;}
    gcode_command += tmpC;
    len++;
  }
  return 0;
}
