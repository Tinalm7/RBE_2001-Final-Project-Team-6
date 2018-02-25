
#include "Arduino.h"
#include "Messages.h"
#include <Servo.h>
#include <QTRSensors.h>

int gripMotorPin=11;
int slideMotorPin=10;
int armMotorPin=9;
int rightMotorPin=8;
int leftMotorPin=7;
int RadLED=6; 
int stopped=false;

unsigned long stepTimer1=0;
unsigned long stepTimer2=0;
unsigned long stepTimer3=0;
//analog pins for each line sensor 
int lineSensor1=0;
int lineSensor2=1;
int lineSensor3=2;
int lineSensor4=3;
int lineSensor5=4;
int lineSensor6=5;
int Button1=22;
int Button2=22;
int Button3=2;
int line=4;
int bitCalc;
Servo leftMotor;
Servo rightMotor;
Servo armMotor;
Servo slideMotor;
Servo gripMotor;

Messages msg;
unsigned long timeForHeartbeat;
unsigned long timeForRadAlert;

bool finished=false;
bool stageFinished=false;
int currentRod =0;
int spentRod=1;
// state machine variables 
enum reactorStage {} reactorStage;
#include <QTRSensors.h>
#define NUM_SENSORS             6  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per reading
#define EMITTER_PIN             QTR_NO_EMITTER_PIN  // no emitter pin

// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsAnalog qtra((unsigned char[]) {0, 1, 2, 3, 4, 5}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];
/**
 * This "starter" program reads and debug-prints messages from the field. It also sends
 * heartbeat messages every second letting the field know that the robot is still running.
 *
 * You can use this as the basis for your project, or copy the elements from here into your
 * project. If you do that, be sure that you include the Messages and BTComms classes (.cpp
 * and .h files) into your new project.
 */
//this function should set three timers
void setTimers(){
  stepTimer1=millis()+500;
  stepTimer2=millis()+1000;
  stepTimer3=millis()+1500;
}
//this function checks the deposit storage at the current line
bool storage(int currentLine){
  int bitCalc=msg.storageAvailability();
  if(currentLine==4){
    return (bitCalc<8);
  }
  else if(currentLine==3){
    return ((bitCalc<3)||(12>bitCalc>7));
  }
  else if(currentLine==2){
    return ((bitCalc<2)||(6>bitCalc>3)||(10>bitCalc>7)||(14>bitCalc>11));
  }
  else if(currentLine==1){
    return ((bitCalc==0)||(bitCalc==2)||(bitCalc==4)||(bitCalc==6)||(bitCalc==8)||(bitCalc==10)||(bitCalc==12)||(bitCalc==14));
  }
}
//this function checks the supply storage at the current line
bool supply(int currentLine){
  int bitCalc=msg.supplyAvailability();
  if(currentLine==4){
    return (bitCalc<8);
  }
  else if(currentLine==3){
    return ((bitCalc<3)||(12>bitCalc>7));
  }
  else if(currentLine==2){
    return ((bitCalc<2)||(6>bitCalc>3)||(10>bitCalc>7)||(14>bitCalc>11));
  }
  else if(currentLine==1){
    return ((bitCalc==0)||(bitCalc==2)||(bitCalc==4)||(bitCalc==6)||(bitCalc==8)||(bitCalc==10)||(bitCalc==12)||(bitCalc==14));
  }
  else{
    return false;
  }
}
//this function should follow a line
void lineFollowing(){

}
//this function should turn left until sensors read a line
void turnLeft(){
  
}
//this function should turn right until sensors read a line
void turnRight(){
  
}
void backwards(){
  
}
void driveStop(){
  
}
//raises arm to horizontal position
void armRaise(){
  
}
//lowers arm to vertical position
void armLower(){
  
}
void armStop(){
  
}
//extend linear slide 
void slideExtend(){
  slideMotor.write(0);
}
//recline linear slide
void slideRecline(){
  slideMotor.write(0);
}
//close gripper to pick up rod
void gripClose(){
  gripMotor.write(131);
  digitalWrite(RadLED, HIGH);
  }
//opens gripper to release rod
void gripOpen(){
  gripMotor.write(60);
  digitalWrite(RadLED, LOW);
  currentRod=0;
  }
 
/**
 * Initialize the messages class and the debug serial port
 */
void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  msg.setup();
  timeForHeartbeat = millis() + 1000;
  timeForRadAlert=millis() + 1500;

  rightMotor.attach(rightMotorPin, 1000, 2000);
  leftMotor.attach(leftMotorPin, 1000, 2000);
  armMotor.attach(armMotorPin, 1000, 2000);
  slideMotor.attach(slideMotorPin, 1000, 2000);
  gripMotor.attach(gripMotorPin, 1000, 2000);
  pinMode(RadLED, OUTPUT);
  pinMode(Button1, INPUT);
  pinMode(Button2, INPUT);
  pinMode(Button3, INPUT);
  attachInterrupt(0, buttonPress, FALLING);
}

/**
 * The loop method runs continuously, so call the Messages read method to check for input
 * on the bluetooth serial port and parse any complete messages. 
 */
void loop() {
 if (msg.read()) {
 msg.printMessage();}

  //sensor read code
  if (msg.isStopped()||finished||stopped){     
    driveStop();
    armStop();
  }
  else{ 
   //state machine code 
   switch (reactorStage){
     case 1:
       if(digitalRead(Button1)||digitalRead(Button2)){
         lineFollowing();
         armLower();
       }
       else{
         driveStop();
         gripOpen();
         reactorStage=2; 
         setTimers();
         qtra.read(sensorValues); 
       }
       break;
     case 2:
       if(stepTimer1>millis()){
         slideExtend();
       }
       else if(stepTimer2>millis()){
         gripClose();
         currentRod=1;
       }
       else if(stepTimer3>millis()){
         slideRecline();
       }
       else{
         reactorStage=3;
       }
       break;
     case 3:
       if(stepTimer2>millis()){ 
         armRaise();
         backwards();
       }
       else if((stepTimer3>millis())||(sensorValues[4]>500)||(sensorValues[3]>500)){
         armStop();
         turnRight();
         qtra.read(sensorValues);
       }
       else{
         driveStop();
         reactorStage=4;
         qtra.read(sensorValues);
         setTimers();
         if(spentRod==1){
          line=4;
         }
         else if(spentRod==2){
          line=1;
         }
       }
       break;
     case 4:
       if((sensorValues[6]>500)||(sensorValues[1]>500)){
        lineFollowing();
        qtra.read(sensorValues);
       }
       else{
        reactorStage=5;
        setTimers();
       }
       break;
     case 5:
       if(storage(line)){
        reactorStage=6;
        qtra.read(sensorValues);
        setTimers();
       }
       else if((stepTimer1-250)>millis()){
        lineFollowing();
       }
       else{
        if(spentRod==1){
          line--;
        }
        else{
          line++;
        }
        reactorStage=4;
       }
       break;
     case 6:
      if((stepTimer1>millis)||(sensorValues[4]>500)||(sensorValues[3]>500)){
        if(spentRod==1){
        turnLeft();
        }
        else if(spentRod==2){
          turnRight();
        }
        qtra.read(sensorValues);
      }
      else if(digitalRead(Button1)||digitalRead(Button2)){
         lineFollowing; 
       }
      else{
        driveStop();
        reactorStage=7;
        setTimers();
      }
      break;
     case 7:
       if(stepTimer1>millis()){
         slideExtend();
       }  
       else if(stepTimer2>millis()){
         gripOpen();
        }
       else if(stepTimer3>millis()){
        slideRecline();
       }
       else{
        reactorStage=8;
        qtra.read(sensorValues);
       }
       break;
       case 8:
         if(stepTimer2>millis()){
                backwards();
         }
       else if((stepTimer3>millis())||(sensorValues[4]>500)||(sensorValues[3]>500)){
         turnRight();
           qtra.read(sensorValues);
         }
       else{
        driveStop();
        reactorStage=9;
        qtra.read(sensorValues);
       }
       break;
    case 9: 
      if((sensorValues[6]>500)||(sensorValues[1]>500)){
        lineFollowing();
        qtra.read(sensorValues);
      }
      else{
        driveStop();
        reactorStage=10;
        setTimers();
      }
      break;
    case 10:
      if(supply(line)){
        reactorStage=15;
      }
      else if(supply(line-1)||supply(line-2)||supply(line-3)){
         if((stepTimer1>millis())||(sensorValues[4]>500)||(sensorValues[3]>500)){
          turnRight();
          qtra.read(sensorValues);
         }
         else{
          reactorStage=11;
          line++;
         }
      }
      else if(supply(line+1)||supply(line+2)||supply(line+3)){
       if((stepTimer1>millis())||(sensorValues[4]>500)||(sensorValues[3]>500)){
        turnLeft();
        qtra.read(sensorValues);
       }
       else{
        reactorStage=13;
        line--;
       }
     }
     break;
    case 11:
      if(stepTimer1>millis()||(sensorValues[6]>500)||(sensorValues[1]>500)){
        lineFollowing();
        qtra.read(sensorValues);
      }
      else if(supply(line)){
        reactorStage=12;
        setTimers();
      }
      else{
        setTimers(); 
        line++;
      }
      break;
    case 12:
      if(stepTimer1>millis()||(sensorValues[4]>500)||(sensorValues[3]>500)){
        turnLeft();
        qtra.read(sensorValues);
      }
      else{
      reactorStage=15;
      }
      break;
    case 13:
   if(stepTimer1>millis()||(sensorValues[6]>500)||(sensorValues[1]>500)){
        lineFollowing();
        qtra.read(sensorValues);
      }
      else if(supply(line)){
        reactorStage=14;
        setTimers();
      }
      else{
        setTimers(); 
        line--;
      }
      break;
    case 14:
      if(stepTimer1>millis()||(sensorValues[4]>500)||(sensorValues[3]>500)){
        turnRight();
        qtra.read(sensorValues);
      }
      else{
      reactorStage=15;
      }
      break;
    case 15:
      if(digitalRead(Button1)||digitalRead(Button2)){
        lineFollowing();
      }
      else{
        reactorStage=16;
        setTimers();
      }
      break;
    case 16:
      if(stepTimer1>millis()){
        slideExtend(); 
      }
      else if(stepTimer2>millis()){
        currentRod=2;
        gripClose();
      }
      else if(stepTimer3>millis()){
        slideRecline();
      }
      else{
        reactorStage=17;
        setTimers();
      }
     case 17:
       if(stepTimer2>millis()){
                backwards();
         }
       else if((stepTimer3>millis())||(sensorValues[4]>500)||(sensorValues[3]>500)){
         turnRight();
           qtra.read(sensorValues);
         }
       else{
        driveStop();
        reactorStage=18;
       }
       break;
     case 18:
       if((sensorValues[6]>500)||(sensorValues[1]>500)){
        lineFollowing();
        qtra.read(sensorValues);
       }
       else if(spentRod==1){
        reactorStage=19;
        setTimers();
       }
       else if(spentRod==2){
        reactorStage=20;
        setTimers();
       }
      break;
    case 19:
      if(stepTimer1>millis()||(sensorValues[4]>500)||(sensorValues[3]>500)){
        turnLeft();
        qtra.read(sensorValues);
      }
      else{
        reactorStage=21;
      }
      break;
    case 20:
    if(stepTimer1>millis()||(sensorValues[4]>500)||(sensorValues[3]>500)){
        turnRight();
        qtra.read(sensorValues);
      }
      break;
    case 21:
      if(digitalRead(Button1)||digitalRead(Button2)){
         lineFollowing();
         armLower();
       }
       else{
         driveStop();
         reactorStage=22; 
         setTimers();
         qtra.read(sensorValues); 
       }
       break;
     case 22:
       if(stepTimer1>millis()){
         slideExtend();
       }
       else if(stepTimer2>millis()){
         gripOpen();
       }
       else if(stepTimer3>millis()){
         slideRecline();
       }
       else{
         reactorStage=23;
       }
       break;
     case 23:
       if(stepTimer2>millis()){ 
         armRaise();
         backwards();
       }
       else if((stepTimer3>millis())||(sensorValues[4]>500)||(sensorValues[3]>500)){
         armStop();
         turnRight();
         qtra.read(sensorValues);
       }
       else if(spentRod==1){
         driveStop();
         reactorStage=1;
         spentRod==2;
       }
       else if(spentRod==2){
        driveStop();
        finished=true;
       }
       break;
     }  
  }
  if ((millis() > timeForRadAlert) && (currentRod > 0)){
    msg.sendRadAlert(currentRod);
    timeForRadAlert = millis() + 1500;
    Serial.println("radiation alert");
  }
  if (millis() > timeForHeartbeat) {
    timeForHeartbeat = millis() + 1000;
    msg.sendHeartbeat();
    Serial.println("heartbeat");}
} 
void buttonPress() {
stopped=!stopped;
}

