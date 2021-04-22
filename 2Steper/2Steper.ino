//淘宝『有名称的店铺』https://somebodys.taobao.com/
//更新日期 2019/06/04
//Wall Drawing Machine 墙画机 程序
//本程序对应商品 https://item.taobao.com/item.htm?id=597354643355

//web版程序连接：
//Github版链接：  https://github.com/shihaipeng03/Walldraw

//部分系统有不不能正常安装驱动的情况，可以手工安装驱动（CH340 CH341都可以适用）。
//WIN驱动下载链接： https://sparks.gogo.co.nz/assets/_site_/downloads/CH34x_Install_Windows_v3_4.zip  
//其他系统请到  https://sparks.gogo.co.nz/ch340.html  （CH34X驱动大全或自行搜索） 
//注意，x64的win7系统或者是ghost的系统可能会无法安装驱动。


//墙画机无舵机测试程序。本程序只测试2只步进电机，画曲线花纹，修改参数可以改变花纹尺寸和样式。



#include <AccelStepper.h>
//此lib库文件在程序包中，需要先复制到arduiino的\libraries文件夹下。
//方法1： 复制到 我的文档\Arduino\libraries 中
//方法2： 在arduino IDE的菜单中选择 项目->加载库->管理库 中搜索AccelStepper，自动安装

#define FULLSTEP 4
#define HALFSTEP 8
//接线方法 
//电机1的接法
#define motorPin1  2     //  28BYJ48 pin 1 接 arduino的2#口
#define motorPin2  3     //  28BYJ48 pin 2 接 3#
#define motorPin3  5     //  28BYJ48 pin 3 接 5#  ！注意，4#口预留sd读卡器
#define motorPin4  6     //  28BYJ48 pin 4 接 6#

//电机2的接法
#define motorPin5  7     //  28BYJ48 pin 1 接 7#
#define motorPin6  8     //  28BYJ48 pin 2 接 8#
#define motorPin7  9     //  28BYJ48 pin 3 接 9#
#define motorPin8  10    //  28BYJ48 pin 4 接 10#


#define stp1 279		//
#define stp2 673		//修改此参数，可以改变图案的尺寸和样式，数值越大图案越大，差距越大越复杂。达到2数的最小公倍数后图案封闭。


AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
AccelStepper stepper2(HALFSTEP, motorPin5, motorPin7, motorPin6, motorPin8);

void setup() 
{
  Serial.begin(9600);
  stepper1.setMaxSpeed(800.0);     //最大速度，过高扭矩变小，超过256容易丢步
  stepper1.setAcceleration(200.0);  //加速度，试稳定程序可以调节。
  stepper1.setSpeed(50);           //速度
  //视 FULLSTEP 或 HALFSTEP， 1024 或 512 步进电机转一周   
  //设置电机1的旋转步数 可调节（数字越大，图形尺寸越大）
  stepper1.moveTo(stp1);
  
  stepper2.setMaxSpeed(800.0);
  stepper2.setAcceleration(200.0);
  stepper2.setSpeed(50);
  //同stepper1
  stepper2.moveTo(stp2);  
  
}
void loop()  
{
  if(stepper1.distanceToGo() == 0)
    stepper1.moveTo(-stepper1.currentPosition());
  if(stepper2.distanceToGo() == 0)
    stepper2.moveTo(-stepper2.currentPosition());
    
  stepper1.run();
  stepper2.run();
    
}

//此程序无限循环，不会结束。
