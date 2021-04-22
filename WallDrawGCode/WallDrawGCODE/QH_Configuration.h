#ifndef QH_CONFIGURATION_H
#define QH_CONFIGURATION_H

#include <Arduino.h>

#define XY 2
#define XYZ 3
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define BAUDRATE            (115200)    //串口速率，用于传输G代码或调试 可选9600，57600，115200 或其他常用速率

#define STEPS_PER_TURN  (2048)  //步进电机一周步长 2048步转360度
#define SPOOL_DIAMETER  (35)    //线轴直径mm
#define SPOOL_CIRC      (SPOOL_DIAMETER * 3.1416)  //线轴周长 35*3.14=109.956
#define DEFAULT_XY_MM_PER_STEP    (SPOOL_CIRC / STEPS_PER_TURN)  //步进电机步距，最小分辨率 每步线绳被拉动的距离  0.053689mm

#define MM_PER_ARC_SEGMENT  1  //圆弧分割最小每步距离 
#define N_ARC_CORRECTION   25  //修正之间的极化段数


#define X_SEPARATION  505           //两绳上方的水平距离mm 
#define X_MAX_POS       ( X_SEPARATION*0.5)   //x轴最大值  0位在画板中心
#define X_MIN_POS       (-X_SEPARATION*0.5)   //x轴最小值

/* 垂直距离的参数： 正值在画板下放，理论上只要画板够大可以无限大，负值区域在笔（开机前）的上方*/
#define Y_MAX_POS         (-300)   //y轴最大值 画板最下方
#define Y_MIN_POS         (300)    //y轴最小值 画板最上方  左右两线的固定点到笔的垂直距离，尽量测量摆放准确，误差过大会有畸变

#define LINE_DELAY      1   //步进电机每步的等候时间 （微妙）

//两个电机的旋转方向  1正转  -1反转  
//调节进出方向可垂直反转图像
#define INVERT_M1_DIR     1        
#define INVERT_M2_DIR     -1 

#define M_PI 3.14159265358979323846
#define RADIANS(d) ((d)*M_PI/180.0)
#define ATAN2(y, x) atan2(y, x)
#define SQRT(x)     sqrt(x)
#define HYPOT2(x,y) (sq(x)+sq(y))
#define HYPOT(x,y)  SQRT(HYPOT2(x,y))

extern String gcode_command;
extern float destination[XYZ];
extern float current_position[XYZ];
extern long current_steps_M1, current_steps_M2; //当前步进电机相对于0点位置总步数

#endif
