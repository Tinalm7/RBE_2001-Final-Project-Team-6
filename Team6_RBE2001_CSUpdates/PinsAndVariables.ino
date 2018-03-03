//timers to be used in case machine
unsigned long stepTimer1=0;
unsigned long stepTimer2=0;
unsigned long stepTimer3=0;
//digital pins for button, limit switches and LED, analog pin for pot
int allignSwitch=22;
int button=2;
int RadLED=26;
int armPotPin;
//servo pins for motors
int gripMotorPin=11;
int slideMotorPin=10;
int leftDriveBack=9;
int leftDriveFor=8;
int rightDriveBack=7;
int rightDriveFor=6;
int armMotorLower=5;
int armMotorRaise=4;
//mesage variable and timers for messages
Messages msg;
unsigned long timeForHeartbeat;
unsigned long timeForRadAlert;
//variables used in main program
bool finished=false;
bool stageFinished=false;
int stopped=false;
int currentRod=0;
int spentRod=1;
int line=4;
int bitCalc;
//line follower proportion variables 
int linekp=1;
int linePos;
//arm proportion variables 
int currentValue;
int error;
int kp=1;
int armRaisedValue;
int armLoweredValue;
