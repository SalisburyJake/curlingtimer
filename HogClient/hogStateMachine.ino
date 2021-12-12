//DEFINES
#define SENS_TRIGGERED    1
#define SENS_NOTTRIGGERED 0
TimerState prevState = ARMED;
bool hogArmed = false;
bool sendUnarmed = false;

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
      //Serial.println("Not Armed");
      if(lightRead >= MIN_SENS_CUTOFF && hogArmed == false)
      {
        webSocket.sendTXT("HOG_ARMED");
        hogArmed = true;
      }      
      else if(lightRead < MIN_SENS_CUTOFF && hogArmed == true)
      { 
        webSocket.sendTXT("HOG_UNARMED");
        hogArmed = false;
      } 
      break;
      
    case ARMED:
      lightRead = readSensor();
      //Serial.println("Armed");
      if(lightRead >= MIN_SENS_CUTOFF && hogArmed == false)
      {
        webSocket.sendTXT("HOG_ARMED");
        hogArmed = true;
      }      
      else if(lightRead < MIN_SENS_CUTOFF && hogArmed == true)
      { 
        webSocket.sendTXT("HOG_UNARMED");
        hogArmed = false;
      } 
      break;
      
    case T_SET:
      //Serial.println("T SET");
      lightRead = readSensor();
      if(sendUnarmed == false)
      {
        webSocket.sendTXT("HOG_UNARMED");
        sendUnarmed = true;
      }
      if(lightRead <= MIN_SENS_CUTOFF)          
      {
        Serial.println("Hog Line has been crossed");
        //Send message back
        //sock_sendHogTriggerStatus(SENS_TRIGGERED);
          webSocket.sendTXT("TLINE_TIME");
          state = FINISHED;  //quick hack.  Remove later
      }
      break;
      
    case FINISHED:
      //Serial.println("FINISHED");
      sendUnarmed = false;
      hogArmed = false;
      break;
      
    case FAULT:
      //Serial.println("FAULT");
      break;      
  } 

  if(prevState != state)
    Serial.println(state);
    
  prevState = state;
}
