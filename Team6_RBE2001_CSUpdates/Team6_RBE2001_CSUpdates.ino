
#include "Arduino.h"
#include "Messages.h"
#include <Servo.h>
#include <QTRSensors.h>


//declares servo's
Servo slideMotor;
Servo gripMotor;
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

 
/**
 * Initialize the messages class and the debug serial port
 */
void setup() {
  Serial.begin(9600);
  Serial.println("start");
  pinMode(leftDriveBack, OUTPUT);
  pinMode(leftDriveFor, OUTPUT);
  pinMode(rightDriveBack, OUTPUT);
  pinMode(rightDriveFor, OUTPUT);
  pinMode(armMotorLower, OUTPUT);
  pinMode(armMotorRaise, OUTPUT);
  slideMotor.attach(slideMotorPin, 1000, 2000);
  gripMotor.attach(gripMotorPin, 1000, 2000);
  pinMode(RadLED, OUTPUT);
  pinMode(armPotPin, INPUT);
  pinMode(button, INPUT);
  pinMode(allignSwitch, INPUT);
  attachInterrupt(digitalPinToInterrupt(button), buttonPress, FALLING);
}

/**
 * The loop method runs continuously, so call the Messages read method to check for input
 * on the bluetooth serial port and parse any complete messages. 
 */
void loop() {
 if (msg.read()) {
 msg.printMessage();}

  //checks to see if robot should be stopped
  if (msg.isStopped()||finished||stopped){     
    driveStop();
    armStop();
  }
  else{ 
   //state machine code the first three cases have the robot navigate and pickup a spent rod and then face the opposite direction.
   switch (reactorStage){
     case 1:
       if(!digitalRead(allignSwitch)){
         lineFollowing();
         armLower();
       }
       else{
         driveStop();
         gripOpen();
         reactorStage=2; 
         setTimers();
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
       else if((stepTimer3>millis())||(sensorValues[4]<500)||(sensorValues[3]<200)){
         turnRight();
       }
       else{
         driveStop();
         reactorStage=4;
         setTimers();
         if(spentRod==1){
          line=4;
         }
         else if(spentRod==2){
          line=1;
         }
       }
       break;
       //cases 4-7 have the robot navigate to and empty deposit holder and deposit the rod.
     case 4:
       if((sensorValues[5]<500)||(sensorValues[1]<500)){
        lineFollowing();
       }
       else{
        reactorStage=5;
        setTimers();
       }
       break;
     case 5:
       if(storage(line)){
        reactorStage=6;
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
      if((stepTimer1>millis)||(sensorValues[4]<500)||(sensorValues[3]<500)){
        if(spentRod==1){
        turnLeft();
        }
        else if(spentRod==2){
          turnRight();
        }
      }
      else if(!digitalRead(allignSwitch)){
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
         currentRod=0;
        }
       else if(stepTimer3>millis()){
        slideRecline();
       }
       else{
        reactorStage=8;
       }
       break;
       //cases 8-17 have the robot navigate to the new fuel and pickup a new rod
       case 8:
         if(stepTimer2>millis()){
                backwards();
         }
       else if((stepTimer3>millis())||(sensorValues[4]<500)||(sensorValues[3]<500)){
         turnRight();
         }
       else{
        driveStop();
        reactorStage=9;
       }
       break;
    case 9: 
      if((sensorValues[5]<500)||(sensorValues[1]<500)){
        lineFollowing();
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
         if((stepTimer1>millis())||(sensorValues[4]<500)||(sensorValues[3]<500)){
          turnRight();
         }
         else{
          reactorStage=11;
          line++;
         }
      }
      else if(supply(line+1)||supply(line+2)||supply(line+3)){
       if((stepTimer1>millis())||(sensorValues[4]<500)||(sensorValues[3]<500)){
        turnLeft();
       }
       else{
        reactorStage=13;
        line--;
       }
     }
     break;
    case 11:
      if(stepTimer1>millis()||(sensorValues[5]<500)||(sensorValues[1]<500)){
        lineFollowing();
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
      if(stepTimer1>millis()||(sensorValues[4]<500)||(sensorValues[3]<500)){
        turnLeft();
      }
      else{
      reactorStage=15;
      }
      break;
    case 13:
   if(stepTimer1>millis()||(sensorValues[5]<500)||(sensorValues[1]<500)){
        lineFollowing();
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
      if(stepTimer1>millis()||(sensorValues[4]<500)||(sensorValues[3]<500)){
        turnRight();
      }
      else{
      reactorStage=15;
      }
      break;
    case 15:
      if(!digitalRead(allignSwitch)){
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
       else if((stepTimer3>millis())||(sensorValues[4]<500)||(sensorValues[3]<500)){
         turnRight();
         }
       else{
        driveStop();
        reactorStage=18;
       }
       break;
       //case 18-24 have the robot navigate to the empty holder and deposit the new rod
     case 18:
       if((sensorValues[5]<500)||(sensorValues[1]<500)){
        lineFollowing();
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
      if(stepTimer1>millis()||(sensorValues[4]<500)||(sensorValues[3]<500)){
        turnLeft();
      }
      else{
        reactorStage=21;
      }
      break;
    case 20:
    if(stepTimer1>millis()||(sensorValues[4]<500)||(sensorValues[3]<500)){
        turnRight();
      }
      break;
    case 21:
      if(!digitalRead(allignSwitch)){
         lineFollowing();
         armLower();
       }
       else{
         driveStop();
         reactorStage=22; 
         setTimers();
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
       else if((stepTimer3>millis())||(sensorValues[4]<500)||(sensorValues[3]<500)){
         armStop();
         turnRight();
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
  //generates radiation alert message every 1.5 sec
  if ((millis() > timeForRadAlert) && (currentRod > 0)){
    msg.sendRadAlert(currentRod);
    timeForRadAlert = millis() + 1500;
    Serial.println("radiation alert");
  }
  //generates heartbeat message every second
  if (millis() > timeForHeartbeat) {
    timeForHeartbeat = millis() + 1000;
    msg.sendHeartbeat();
    Serial.println("heartbeat");}
} 
void buttonPress() {
//stopped=!stopped;
}

