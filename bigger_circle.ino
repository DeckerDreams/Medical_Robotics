#include <Servo.h>

#define ROBOT_GEEK_9G_GRIPPER 1
#define ROBOT_GEEK_PARALLEL_GRIPPER 2

//The 9G gripper is the gripper with the small blue 9g servo
//The Parralle gripper has a full robotgeek servo and paralle rails
//Uncomment one of the following lines depending on which gripper you are using.
//#define GRIPPER_TYPE ROBOT_GEEK_9G_GRIPPER
#define GRIPPER_TYPE ROBOT_GEEK_PARALLEL_GRIPPER

#ifndef GRIPPER_TYPE
   #error YOU HAVE TO SELECT THE GRIPPER YOU ARE USING! Uncomment the correct line above for your gripper
#endif

#define DELAY_TIME 2 //milliseconds to wait between

//define analog pins that will be connected to the joystick pins
#define BASE      0  //connected to Rotation Knob / Potentiometer # 1
#define SHOULDER  1  //connected to Vertical Axis on Joystick # 1
#define ELBOW     2  //connected to Vertical Axis on Joystick # 2
#define WRIST     3  //connected to Vertical Axis on Joystick # 3
#define GRIPPER   4  //connected to Rotation Knob / Potentiometer # 2

// Servo position limitations - limits in microseconds
#define BASE_MIN      600     //full counterclockwise for RobotGeek 180 degree servo
#define BASE_MAX      2400    //full clockwise for RobotGeek 180 degree servo
#define SHOULDER_MIN  600
#define SHOULDER_MAX  1800
#define ELBOW_MIN     1200
#define ELBOW_MAX     2400
#define WRIST_MIN     600
#define WRIST_MAX     2400 

//mins and maxes depending on gripper type
#if GRIPPER_TYPE == ROBOT_GEEK_9G_GRIPPER
  #define GRIPPER_MIN   900   //full counterclockwise for 9g servo
  #define GRIPPER_MAX   2100  //full clockwise for 9g servo
#elif GRIPPER_TYPE == ROBOT_GEEK_PARALLEL_GRIPPER
  #define GRIPPER_MIN   750   //fully open
  #define GRIPPER_MAX   2400  //fully closed
#endif

#define CENTERED      1500

//generic deadband limits - not all joystics will center at 512, so these limits remove 'drift' from joysticks that are off-center.
#define DEADBANDLOW 462 //decrease this value if drift occurs, increase it to increase sensitivity around the center position
#define DEADBANDHIGH 562  //increase this value if drift occurs, decrease it to increase sensitivity around the center position

// Declare servo objects
Servo BAS_SERVO;  //base servo - RobotGeek Servo
Servo SHL_SERVO;  //shoulder servo - RobotGeek Servo 
Servo ELB_SERVO;  //elbow servo - RobotGeek Servo 
Servo WRI_SERVO;  //wrist servo - RobotGeek Servo
Servo WRO_SERVO;  //wrist rotate servo - RobotGeek Servo (unused for snapper arm)        
Servo GRI_SERVO;  //gripper servo - 9g servo

//present positions of the servos 
int Base     =1470;  //holds the present position of the Base servo, starts at 1500 (centered)
int Shoulder =1385;  //holds the present position of the Shoulder servo, starts at 1500 (centered)
int Elbow    =1500;  //holds the present position of the Elbow servo, starts at 1500 (centered)
int Wrist    =1475;  //holds the present position of the wrist servo, starts at 1500 (centered)
int Gripper  =750;  //holds the present position of the gripper servo, starts at 1500 (centered)

//last read values of analog sensors (Native values, 0-1023)
int joyBaseVal = 0;     //present value of the base rotation knob (analog 0)
int joyShoulderVal = 0; //present value of the shoulder joystick (analog 1)
int joyElbowVal = 0;    //present value of the elbow joystick (analog 2)
int joyWristVal = 0;    //present value of the wrist joystick (analog 3)
int joyGripperVal = 0;  //present value of the gripper rotation knob (analog 4)

//last calculated values of analog sensors (Mapped values)
//knob values (base and gripper) will be mapped directly to the servo limits
//joystick values (shoulder, elbow and wrist) will be mapped from -speed to speed, to faciliate incremental control
int joyBaseMapped = 0;      //base knob value, mapped from 1-1023 to BASE_MIN-BASE_MAX
int joyShoulderMapped = 0;  //shoulder joystick value, mapped from 1-1023 to -speed to speed
int joyElbowMapped = 0;     //elbow joystick value, mapped from 1-1023 to -speed to speed
int joyWristMapped = 0;     //wrist joystick value, mapped from 1-1023 to -speed to speed
int joyGripperMapped = 0;   //gripper knob  value, mapped from 1-1023 to GRIPPER_MIN-GRIPPER_MAX

int speed = 2;  //speed modififer, increase this to increase the speed of the movement

//============================================================================
// Setup 
//============================================================================

void setup() 
{
  // Attach servo and set limits
  BAS_SERVO.attach(3, BASE_MIN, BASE_MAX); 
  SHL_SERVO.attach(5, SHOULDER_MIN, SHOULDER_MAX);
  ELB_SERVO.attach(6, ELBOW_MIN, ELBOW_MAX);
  WRI_SERVO.attach(9, WRIST_MIN, WRIST_MAX);
  GRI_SERVO.attach(10, GRIPPER_MIN, GRIPPER_MAX);
  
  delay(100);  //wait 1 second
  
  set_servo();  // Move servos to default positions
  delay(1000);
  // q1 = base
  // q2 = shoulder
  // 0
  // q3 (-) elbow
  // q4 (-) wrist
  // 0

  //circle delay time
 int d = 3;

  //grab pen
  moveServo(WRI_SERVO, Wrist, Wrist-525);
  delay(1000);
  moveServo(SHL_SERVO, Shoulder, Shoulder+220);
  delay(1000);
  moveServo(ELB_SERVO, Elbow, Elbow-160);
  delay(1000);

  moveServo(GRI_SERVO, Gripper, Gripper+777); // + close; - open 
  delay(1000);

  //lift up pen
  moveServo(ELB_SERVO, Elbow-160, Elbow+100);
  delay(1000);


//make a circle
  //start circle
    delay(100);
  moveServo(WRI_SERVO, Wrist-525, Wrist+401);
  delay(250);
  moveServo(ELB_SERVO, Elbow+100, Elbow-29);
  moveServo(SHL_SERVO, Shoulder+220, Shoulder+372);
  delay(1200);


moveServo(BAS_SERVO, Base+0, Base+8);
moveServo(SHL_SERVO, Shoulder+372, Shoulder+372);
moveServo(ELB_SERVO, Elbow-29, Elbow-30);
moveServo(WRI_SERVO, Wrist+401, Wrist+401);
delay(d);
moveServo(BAS_SERVO, Base+8, Base+16);
moveServo(SHL_SERVO, Shoulder+372, Shoulder+370);
moveServo(ELB_SERVO, Elbow-30, Elbow-33);
moveServo(WRI_SERVO, Wrist+401, Wrist+403);
delay(d);
moveServo(BAS_SERVO, Base+16, Base+24);
moveServo(SHL_SERVO, Shoulder+370, Shoulder+368);
moveServo(ELB_SERVO, Elbow-33, Elbow-38);
moveServo(WRI_SERVO, Wrist+403, Wrist+406);
delay(d);
moveServo(BAS_SERVO, Base+24, Base+32);
moveServo(SHL_SERVO, Shoulder+368, Shoulder+365);
moveServo(ELB_SERVO, Elbow-38, Elbow-45);
moveServo(WRI_SERVO, Wrist+406, Wrist+409);
delay(d);
moveServo(BAS_SERVO, Base+32, Base+40);
moveServo(SHL_SERVO, Shoulder+365, Shoulder+361);
moveServo(ELB_SERVO, Elbow-45, Elbow-54);
moveServo(WRI_SERVO, Wrist+409, Wrist+414);
delay(d);
moveServo(BAS_SERVO, Base+40, Base+48);
moveServo(SHL_SERVO, Shoulder+361, Shoulder+356);
moveServo(ELB_SERVO, Elbow-54, Elbow-64);
moveServo(WRI_SERVO, Wrist+414, Wrist+420);
delay(d);
moveServo(BAS_SERVO, Base+48, Base+56);
moveServo(SHL_SERVO, Shoulder+356, Shoulder+350);
moveServo(ELB_SERVO, Elbow-64, Elbow-77);
moveServo(WRI_SERVO, Wrist+420, Wrist+427);
delay(d);
moveServo(BAS_SERVO, Base+56, Base+63);
moveServo(SHL_SERVO, Shoulder+350, Shoulder+344);
moveServo(ELB_SERVO, Elbow-77, Elbow-91);
moveServo(WRI_SERVO, Wrist+427, Wrist+434);
delay(d);
moveServo(BAS_SERVO, Base+63, Base+70);
moveServo(SHL_SERVO, Shoulder+344, Shoulder+337);
moveServo(ELB_SERVO, Elbow-91, Elbow-106);
moveServo(WRI_SERVO, Wrist+434, Wrist+443);
delay(d);
moveServo(BAS_SERVO, Base+70, Base+78);
moveServo(SHL_SERVO, Shoulder+337, Shoulder+329);
moveServo(ELB_SERVO, Elbow-106, Elbow-123);
moveServo(WRI_SERVO, Wrist+443, Wrist+452);
delay(d);
moveServo(BAS_SERVO, Base+78, Base+85);
moveServo(SHL_SERVO, Shoulder+329, Shoulder+321);
moveServo(ELB_SERVO, Elbow-123, Elbow-141);
moveServo(WRI_SERVO, Wrist+452, Wrist+462);
delay(d);
moveServo(BAS_SERVO, Base+85, Base+91);
moveServo(SHL_SERVO, Shoulder+321, Shoulder+312);
moveServo(ELB_SERVO, Elbow-141, Elbow-161);
moveServo(WRI_SERVO, Wrist+462, Wrist+473);
delay(d);
moveServo(BAS_SERVO, Base+91, Base+98);
moveServo(SHL_SERVO, Shoulder+312, Shoulder+303);
moveServo(ELB_SERVO, Elbow-161, Elbow-181);
moveServo(WRI_SERVO, Wrist+473, Wrist+484);
delay(d);
moveServo(BAS_SERVO, Base+98, Base+104);
moveServo(SHL_SERVO, Shoulder+303, Shoulder+293);
moveServo(ELB_SERVO, Elbow-181, Elbow-203);
moveServo(WRI_SERVO, Wrist+484, Wrist+496);
delay(d);
moveServo(BAS_SERVO, Base+104, Base+110);
moveServo(SHL_SERVO, Shoulder+293, Shoulder+283);
moveServo(ELB_SERVO, Elbow-203, Elbow-225);
moveServo(WRI_SERVO, Wrist+496, Wrist+508);
delay(d);
moveServo(BAS_SERVO, Base+110, Base+115);
moveServo(SHL_SERVO, Shoulder+283, Shoulder+273);
moveServo(ELB_SERVO, Elbow-225, Elbow-248);
moveServo(WRI_SERVO, Wrist+508, Wrist+521);
delay(d);
moveServo(BAS_SERVO, Base+115, Base+121);
moveServo(SHL_SERVO, Shoulder+273, Shoulder+263);
moveServo(ELB_SERVO, Elbow-248, Elbow-272);
moveServo(WRI_SERVO, Wrist+521, Wrist+535);
delay(d);
moveServo(BAS_SERVO, Base+121, Base+125);
moveServo(SHL_SERVO, Shoulder+263, Shoulder+252);
moveServo(ELB_SERVO, Elbow-272, Elbow-296);
moveServo(WRI_SERVO, Wrist+535, Wrist+548);
delay(d);
moveServo(BAS_SERVO, Base+125, Base+130);
moveServo(SHL_SERVO, Shoulder+252, Shoulder+242);
moveServo(ELB_SERVO, Elbow-296, Elbow-320);
moveServo(WRI_SERVO, Wrist+548, Wrist+562);
delay(d);
moveServo(BAS_SERVO, Base+130, Base+134);
moveServo(SHL_SERVO, Shoulder+242, Shoulder+231);
moveServo(ELB_SERVO, Elbow-320, Elbow-345);
moveServo(WRI_SERVO, Wrist+562, Wrist+577);
delay(d);
moveServo(BAS_SERVO, Base+134, Base+137);
moveServo(SHL_SERVO, Shoulder+231, Shoulder+221);
moveServo(ELB_SERVO, Elbow-345, Elbow-371);
moveServo(WRI_SERVO, Wrist+577, Wrist+591);
delay(d);
moveServo(BAS_SERVO, Base+137, Base+140);
moveServo(SHL_SERVO, Shoulder+221, Shoulder+210);
moveServo(ELB_SERVO, Elbow-371, Elbow-396);
moveServo(WRI_SERVO, Wrist+591, Wrist+606);
delay(d);
moveServo(BAS_SERVO, Base+140, Base+142);
moveServo(SHL_SERVO, Shoulder+210, Shoulder+200);
moveServo(ELB_SERVO, Elbow-396, Elbow-422);
moveServo(WRI_SERVO, Wrist+606, Wrist+621);
delay(d);
moveServo(BAS_SERVO, Base+142, Base+144);
moveServo(SHL_SERVO, Shoulder+200, Shoulder+189);
moveServo(ELB_SERVO, Elbow-422, Elbow-447);
moveServo(WRI_SERVO, Wrist+621, Wrist+636);
delay(d);
moveServo(BAS_SERVO, Base+144, Base+145);
moveServo(SHL_SERVO, Shoulder+189, Shoulder+179);
moveServo(ELB_SERVO, Elbow-447, Elbow-472);
moveServo(WRI_SERVO, Wrist+636, Wrist+652);
delay(d);
moveServo(BAS_SERVO, Base+145, Base+145);
moveServo(SHL_SERVO, Shoulder+179, Shoulder+170);
moveServo(ELB_SERVO, Elbow-472, Elbow-497);
moveServo(WRI_SERVO, Wrist+652, Wrist+667);
delay(d);
moveServo(BAS_SERVO, Base+145, Base+144);
moveServo(SHL_SERVO, Shoulder+170, Shoulder+160);
moveServo(ELB_SERVO, Elbow-497, Elbow-522);
moveServo(WRI_SERVO, Wrist+667, Wrist+682);
delay(d);
moveServo(BAS_SERVO, Base+144, Base+143);
moveServo(SHL_SERVO, Shoulder+160, Shoulder+151);
moveServo(ELB_SERVO, Elbow-522, Elbow-546);
moveServo(WRI_SERVO, Wrist+682, Wrist+698);
delay(d);
moveServo(BAS_SERVO, Base+143, Base+141);
moveServo(SHL_SERVO, Shoulder+151, Shoulder+143);
moveServo(ELB_SERVO, Elbow-546, Elbow-570);
moveServo(WRI_SERVO, Wrist+698, Wrist+713);
delay(d);
moveServo(BAS_SERVO, Base+141, Base+138);
moveServo(SHL_SERVO, Shoulder+143, Shoulder+135);
moveServo(ELB_SERVO, Elbow-570, Elbow-593);
moveServo(WRI_SERVO, Wrist+713, Wrist+728);
delay(d);
moveServo(BAS_SERVO, Base+138, Base+134);
moveServo(SHL_SERVO, Shoulder+135, Shoulder+128);
moveServo(ELB_SERVO, Elbow-593, Elbow-616);
moveServo(WRI_SERVO, Wrist+728, Wrist+743);
delay(d);
moveServo(BAS_SERVO, Base+134, Base+129);
moveServo(SHL_SERVO, Shoulder+128, Shoulder+121);
moveServo(ELB_SERVO, Elbow-616, Elbow-637);
moveServo(WRI_SERVO, Wrist+743, Wrist+758);
delay(d);
moveServo(BAS_SERVO, Base+129, Base+124);
moveServo(SHL_SERVO, Shoulder+121, Shoulder+115);
moveServo(ELB_SERVO, Elbow-637, Elbow-658);
moveServo(WRI_SERVO, Wrist+758, Wrist+773);
delay(d);
moveServo(BAS_SERVO, Base+124, Base+117);
moveServo(SHL_SERVO, Shoulder+115, Shoulder+110);
moveServo(ELB_SERVO, Elbow-658, Elbow-677);
moveServo(WRI_SERVO, Wrist+773, Wrist+787);
delay(d);
moveServo(BAS_SERVO, Base+117, Base+109);
moveServo(SHL_SERVO, Shoulder+110, Shoulder+105);
moveServo(ELB_SERVO, Elbow-677, Elbow-695);
moveServo(WRI_SERVO, Wrist+787, Wrist+800);
delay(d);
moveServo(BAS_SERVO, Base+109, Base+101);
moveServo(SHL_SERVO, Shoulder+105, Shoulder+101);
moveServo(ELB_SERVO, Elbow-695, Elbow-712);
moveServo(WRI_SERVO, Wrist+800, Wrist+813);
delay(d);
moveServo(BAS_SERVO, Base+101, Base+91);
moveServo(SHL_SERVO, Shoulder+101, Shoulder+98);
moveServo(ELB_SERVO, Elbow-712, Elbow-727);
moveServo(WRI_SERVO, Wrist+813, Wrist+825);
delay(d);
moveServo(BAS_SERVO, Base+91, Base+81);
moveServo(SHL_SERVO, Shoulder+98, Shoulder+95);
moveServo(ELB_SERVO, Elbow-727, Elbow-741);
moveServo(WRI_SERVO, Wrist+825, Wrist+836);
delay(d);
moveServo(BAS_SERVO, Base+81, Base+70);
moveServo(SHL_SERVO, Shoulder+95, Shoulder+93);
moveServo(ELB_SERVO, Elbow-741, Elbow-753);
moveServo(WRI_SERVO, Wrist+836, Wrist+846);
delay(d);
moveServo(BAS_SERVO, Base+70, Base+58);
moveServo(SHL_SERVO, Shoulder+93, Shoulder+92);
moveServo(ELB_SERVO, Elbow-753, Elbow-763);
moveServo(WRI_SERVO, Wrist+846, Wrist+855);
delay(d);
moveServo(BAS_SERVO, Base+58, Base+46);
moveServo(SHL_SERVO, Shoulder+92, Shoulder+91);
moveServo(ELB_SERVO, Elbow-763, Elbow-772);
moveServo(WRI_SERVO, Wrist+855, Wrist+862);
delay(d);
moveServo(BAS_SERVO, Base+46, Base+33);
moveServo(SHL_SERVO, Shoulder+91, Shoulder+90);
moveServo(ELB_SERVO, Elbow-772, Elbow-778);
moveServo(WRI_SERVO, Wrist+862, Wrist+868);
delay(d);
moveServo(BAS_SERVO, Base+33, Base+20);
moveServo(SHL_SERVO, Shoulder+90, Shoulder+90);
moveServo(ELB_SERVO, Elbow-778, Elbow-782);
moveServo(WRI_SERVO, Wrist+868, Wrist+872);
delay(d);
moveServo(BAS_SERVO, Base+20, Base+7);
moveServo(SHL_SERVO, Shoulder+90, Shoulder+89);
moveServo(ELB_SERVO, Elbow-782, Elbow-784);
moveServo(WRI_SERVO, Wrist+872, Wrist+874);
delay(d);
moveServo(BAS_SERVO, Base+7, Base-7);
moveServo(SHL_SERVO, Shoulder+89, Shoulder+89);
moveServo(ELB_SERVO, Elbow-784, Elbow-784);
moveServo(WRI_SERVO, Wrist+874, Wrist+874);
delay(d);
moveServo(BAS_SERVO, Base-7, Base-20);
moveServo(SHL_SERVO, Shoulder+89, Shoulder+90);
moveServo(ELB_SERVO, Elbow-784, Elbow-782);
moveServo(WRI_SERVO, Wrist+874, Wrist+872);
delay(d);
moveServo(BAS_SERVO, Base-20, Base-33);
moveServo(SHL_SERVO, Shoulder+90, Shoulder+90);
moveServo(ELB_SERVO, Elbow-782, Elbow-778);
moveServo(WRI_SERVO, Wrist+872, Wrist+868);
delay(d);
moveServo(BAS_SERVO, Base-33, Base-46);
moveServo(SHL_SERVO, Shoulder+90, Shoulder+91);
moveServo(ELB_SERVO, Elbow-778, Elbow-772);
moveServo(WRI_SERVO, Wrist+868, Wrist+862);
delay(d);
moveServo(BAS_SERVO, Base-46, Base-58);
moveServo(SHL_SERVO, Shoulder+91, Shoulder+92);
moveServo(ELB_SERVO, Elbow-772, Elbow-763);
moveServo(WRI_SERVO, Wrist+862, Wrist+855);
delay(d);
moveServo(BAS_SERVO, Base-58, Base-70);
moveServo(SHL_SERVO, Shoulder+92, Shoulder+93);
moveServo(ELB_SERVO, Elbow-763, Elbow-753);
moveServo(WRI_SERVO, Wrist+855, Wrist+846);
delay(d);
moveServo(BAS_SERVO, Base-70, Base-81);
moveServo(SHL_SERVO, Shoulder+93, Shoulder+95);
moveServo(ELB_SERVO, Elbow-753, Elbow-741);
moveServo(WRI_SERVO, Wrist+846, Wrist+836);
delay(d);
moveServo(BAS_SERVO, Base-81, Base-91);
moveServo(SHL_SERVO, Shoulder+95, Shoulder+98);
moveServo(ELB_SERVO, Elbow-741, Elbow-727);
moveServo(WRI_SERVO, Wrist+836, Wrist+825);
delay(d);
moveServo(BAS_SERVO, Base-91, Base-101);
moveServo(SHL_SERVO, Shoulder+98, Shoulder+101);
moveServo(ELB_SERVO, Elbow-727, Elbow-712);
moveServo(WRI_SERVO, Wrist+825, Wrist+813);
delay(d);
moveServo(BAS_SERVO, Base-101, Base-109);
moveServo(SHL_SERVO, Shoulder+101, Shoulder+105);
moveServo(ELB_SERVO, Elbow-712, Elbow-695);
moveServo(WRI_SERVO, Wrist+813, Wrist+800);
delay(d);
moveServo(BAS_SERVO, Base-109, Base-117);
moveServo(SHL_SERVO, Shoulder+105, Shoulder+110);
moveServo(ELB_SERVO, Elbow-695, Elbow-677);
moveServo(WRI_SERVO, Wrist+800, Wrist+787);
delay(d);
moveServo(BAS_SERVO, Base-117, Base-124);
moveServo(SHL_SERVO, Shoulder+110, Shoulder+115);
moveServo(ELB_SERVO, Elbow-677, Elbow-658);
moveServo(WRI_SERVO, Wrist+787, Wrist+773);
delay(d);
moveServo(BAS_SERVO, Base-124, Base-129);
moveServo(SHL_SERVO, Shoulder+115, Shoulder+121);
moveServo(ELB_SERVO, Elbow-658, Elbow-637);
moveServo(WRI_SERVO, Wrist+773, Wrist+758);
delay(d);
moveServo(BAS_SERVO, Base-129, Base-134);
moveServo(SHL_SERVO, Shoulder+121, Shoulder+128);
moveServo(ELB_SERVO, Elbow-637, Elbow-616);
moveServo(WRI_SERVO, Wrist+758, Wrist+743);
delay(d);
moveServo(BAS_SERVO, Base-134, Base-138);
moveServo(SHL_SERVO, Shoulder+128, Shoulder+135);
moveServo(ELB_SERVO, Elbow-616, Elbow-593);
moveServo(WRI_SERVO, Wrist+743, Wrist+728);
delay(d);
moveServo(BAS_SERVO, Base-138, Base-141);
moveServo(SHL_SERVO, Shoulder+135, Shoulder+143);
moveServo(ELB_SERVO, Elbow-593, Elbow-570);
moveServo(WRI_SERVO, Wrist+728, Wrist+713);
delay(d);
moveServo(BAS_SERVO, Base-141, Base-143);
moveServo(SHL_SERVO, Shoulder+143, Shoulder+151);
moveServo(ELB_SERVO, Elbow-570, Elbow-546);
moveServo(WRI_SERVO, Wrist+713, Wrist+698);
delay(d);
moveServo(BAS_SERVO, Base-143, Base-144);
moveServo(SHL_SERVO, Shoulder+151, Shoulder+160);
moveServo(ELB_SERVO, Elbow-546, Elbow-522);
moveServo(WRI_SERVO, Wrist+698, Wrist+682);
delay(d);
moveServo(BAS_SERVO, Base-144, Base-145);
moveServo(SHL_SERVO, Shoulder+160, Shoulder+170);
moveServo(ELB_SERVO, Elbow-522, Elbow-497);
moveServo(WRI_SERVO, Wrist+682, Wrist+667);
delay(d);
moveServo(BAS_SERVO, Base-145, Base-145);
moveServo(SHL_SERVO, Shoulder+170, Shoulder+179);
moveServo(ELB_SERVO, Elbow-497, Elbow-472);
moveServo(WRI_SERVO, Wrist+667, Wrist+652);
delay(d);
moveServo(BAS_SERVO, Base-145, Base-144);
moveServo(SHL_SERVO, Shoulder+179, Shoulder+189);
moveServo(ELB_SERVO, Elbow-472, Elbow-447);
moveServo(WRI_SERVO, Wrist+652, Wrist+636);
delay(d);
moveServo(BAS_SERVO, Base-144, Base-142);
moveServo(SHL_SERVO, Shoulder+189, Shoulder+200);
moveServo(ELB_SERVO, Elbow-447, Elbow-422);
moveServo(WRI_SERVO, Wrist+636, Wrist+621);
delay(d);
moveServo(BAS_SERVO, Base-142, Base-140);
moveServo(SHL_SERVO, Shoulder+200, Shoulder+210);
moveServo(ELB_SERVO, Elbow-422, Elbow-396);
moveServo(WRI_SERVO, Wrist+621, Wrist+606);
delay(d);
moveServo(BAS_SERVO, Base-140, Base-137);
moveServo(SHL_SERVO, Shoulder+210, Shoulder+221);
moveServo(ELB_SERVO, Elbow-396, Elbow-371);
moveServo(WRI_SERVO, Wrist+606, Wrist+591);
delay(d);
moveServo(BAS_SERVO, Base-137, Base-134);
moveServo(SHL_SERVO, Shoulder+221, Shoulder+231);
moveServo(ELB_SERVO, Elbow-371, Elbow-345);
moveServo(WRI_SERVO, Wrist+591, Wrist+577);
delay(d);
moveServo(BAS_SERVO, Base-134, Base-130);
moveServo(SHL_SERVO, Shoulder+231, Shoulder+242);
moveServo(ELB_SERVO, Elbow-345, Elbow-320);
moveServo(WRI_SERVO, Wrist+577, Wrist+562);
delay(d);
moveServo(BAS_SERVO, Base-130, Base-125);
moveServo(SHL_SERVO, Shoulder+242, Shoulder+252);
moveServo(ELB_SERVO, Elbow-320, Elbow-296);
moveServo(WRI_SERVO, Wrist+562, Wrist+548);
delay(d);
moveServo(BAS_SERVO, Base-125, Base-121);
moveServo(SHL_SERVO, Shoulder+252, Shoulder+263);
moveServo(ELB_SERVO, Elbow-296, Elbow-272);
moveServo(WRI_SERVO, Wrist+548, Wrist+535);
delay(d);
moveServo(BAS_SERVO, Base-121, Base-115);
moveServo(SHL_SERVO, Shoulder+263, Shoulder+273);
moveServo(ELB_SERVO, Elbow-272, Elbow-248);
moveServo(WRI_SERVO, Wrist+535, Wrist+521);
delay(d);
moveServo(BAS_SERVO, Base-115, Base-110);
moveServo(SHL_SERVO, Shoulder+273, Shoulder+283);
moveServo(ELB_SERVO, Elbow-248, Elbow-225);
moveServo(WRI_SERVO, Wrist+521, Wrist+508);
delay(d);
moveServo(BAS_SERVO, Base-110, Base-104);
moveServo(SHL_SERVO, Shoulder+283, Shoulder+293);
moveServo(ELB_SERVO, Elbow-225, Elbow-203);
moveServo(WRI_SERVO, Wrist+508, Wrist+496);
delay(d);
moveServo(BAS_SERVO, Base-104, Base-98);
moveServo(SHL_SERVO, Shoulder+293, Shoulder+303);
moveServo(ELB_SERVO, Elbow-203, Elbow-181);
moveServo(WRI_SERVO, Wrist+496, Wrist+484);
delay(d);
moveServo(BAS_SERVO, Base-98, Base-91);
moveServo(SHL_SERVO, Shoulder+303, Shoulder+312);
moveServo(ELB_SERVO, Elbow-181, Elbow-161);
moveServo(WRI_SERVO, Wrist+484, Wrist+473);
delay(d);
moveServo(BAS_SERVO, Base-91, Base-85);
moveServo(SHL_SERVO, Shoulder+312, Shoulder+321);
moveServo(ELB_SERVO, Elbow-161, Elbow-141);
moveServo(WRI_SERVO, Wrist+473, Wrist+462);
delay(d);
moveServo(BAS_SERVO, Base-85, Base-78);
moveServo(SHL_SERVO, Shoulder+321, Shoulder+329);
moveServo(ELB_SERVO, Elbow-141, Elbow-123);
moveServo(WRI_SERVO, Wrist+462, Wrist+452);
delay(d);
moveServo(BAS_SERVO, Base-78, Base-70);
moveServo(SHL_SERVO, Shoulder+329, Shoulder+337);
moveServo(ELB_SERVO, Elbow-123, Elbow-106);
moveServo(WRI_SERVO, Wrist+452, Wrist+443);
delay(d);
moveServo(BAS_SERVO, Base-70, Base-63);
moveServo(SHL_SERVO, Shoulder+337, Shoulder+344);
moveServo(ELB_SERVO, Elbow-106, Elbow-91);
moveServo(WRI_SERVO, Wrist+443, Wrist+434);
delay(d);
moveServo(BAS_SERVO, Base-63, Base-56);
moveServo(SHL_SERVO, Shoulder+344, Shoulder+350);
moveServo(ELB_SERVO, Elbow-91, Elbow-77);
moveServo(WRI_SERVO, Wrist+434, Wrist+427);
delay(d);
moveServo(BAS_SERVO, Base-56, Base-48);
moveServo(SHL_SERVO, Shoulder+350, Shoulder+356);
moveServo(ELB_SERVO, Elbow-77, Elbow-64);
moveServo(WRI_SERVO, Wrist+427, Wrist+420);
delay(d);
moveServo(BAS_SERVO, Base-48, Base-40);
moveServo(SHL_SERVO, Shoulder+356, Shoulder+361);
moveServo(ELB_SERVO, Elbow-64, Elbow-54);
moveServo(WRI_SERVO, Wrist+420, Wrist+414);
delay(d);
moveServo(BAS_SERVO, Base-40, Base-32);
moveServo(SHL_SERVO, Shoulder+361, Shoulder+365);
moveServo(ELB_SERVO, Elbow-54, Elbow-45);
moveServo(WRI_SERVO, Wrist+414, Wrist+409);
delay(d);
moveServo(BAS_SERVO, Base-32, Base-24);
moveServo(SHL_SERVO, Shoulder+365, Shoulder+368);
moveServo(ELB_SERVO, Elbow-45, Elbow-38);
moveServo(WRI_SERVO, Wrist+409, Wrist+406);
delay(d);
moveServo(BAS_SERVO, Base-24, Base-16);
moveServo(SHL_SERVO, Shoulder+368, Shoulder+370);
moveServo(ELB_SERVO, Elbow-38, Elbow-33);
moveServo(WRI_SERVO, Wrist+406, Wrist+403);
delay(d);
moveServo(BAS_SERVO, Base-16, Base-8);
moveServo(SHL_SERVO, Shoulder+370, Shoulder+372);
moveServo(ELB_SERVO, Elbow-33, Elbow-30);
moveServo(WRI_SERVO, Wrist+403, Wrist+401);
delay(d);
moveServo(BAS_SERVO, Base-8, Base+0);
moveServo(SHL_SERVO, Shoulder+372, Shoulder+372);
moveServo(ELB_SERVO, Elbow-30, Elbow-29);
moveServo(WRI_SERVO, Wrist+401, Wrist+401);
delay(1000);

moveServo(WRI_SERVO, Wrist+401, Wrist+700);
 moveServo(ELB_SERVO, Elbow-30, Elbow+100);
  delay(1000);

//start smile

moveServo(BAS_SERVO, Base+0, Base+83);
moveServo(SHL_SERVO, Shoulder+372, Shoulder+161);
moveServo(ELB_SERVO, Elbow-29, Elbow-519);
moveServo(WRI_SERVO, Wrist+700, Wrist+681);
delay(1200);

moveServo(BAS_SERVO, Base+83, Base+70);
moveServo(SHL_SERVO, Shoulder+161, Shoulder+139);
moveServo(ELB_SERVO, Elbow-519, Elbow-581);
moveServo(WRI_SERVO, Wrist+681, Wrist+720);
delay(d);
moveServo(BAS_SERVO, Base+70, Base+47);
moveServo(SHL_SERVO, Shoulder+139, Shoulder+124);
moveServo(ELB_SERVO, Elbow-581, Elbow-627);
moveServo(WRI_SERVO, Wrist+720, Wrist+751);
delay(d);
moveServo(BAS_SERVO, Base+47, Base+17);
moveServo(SHL_SERVO, Shoulder+124, Shoulder+117);
moveServo(ELB_SERVO, Elbow-627, Elbow-651);
moveServo(WRI_SERVO, Wrist+751, Wrist+768);
delay(d);
moveServo(BAS_SERVO, Base+17, Base-17);
moveServo(SHL_SERVO, Shoulder+117, Shoulder+117);
moveServo(ELB_SERVO, Elbow-651, Elbow-651);
moveServo(WRI_SERVO, Wrist+768, Wrist+768);
delay(d);
moveServo(BAS_SERVO, Base-17, Base-47);
moveServo(SHL_SERVO, Shoulder+117, Shoulder+124);
moveServo(ELB_SERVO, Elbow-651, Elbow-627);
moveServo(WRI_SERVO, Wrist+768, Wrist+751);
delay(d);
moveServo(BAS_SERVO, Base-47, Base-70);
moveServo(SHL_SERVO, Shoulder+124, Shoulder+139);
moveServo(ELB_SERVO, Elbow-627, Elbow-581);
moveServo(WRI_SERVO, Wrist+751, Wrist+720);
delay(d);
moveServo(BAS_SERVO, Base-70, Base-83);
moveServo(SHL_SERVO, Shoulder+139, Shoulder+161);
moveServo(ELB_SERVO, Elbow-581, Elbow-519);
moveServo(WRI_SERVO, Wrist+720, Wrist+681);
delay(d);


delay(12000);



  moveServo(GRI_SERVO, Gripper+777, Gripper);




}



void loop()
{

}

void moveServo(Servo servo, int initialPosition, int finalPosition)
  {
    for(int i = initialPosition; i<finalPosition; i = i+1)
    {
      servo.writeMicroseconds(i);
      delay(DELAY_TIME);
    }

    for(int i = initialPosition; i>finalPosition; i = i-1)
    {
      servo.writeMicroseconds(i);
      delay(DELAY_TIME);
    }
  }

void set_servo()
{
  delay(1000);
  BAS_SERVO.writeMicroseconds(Base);
  delay(1000);
  WRI_SERVO.writeMicroseconds(Wrist);
  delay(1000);
  SHL_SERVO.writeMicroseconds(Shoulder);
  delay(1000);
  ELB_SERVO.writeMicroseconds(Elbow);
  delay(1000);
  GRI_SERVO.writeMicroseconds(Gripper);
  delay(1000);
}

