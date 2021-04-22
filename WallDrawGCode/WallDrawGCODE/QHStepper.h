
#ifndef QH_STEPPER_H
#define QH_STEPPER_H

#include "QH_Configuration.h"

void stepper_init();
void buffer_line_to_destination();
void buffer_arc_to_destination( float (&offset)[2], bool clockwise );

#endif
