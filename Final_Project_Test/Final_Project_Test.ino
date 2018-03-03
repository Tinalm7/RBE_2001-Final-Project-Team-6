#include <Servo.h>
#include <Encoder.h>
unsigned long stepTimer1=0;
unsigned long stepTimer2=0;
unsigned long stepTimer3=0;
//interrupt pins for button, digital for limit switches and LED, analog pin for pot
int allignSwitch=22;
int button=2;
int RadLED=26;
int armPotPin=11;
int slideEnc1=19;
int slideEnc2=20;
//servo pins for motors
int gripMotorPin=5;
int slideMotorPin=4;
int rightDriveBackPin=9;
int rightDriveForPin=8;
int leftDriveBackPin=6;
int leftDriveForPin=7;
int armMotorLowerPin=10;
int armMotorRaisePin=11;
bool finished=false;
bool stageFinished=false;
int stopped=false;
int currentRod=0;
int spentRod=1;
int line=4;
int bitCalc;
//line follower proportion variables 
int comp;
int linekp=1;
int linePos;
//timers for 4Bar & bool for 4bar
bool timerSet;
bool raiseArm;
int slideExtVal=-160;
int slideRecVal=0;
long fourBarTimer1;
long fourBarTimer2;
//declares servo's
Servo slideMotor;
Servo gripMotor;
Encoder slideEnc(slideEnc1, slideEnc2);
// state machine variable 
enum reactorStage {} reactorStage;
#include <QTRSensors.h>
#define NUM_SENSORS             6  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per reading
#define EMITTER_PIN             QTR_NO_EMITTER_PIN  // no emitter pin

// sensors 1 through 6 are connected to analog inputs 0 through 5, respectively
QTRSensorsAnalog qtra((unsigned char[]) {0,1,2,3,4,5}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];
void setTimers(){
  stepTimer1=millis()+500;
  stepTimer2=millis()+1000;
  stepTimer3=millis()+1500;
}
void lineFollowing(){
  qtra.read(sensorValues);
  linePos=sensorValues[3]-(sensorValues[4]+comp);
  analogWrite(leftDriveBackPin,0);
  analogWrite(leftDriveForPin,150-(linePos*linekp));
  analogWrite(rightDriveBackPin,0);
  analogWrite(rightDriveForPin,150+(linePos*linekp));
}
//this function should have robot turn left
void turnLeft(){
  qtra.read(sensorValues);
  analogWrite(leftDriveBackPin,120);
  analogWrite(leftDriveForPin,0);
  analogWrite(rightDriveBackPin,0);
  analogWrite(rightDriveForPin,120);
}
//this function should have robot turn right
void turnRight(){
  qtra.read(sensorValues);
  analogWrite(leftDriveBackPin,0);
  analogWrite(leftDriveForPin,100);
  analogWrite(rightDriveBackPin,60);
  analogWrite(rightDriveForPin,0);
}
//this function has the robot drive backwards
void backwards(){
  qtra.read(sensorValues);
  linePos=sensorValues[3]-(sensorValues[4]+comp);
  analogWrite(leftDriveBackPin,120+(linePos*linekp));
  analogWrite(leftDriveForPin,0);
  analogWrite(rightDriveBackPin,120-(linePos*linekp));
  analogWrite(rightDriveForPin,0);
}
//this function should stop the drive motors
void driveStop(){
  analogWrite(leftDriveBackPin,0);
  analogWrite(leftDriveForPin,0);
  analogWrite(rightDriveBackPin,0);
  analogWrite(rightDriveForPin,0);
}
void set4BarTimers(){
 fourBarTimer1=millis()+1000;
 fourBarTimer2=millis()+150;
}
//raises arm to horizontal position using PID
void armRaise(){
  if(digitalRead(armPotPin)>140){
  analogWrite(armMotorRaisePin, 150);
  analogWrite(armMotorLowerPin, 0);
  }
  else{
  analogWrite(armMotorRaisePin, 0);
  analogWrite(armMotorLowerPin, 0);  
  }
}
//lowers arm to vertical position using PID
void armLower(){
  if(210>digitalRead(armPotPin)){
  analogWrite(armMotorRaisePin, 0);
  analogWrite(armMotorLowerPin, 150);
  }
  else{
  analogWrite(armMotorRaisePin, 0);
  analogWrite(armMotorLowerPin, 0); 
  }
}
//stops arm motor
void armStop(){
  analogWrite(armMotorRaisePin, 0);
  analogWrite(armMotorLowerPin, 0);
  slideMotor.write(90);
}
//extend linear slide 
void slideExtend(){
  if(slideEnc.read()>slideExtVal){
  slideMotor.write(105);
  }
  else{
    slideMotor.write(90);
  }
}
//recline linear slide
void slideRecline(){
  if(slideEnc.read()<slideRecVal){
  slideMotor.write(64);
  }
  else{
    slideMotor.write(90);
  }
}
//close gripper to pick up rod
void gripClose(){
  gripMotor.write(160);
  digitalWrite(RadLED, HIGH);
  }
//opens gripper to release rod
void gripOpen(){
  gripMotor.write(60);
  digitalWrite(RadLED, LOW);
  currentRod=0;
}
void setup(){
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("start");
  pinMode(leftDriveBackPin, OUTPUT);
  pinMode(leftDriveForPin, OUTPUT);
  pinMode(rightDriveBackPin, OUTPUT);
  pinMode(rightDriveForPin, OUTPUT);
  pinMode(armMotorLowerPin, OUTPUT);
  pinMode(armMotorRaisePin, OUTPUT);
  slideMotor.attach(slideMotorPin, 1000, 2000);
  gripMotor.attach(gripMotorPin, 1000, 2000);
  pinMode(RadLED, OUTPUT);
  pinMode(armPotPin, INPUT);
  pinMode(button, INPUT);
  pinMode(allignSwitch, INPUT);
  pinMode(29,INPUT);
  attachInterrupt(digitalPinToInterrupt(button), buttonPress, RISING);
  qtra.read(sensorValues);
  comp=sensorValues[3]-sensorValues[4];
  setTimers();
}
void loop() {
  // put your main code here, to run repeatedly:
qtra.read(sensorValues);
//if((sensorValues[1]<500||sensorValues[3]<500||sensorValues[5]<500)&&(!digitalRead(allignSwitch))){
//lineFollowing();
//setTimers();
//}
//else{
//  driveStop();
//}
//armStop();
//gripOpen();
//delay(500);
//slideMotor.write(180);
//delay(500);
//gripClose();
//delay(500); 
//slideMotor.write(0);
//armRaise();
//delay(10000);
//armStop();
////while(millis()<stepTimer1+10000){
////  armRaise();
////}
//armRaise();
//else if((stepTimer1-250)>millis()){
//  lineFollowing();
//  stopFollowing=1;
//}
//else if(stepTimer2>millis()||sensorValues[1]<600||sensorValues[3]<600||sensorValues[5]<600){
  //turnRight();
//if(stopped==0){
//  setTimers();
//  stopped=true;
//}
armStop();
if(digitalRead(29)==1){
armStop();
slideMotor.write(90);
setTimers();
//while(millis()<stepTimer3){
//slideExtend();
//}
//setTimers();
//while(millis()<stepTimer3){
//slideRecline(); 
//}
//delay(10000);
//slideMotor.write(65);
Serial.println(slideEnc.read());
}
//Serial.print(sensorValues[1]);
//Serial.print(":");
//Serial.print(sensorValues[2]);
//Serial.print(":");
//Serial.print(sensorValues[3]);
//Serial.print(":");
//Serial.print(sensorValues[4]);
//Serial.print(":");
//Serial.print(sensorValues[5]);
//Serial.print(":");
//Serial.println(sensorValues[6]);
}
void buttonPress(){
//if(stopped==1){
//  stopped=0;
//}
//else{
//stopped=1;
//}
//Serial.println("pressed");
}

