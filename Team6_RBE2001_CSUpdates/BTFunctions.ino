/**
 * This "starter" program reads and debug-prints messages from the field. It also sends
 * heartbeat messages every second letting the field know that the robot is still running.
 *
 * You can use this as the basis for your project, or copy the elements from here into your
 * project. If you do that, be sure that you include the Messages and BTComms classes (.cpp
 * and .h files) into your new project.
 */
//this function sets three timers
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
