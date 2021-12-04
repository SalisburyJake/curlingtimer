#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

#define LIGHT_SENSOR A0
#define MIN_SENS_CUTOFF 512

#define SOCKMSG_STATUS 0x01

typedef enum {
  NOT_ARMED,
  ARMED, 
  T_SET,
  FINISHED,
  FAULT  
} TimerState;

unsigned long t_curr = 0;
unsigned long t_TLine = 0;
unsigned long t_HogLine = 0;            
int lightRead = 0; 
TimerState state = NOT_ARMED;

bool tlineTime = false;

WebSocketsServer webSocket = WebSocketsServer(80);

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
      if(lightRead >= MIN_SENS_CUTOFF)          
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
        sock_sendState(T_SET);
        delay(200);
      }
      break;
      
    case T_SET:
      lightRead = readSensor();
      if(tlineTime)          
      {
        Serial.println("Hog Line has been crossed");
        t_HogLine = t_curr;
        state = FINISHED;
        sock_sendState(FINISHED);
        tlineTime = false;
      }
      break;
      
    case FINISHED:
      Serial.print("Total Time is: ");  Serial.print((((float) t_HogLine - (float) t_TLine)/1000)); Serial.println(" Seconds");
      state = NOT_ARMED;
      //delay(3000);
      sock_sendState(NOT_ARMED);
      break;
      
    case FAULT:
      break;      
  } 
}

/* 
 *  Function Name: readSensor
 * Description:    Read the sensor data
 * Inputs:         N/A
 * Outputs:        N/A
 */
int readSensor()
{
  int l_lightRead = 0;
  l_lightRead = analogRead(LIGHT_SENSOR);    //Read analog data from the sensor
  delay(50);
  return l_lightRead;
}

/* 
 *  Function Name: setup
 * Description:    SETUP
 * Inputs:         N/A
 * Outputs:        N/A
 */
void setup() {
  Serial.begin(115200);
  connectToWiFi();
} 

/* 
 *  Function Name: loop
 * Description:    LOOP
 * Inputs:         N/A
 * Outputs:        N/A
 */
void loop() {  
  webSocket.loop();
  t_curr = millis();
  main_sm();
  
}
