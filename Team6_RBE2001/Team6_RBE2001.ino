
#include "Arduino.h"
#include "Messages.h"
#include <Servo.h>
#include <QTRSensors.h>
const int motorPin=11;
Servo Motor;
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

void goingRod(){
  switch goingRodStage{
    case 1:
      if(){
        //lineTracker
        }
      else if(){
        //stop drive motor
        //armLower
      }
      else if (){
        //stop arm motor 
        //trackExtend
      }
      else if{
        //gripClose
      }
      else {
        goingRodStage=2; 
      }
      break;
    case 2:
      if(){
        //trackRecline
      }
      else if(){
        //armRaise 
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
        //turnLeft
      }
      else {
        stageFinished=true;
        goingRodStage=1;
      }
      break;
  }
} 

void depositingRod(){
  switch depositingRodStage{
    case 1:
  }
}

void gettingNewRod() {
  gettingNewRod(){
    
  }
}

void depositingNewRod(){
  depositingNewRod(){
    
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
  
  Motor.attach(motorPin, 1000, 2000);
  
}

/**
 * The loop method runs continuously, so call the Messages read method to check for input
 * on the bluetooth serial port and parse any complete messages. 
 */
void loop() {
 if (msg.read()) {
 msg.printMessage();}
  // four bar code
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
