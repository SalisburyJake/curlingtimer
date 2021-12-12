//DEFINES
#define SENS_TRIGGERED    1
#define SENS_NOTTRIGGERED 0


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
      //Serial.println("Not Armed");
      break;
      
    case ARMED:
      //Serial.println("Armed");
      break;
      
    case T_SET:
      //Serial.println("T SET");
      lightRead = readSensor();
      if(lightRead <= MIN_SENS_CUTOFF)          
      {
        Serial.println("Hog Line has been crossed");
        //Send message back
        sock_sendHogTriggerStatus(SENS_TRIGGERED);
      }
      break;
      
    case FINISHED:
      //Serial.println("FINISHED");
      break;
      
    case FAULT:
      //Serial.println("FAULT");
      break;      
  } 
}
