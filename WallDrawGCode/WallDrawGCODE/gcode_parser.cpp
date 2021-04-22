#include "gcode_parser.h"

void process_parsed_command() {
   gcode_command.toUpperCase();
   if(gcode_command.indexOf('G') > -1){
      switch(gcode_command.substring(gcode_command.indexOf('G')+1,gcode_command.indexOf('G')+2) .toInt()){
        case 0:  
        case 1:   gcode_G0_G1();  break;
        case 2:   gcode_G2_G3(true); break;
        case 3:   gcode_G2_G3(false); break;
        case 4:   gcode_G4();     break;      
      }
   }else if(gcode_command.indexOf('M') > -1){
      switch(gcode_command.substring(gcode_command.indexOf('M')+1,gcode_command.indexOf('M')+2) .toInt()){
        case 3:   gcode_M3();   break;
        case 4:   gcode_M5();   break;
      }
   }
}


void gcode_G0_G1(){
	if( gcode_command.indexOf('X') > -1){
		if( gcode_command.indexOf('Y') > -1 ) destination[X_AXIS] = gcode_command.substring(gcode_command.indexOf('X')+1,gcode_command.indexOf('Y')).toFloat();
		else if( gcode_command.indexOf('Z') > -1 ) destination[X_AXIS] = gcode_command.substring(gcode_command.indexOf('X')+1,gcode_command.indexOf('Z')).toFloat(); 
		else if( gcode_command.indexOf('S') > -1 ) destination[X_AXIS] = gcode_command.substring(gcode_command.indexOf('X')+1,gcode_command.indexOf('S')).toFloat(); 
		else destination[X_AXIS] = gcode_command.substring(gcode_command.indexOf('X')+1,gcode_command.length()).toFloat();
	}
	if( gcode_command.indexOf('Y') > -1){
		if( gcode_command.indexOf('Z') > -1 ) destination[Y_AXIS] = gcode_command.substring(gcode_command.indexOf('Y')+1,gcode_command.indexOf('Z')).toFloat(); 
		else if( gcode_command.indexOf('S') > -1 ) destination[Y_AXIS] = gcode_command.substring(gcode_command.indexOf('Y')+1,gcode_command.indexOf('S')).toFloat(); 
		else destination[Y_AXIS] = gcode_command.substring(gcode_command.indexOf('Y')+1,gcode_command.length()).toFloat();
	}
	
	if( gcode_command.indexOf('Z') > -1){
		if( gcode_command.indexOf('S') > -1 ) destination[Z_AXIS] = gcode_command.substring(gcode_command.indexOf('Z')+1,gcode_command.indexOf('S')).toFloat(); 
		else destination[Z_AXIS] = gcode_command.substring(gcode_command.indexOf('Z')+1,gcode_command.length()).toFloat();
	}
//	Serial.print("G01 X"); Serial.print(destination[X_AXIS]);
//	Serial.print("Y"); Serial.print(destination[Y_AXIS]);
//	Serial.print("Z"); Serial.println(destination[Z_AXIS]);
//	
	buffer_line_to_destination();
	
}

void gcode_G2_G3( bool clockwise ){
	if( gcode_command.indexOf('X') > -1){
		if( gcode_command.indexOf('Y') > -1 ) destination[X_AXIS] = gcode_command.substring(gcode_command.indexOf('X')+1,gcode_command.indexOf('Y')).toFloat();
		else if( gcode_command.indexOf('Z') > -1 ) destination[X_AXIS] = gcode_command.substring(gcode_command.indexOf('X')+1,gcode_command.indexOf('Z')).toFloat(); 
		else if( gcode_command.indexOf('S') > -1 ) destination[X_AXIS] = gcode_command.substring(gcode_command.indexOf('X')+1,gcode_command.indexOf('S')).toFloat(); 
		else destination[X_AXIS] = gcode_command.substring(gcode_command.indexOf('X')+1,gcode_command.length()).toFloat();
	}
	if( gcode_command.indexOf('Y') > -1){
		if( gcode_command.indexOf('Z') > -1 ) destination[Y_AXIS] = gcode_command.substring(gcode_command.indexOf('Y')+1,gcode_command.indexOf('Z')).toFloat(); 
		else if( gcode_command.indexOf('S') > -1 ) destination[Y_AXIS] = gcode_command.substring(gcode_command.indexOf('Y')+1,gcode_command.indexOf('S')).toFloat(); 
		else destination[Y_AXIS] = gcode_command.substring(gcode_command.indexOf('Y')+1,gcode_command.length()).toFloat();
	}
	
	float arc_offset[2] = { 0.0, 0.0 };
	
	if( gcode_command.indexOf('R') > -1){
		float r = 0,
		      p1 = current_position[X_AXIS], q1 = current_position[Y_AXIS],
              p2 = destination[X_AXIS], q2 = destination[Y_AXIS];
		if( gcode_command.indexOf('S') > -1 ) r = gcode_command.substring(gcode_command.indexOf('R')+1,gcode_command.indexOf('S')).toFloat(); 
		else r = gcode_command.substring(gcode_command.indexOf('R')+1,gcode_command.length()).toFloat();
		
		if (r && (p2 != p1 || q2 != q1)) {
          const float e = clockwise ^ (r < 0) ? -1 : 1,           
                      dx = p2 - p1, dy = q2 - q1,                 
                      d = sqrt(sq(dx)+sq(dy)),                          
                      h = sqrt(sq(r) - sq(d * 0.5)),               
                      mx = (p1 + p2) * 0.5, my = (q1 + q2) * 0.5,  
                      sx = -dy / d, sy = dx / d,                   
                      cx = mx + e * h * sx, cy = my + e * h * sy;  
          arc_offset[0] = cx - p1;
          arc_offset[1] = cy - q1;
	    }
	} else {
        if (gcode_command.indexOf('I') > -1) {
			if( gcode_command.indexOf('J') > -1 ) arc_offset[0] = gcode_command.substring(gcode_command.indexOf('I')+1,gcode_command.indexOf('J')).toFloat(); 
			else if( gcode_command.indexOf('S') > -1 ) arc_offset[0] = gcode_command.substring(gcode_command.indexOf('I')+1,gcode_command.indexOf('S')).toFloat(); 
			else arc_offset[0] = gcode_command.substring(gcode_command.indexOf('I')+1,gcode_command.length()).toFloat();
		}
        if (gcode_command.indexOf('J') > -1){
			if( gcode_command.indexOf('S') > -1 ) arc_offset[1] = gcode_command.substring(gcode_command.indexOf('J')+1,gcode_command.indexOf('S')).toFloat(); 
			else arc_offset[1] = gcode_command.substring(gcode_command.indexOf('J')+1,gcode_command.length()).toFloat();
		} 
    }
	Serial.print("G23 X"); Serial.print( destination[X_AXIS] );
	Serial.print("Y"); Serial.print( destination[Y_AXIS] );
	Serial.print("I"); Serial.print( arc_offset[0] );
	Serial.print("J"); Serial.println( arc_offset[1] );
	
	buffer_arc_to_destination( arc_offset, clockwise );
}

void gcode_G4(){
	Serial.println("G4"); 
}

void gcode_M3(){
	Serial.println("M3"); 
}

void gcode_M5(){
	Serial.println("M5"); 
}
