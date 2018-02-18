/*
 * Messages.cpp
 *
 *  Created on: Sep 15, 2016
 *      Author: bradmiller
 */
#include "Arduino.h"
#include "Messages.h"
#include "BTComms.h"

BTComms comms;

/**
 * Constuctor
 * Initialize everything here when the class is created
 * Note: you cannot call methods that depend on other classes having already been created
 */
Messages::Messages() {
	stopped = false;
}

/**
 * Setup class code that is called from the Arduino sketch setup() function. This doesn't
 * get called until all the other classes have been created.
 */
void Messages::setup() {
	comms.setup();
}

/**
 * Check if the field is currently in the "stop" state
 * @returns bool value that is true if the robot should be stopped
 */
bool Messages::isStopped() {
	return stopped;
}
int Messages::supplyAvailability(){
  return supply;
}
int Messages::storageAvailability(){
  return storage;
}
/**
 * Send a heartbeat message to the field to let it know that your code is alive
 * This should be called by your robot program periodically, say once per second. This
 * timing can easily be done in the loop() function of your program.
 */
void Messages::sendHeartbeat() {
	comms.writeMessage(kHeartbeat, 0x06, 0x00);
} 
/** 
 *  Send a radiation alert to the reactor control when carrying a rod
 *  This should be called every second 
 *  if rod = 1, send radiation alert for spent rod
 *  if rod = 2, send radiation alert for new rod
 */
void Messages::sendRadAlert(int rod){
  if(rod==1){
    comms.writeMessage(kRadiationAlert, 0x06, 0x00, 0x2C);}
  else if(rod==2){
    comms.writeMessage(kRadiationAlert, 0x06, 0x00, 0xFF);
  }
}
/**
 * Print message for debugging
 * This method prints the message as a string of hex numbers strictly for debugging
 * purposes and is not required to be called for any other purpose.
 */
void Messages::printMessage() {
    for (int i = 0; i < comms.getMessageLength(); i++) {
      Serial.print(comms.getMessageByte(i), HEX);
      Serial.print(" ");
    }
    Serial.println();
}

/**
 * Read messages from the Bluetooth serial connection
 * This method should be called from the loop() function in your arduino code. It will check
 * to see if the lower level comms object has received a complete message, and run the appropriate
 * code to handle the message type. This should just save the state of the message inside this class
 * inside member variables. Then add getters/setters to retrieve the status from your program.
 */
bool Messages::read() {
	if (comms.read()) {
		switch (comms.getMessageByte(0)) {
		case kStorageAvailability: 
      storage=comms.getMessageByte(availabilityData);
			break;
		case kSupplyAvailability:
			supply=comms.getMessageByte(availabilityData); 
			break;
		case kRadiationAlert:
			break;
		case kStopMovement:
      stopped=true;
 			break;
		case kResumeMovement:
      stopped=false;
			break;
		case kRobotStatus:
			break;
		case kHeartbeat:
			break;
		}
		return true;
	}
	return false;
}
