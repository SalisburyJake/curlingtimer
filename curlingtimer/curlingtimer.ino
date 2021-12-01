#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

#define lightSensor A0
#define minSensCutoff 512

typedef enum {
  NOT_ARMED,
  ARMED, 
  T_SET,
  FINISHED,
  FAULT  
} timerState;

unsigned long t_curr = 0;
unsigned long t_TLine = 0;
unsigned long t_HogLine = 0;            
int lightRead = 0; 
timerState state = NOT_ARMED;

WebSocketsServer webSocket = WebSocketsServer(80);

const char *ssid = "famiLE";
const char *password = "DATAD1999";


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
              // send message to client
              //webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);

            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            webSocket.sendTXT(num, "Fuck you");
            break;
        case WStype_BIN:
            Serial.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }
}

/* 
 *  Function Name: setupServer
 * Description:    Setup the asynchronous server
 * Inputs:         N/A
 * Outputs:        N/A
 */
void setupServer()
{
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

/* 
 *  Function Name: connectToWiFi
 * Description:    Connects to the wifi network
 * Inputs:         N/A
 * Outputs:        N/A
 */
void connectToWiFi() {
  Serial.println();
  
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP

  if(WiFi.status() == WL_CONNECTED) //If WiFi connected to hot spot then start mDNS
  {
    setupServer();
  }
}

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
      if(lightRead >= minSensCutoff)          
      {
        Serial.println("All Sensors are functional.  Arming....");
        state = ARMED;
      }
      break;
      
    case ARMED:
      lightRead = readSensor();
      //If light sensor has been broken, then set the T-Time accordingly
      if(lightRead <= minSensCutoff)          
      {
        Serial.println("T line has been crossed");
        t_TLine = t_curr;
        state = T_SET;
        delay(200);
      }
      break;
      
    case T_SET:
      lightRead = readSensor();
      if(lightRead <= minSensCutoff)          
      {
        Serial.println("Hog Line has been crossed");
        t_HogLine = t_curr;
        state = FINISHED;
      }
      break;
      
    case FINISHED:
      Serial.print("Total Time is: ");  Serial.print((((float) t_HogLine - (float) t_TLine)/1000)); Serial.println(" Seconds");
      state = NOT_ARMED;
      //delay(3000);
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
  l_lightRead = analogRead(lightSensor);    //Read analog data from the sensor
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
