//this function should have robot follow a line
void lineFollowing(){
  qtra.read(sensorValues);
  linePos=sensorValues[3]-(sensorValues[4]+320);
  analogWrite(leftDriveBack,0);
  analogWrite(leftDriveFor,150-(linePos*linekp));
  analogWrite(rightDriveBack,0);
  analogWrite(rightDriveFor,150+(linePos*linekp));
}
//this function should have robot turn left
void turnLeft(){
  qtra.read(sensorValues);
  analogWrite(leftDriveBack,120);
  analogWrite(leftDriveFor,0);
  analogWrite(rightDriveBack,0);
  analogWrite(rightDriveFor,120);
}
//this function should have robot turn right
void turnRight(){
  qtra.read(sensorValues);
  analogWrite(leftDriveBack,0);
  analogWrite(leftDriveFor,120);
  analogWrite(rightDriveBack,120);
  analogWrite(rightDriveFor,0);
}
//this function has the robot drive backwards
void backwards(){
  qtra.read(sensorValues);
  linePos=sensorValues[3]-(sensorValues[4]+320);
  analogWrite(leftDriveBack,150+(linePos*linekp));
  analogWrite(leftDriveFor,0);
  analogWrite(rightDriveBack,150-(linePos*linekp));
  analogWrite(rightDriveFor,0);
}
//this function should stop the drive motors
void driveStop(){
  analogWrite(leftDriveBack,0);
  analogWrite(leftDriveFor,0);
  analogWrite(rightDriveBack,0);
  analogWrite(rightDriveFor,0);
}
//raises arm to horizontal position using PID
void armRaise(){
  currentValue=analogRead(armPotPin);
  error=armRaisedValue-currentValue;
  if(error>0){
  analogWrite(armMotorRaise, (error*kp));
  analogWrite(armMotorLower, 0);
  }
  else if(error<0){
  analogWrite(armMotorLower, (error*kp*-1));
  analogWrite(armMotorRaise, 0);
  }
}
//lowers arm to vertical position using PID
void armLower(){
  currentValue=analogRead(armPotPin);
  error=armLoweredValue-currentValue;
  if(error>0){
  analogWrite(armMotorRaise, (error*kp));
  analogWrite(armMotorLower, 0);
  }
  else if(error<0){
  analogWrite(armMotorLower, (error*kp*-1));
  analogWrite(armMotorRaise, 0);
  }
}
void armStop(){
  analogWrite(armMotorRaise, 0);
  analogWrite(armMotorLower, 0);
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
