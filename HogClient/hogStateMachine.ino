//DEFINES
#define SENS_TRIGGERED    1
#define SENS_NOTTRIGGERED 0
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
      Serial.println(lightRead);

      
      //Serial.println("Not Armed");
      break;
      
    case ARMED:
      lightRead = readSensor();
      Serial.println(lightRead);
      if(lightRead <= MIN_SENS_CUTOFF)
        webSocket.sendTXT("HOG_ARMED");
      else
        webSocket.sendTXT("HOG_UNARMED");
      //Serial.println("Armed");
      break;
      
    case T_SET:
      //Serial.println("T SET");
      lightRead = readSensor();
      Serial.println(lightRead);
      if(lightRead <= MIN_SENS_CUTOFF)          
      {
        Serial.println("Hog Line has been crossed");
        //Send message back
        //sock_sendHogTriggerStatus(SENS_TRIGGERED);
          webSocket.sendTXT("TLINE_TIME");
      }
      break;
      
    case FINISHED:
      //Serial.println("FINISHED");
      break;
      
    case FAULT:
      //Serial.println("FAULT");
      break;      
  } 

  if(prevState != state)
    Serial.println(state);
    
  prevState = state;
}
