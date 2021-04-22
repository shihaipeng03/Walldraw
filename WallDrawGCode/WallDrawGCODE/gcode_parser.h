
#ifndef QH_GCODE_PARSER_H
#define QH_GCODE_PARSER_H

#include "QH_Configuration.h"
#include "QHStepper.h"

void process_parsed_command();
void gcode_G0_G1();
void gcode_G2_G3( bool clockwise );
void gcode_G4();
void gcode_M3();
void gcode_M5();

#endif
