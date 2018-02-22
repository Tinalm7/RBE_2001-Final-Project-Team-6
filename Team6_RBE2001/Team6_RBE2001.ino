
#include "Arduino.h"
#include "Messages.h"
#include <Servo.h>
#include <QTRSensors.h>

int gripMotorPin=11;
int trackMotorPin=10;
int armMotorPin=9;
int rightMotorPin=8;
int leftMotorPin=7
Servo leftMotor;
Servo rightMotor;
Servo armMotor;
Servo trackMotor;
Servo gripMotor;

Messages msg;
unsigned long timeForHeartbeat;
unsigned long timeForRadAlert;

bool finished=false;
bool stageFinished=false;
int currentRod =0;
int spentRod=1;
// state machine variables 
enum reactorStage {kGoingRod,kDepositingRod,kGettingNewRod,kDepositingNewRod} reactorStage;
enum goingRodStage {} goingRodStage;
enum depositingRodStage {} depositingRodStage;
enum gettingNewRodStage {} gettingNewRodStage;
enum depositingNewRodStage {} depositingNewRodStage;
#define NUM_SENSORS             6  // number of sensors used
#define TIMEOUT  4  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN             2  // emitter is controlled by digital pin 2

// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 5, 6, 7, 8, 9, 10},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
/**
 * This "starter" program reads and debug-prints messages from the field. It also sends
 * heartbeat messages every second letting the field know that the robot is still running.
 *
 * You can use this as the basis for your project, or copy the elements from here into your
 * project. If you do that, be sure that you include the Messages and BTComms classes (.cpp
 * and .h files) into your new project.
 */
//this function should follow a line
void lineFollower(){
  
}
//this function should turn left until sensors read a line
void turnLeft(){
  
}
//this function should turn right until sensors read a line
void turnRight(){
  
}
//raises arm to horizontal position
void armRaise(){
  
}
//lowers arm to vertical position
void armLower(){
  
}
//extend linear slide 
void slideExtend(){
}
//recline linear slide
void slideRecline(){
}
//close gripper to pick up rod
void gripClose(){
  gripMotor.write(131);
  currentRod=SpentRod;
  }
//opens gripper to release rod
void gripOpen(){
  gripMotor.write(60);
  currentRod=0;
  }

void goingRod(){
  switch (goingRodStage){
    case 1:
      if(){
        lineFollower();
        }
      else if(){
        //stop drive motor
        armLower();
      }
      else
        //stop arm motor 
        slideExtend();
        gripClose();
        goingRodStage=2; 
      }
      break;
    case 2:
      if(){
        slideRecline();
        armRaise(); 
      } 
      else{
        goingRodStage=3;
      }
      break;
    case 3:
      if(){
        //drive backward
      }
      else if(){
        turnLeft();
      }
      else {
        stageFinished=true;
        goingRodStage=1;
        if(spentRod=1){
          line=5;
        }
        if(spentRod=2){
          line=0;
        }
      }
      break;
  }
} 

void depositingRod(){
  switch (depositingRodStage){
    case 1:
    if(){
      lineFollower();
    }
    else { 
      if(spentRod==2){
        line++;      
      }
      else if(spentRod==1){
        line--;
      }
    depositRodStage=2;
    }
    break;
    case 2:
      if(storageLineCheck(line)){
        depositingRodStage=3;     
      }
    else{
      //driveForward
      depositingRodStage=1;
    }
    break;
    case 3:
      turnLeft();
      break;
    case 4:
      if(){
      lineFollower();
      } 
      else if(){
        slideExtend();
        }
      else if(){
        gripOpen();
        }
        else {
          depositingRodStage=5;
          )
      break;
    case 5;
      if(){
        //drive backwards
      }
      else if(){
        turnLeft();
      }
      else {
        depositingRodStage=1;
        stageFinished=true;
      }
      break;
  }
}

void gettingNewRod() {
  switch (gettingNewRodStage){
    case 1:
    if(){
      lineFollower(); 
    }
    else{
      gettingNewRodStage=2;
    }
    break;
    case 2: 
    //supplyCheck
    case 3: 
    //turnleft 
    //turnright
    case 4:
    if(){
      lineFollower();
    }
    else if(){
      trackExtend();
    }
    else if(){
      gripClose();
    }
    else{
  
    }
    case 5:
    if(){
      trackRecline(); 
    }
    else if(){
      driveBackwards(); 
    }
    else if{
      turnLeft(); 
    }
    else{
      stageFinished=true;
      gettingNewRodStage=1;
    }
  }
}

void depositingNewRod(){
  switch (depositingNewRodStage){
    case 1:
    if(){
      lineFollower();
    }
    else{
      repositingNewRodStage=2;
    }
    break;
    case 2: 
    if(spentRod=1){
      turnLeft();
    }
    else if(spentRod=2){
      turnRight();
    }
    break;
    case 3:
    if(){
      lineFollower(); 
    }
    else if(){
    armLower;
    }
    else if(){
      slideExtend();
    }
    else(){
    gripOpen();
    }
    case 4:
    if(){
      trackRecline(); 
    }
    else if(){
      armRaise();
    }
    case 5:
    if(){
    //driveBackwards
    } 
    else if(){turnLeft
    }
    else{
      stageFinished=true;
      depositingNewRodStage=1;
    }
  }
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
  trackMotor.attach(trackMotorPin, 1000, 2000);
  gripMotor.attach(gripMotorPin, 1000, 2000);
}

/**
 * The loop method runs continuously, so call the Messages read method to check for input
 * on the bluetooth serial port and parse any complete messages. 
 */
void loop() {
 if (msg.read()) {
 msg.printMessage();}

  //sensor read code
  if (msg.isStopped() || finished){     
    //stop motors
  }
  else{
   switch (reactorStage){
    case kGoingRod: 
      if (!stageFinished){
         goingRod();
      }
      else{ 
        stageFinished=false;
        reactorStage=kDepositingRod;
   }
    break;
    case kDepositingRod: 
            if (!stageFinished){
         depositingRod();
      }
      else{ 
        stageFinished=false;
        reactorStage=kGettingNewRod;
      }
      break;
    case kGettingNewRod: 
           if (!stageFinished){
         gettingNewRod();
      }
      else{ 
        stageFinished=false;
        reactorStage=kDepositingNewRod;
      }
      break;
    case kDepositingNewRod: 
            if (!stageFinished){
         depositingNewRod();
      }
      else if(spentRod=1){ 
        stageFinished=false;
        spentRod=2;
        reactorStage=kGoingRod;
      }
      else{
        //code to stop all motors
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
