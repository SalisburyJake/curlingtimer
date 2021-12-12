//VARIABLES
unsigned long t_TLine = 0;
unsigned long t_HogLine = 0; 
TimerState prevState = ARMED;

/* 
 *  Function Name: main_sm
 * Description:    The main State Machine
 * Inputs:         N/A
 * Outputs:        N/A
 */
void main_sm()
{    
  switch(state)
  {
    case NOT_ARMED:
      lightRead = readSensor();
      if((lightRead >= MIN_SENS_CUTOFF) && hogArmed == true)          
      {
        Serial.println("All Sensors are functional.  Arming....");
        state = ARMED;
        sock_sendState(ARMED);
      }
      break;
      
    case ARMED:
      lightRead = readSensor();
      //If light sensor has been broken, then set the T-Time accordingly
      if(lightRead <= MIN_SENS_CUTOFF)          
      {
        Serial.println("T line has been crossed");
        t_TLine = t_curr;
        state = T_SET;
        displayWriteSet();
        sock_sendState(T_SET);
        delay(200);
      }
      break;
      
    case T_SET:
      lightRead = readSensor();
      //if(lightRead <= MIN_SENS_CUTOFF)  //Set this instead of the one above to bypass the second laser/sensor          
      if(tlineTimeTriggered)
      {
        Serial.println("Hog Line has been crossed");
        t_HogLine = t_curr;
        state = FINISHED;
        sock_sendState(FINISHED);
        tlineTimeTriggered = false;
      }
      break;
      
    case FINISHED:
      Serial.print("Total Time is: ");  Serial.print((((float) t_HogLine - (float) t_TLine)/1000)); Serial.println(" Seconds");
      display_WriteFinalTime((((float) t_HogLine - (float) t_TLine)/1000));
      state = NOT_ARMED;
      sock_sendState(NOT_ARMED);
      break;
      
    case FAULT:
      break;      
  } 
  
  if(prevState != state)
    Serial.println(state);
    
  prevState = state;
}
