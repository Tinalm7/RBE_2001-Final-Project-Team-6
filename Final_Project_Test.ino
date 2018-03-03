#include <Servo.h>
unsigned long stepTimer1=0;
unsigned long stepTimer2=0;
unsigned long stepTimer3=0;

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

void setup(){
  // put your setup code here, to run once:
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
armStop();
gripOpen();
delay(500);
slideMotor.write(180);
delay(500);
gripClose();
delay(500); 
slideMotor.write(0);
armRaise();
delay(10000);
armStop();
//while(millis()<stepTimer1+10000){
//  armRaise();
//}
armRaise();
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
Serial.println(analogRead(11));
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
//stopped=!stopped;
//Serial.println("pressed");
}

