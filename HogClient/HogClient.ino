#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

#define LIGHT_SENSOR A0
#define MIN_SENS_CUTOFF 512

//#define SOCKMSG_TLINE_TIME 0x01
#define SOCKMSG_STATUS 0x01

int lightRead = 0;

typedef enum {
  NOT_ARMED,
  ARMED, 
  T_SET,
  FINISHED,
  FAULT  
} TimerState;

TimerState state = NOT_ARMED;

const char* ssid = "famiLE";
const char* password = "DATAD1999";
const char* host = "192.168.1.137";

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED: {
      Serial.printf("[WSc] Connected to url: %s\n", payload);

      // send message to server when Connected
      webSocket.sendTXT("Connected");
    }
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      hexdump(payload, length);

      Serial.println(payload[0]);
      if(payload[0] == SOCKMSG_STATUS)
      {
        state = (TimerState) payload[1];
      }
      
      // send data to server
      // webSocket.sendBIN(payload, length);
      break;
    case WStype_PING:
        // pong will be send automatically
        Serial.printf("[WSc] get ping\n");
        break;
    case WStype_PONG:
        // answer to a ping we send
        Serial.printf("[WSc] get pong\n");
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
}


void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wifi");


  webSocket.begin("192.168.1.137", 80, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
  // start heartbeat (optional)
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  // webSocket.enableHeartbeat(15000, 3000, 2);
}


void loop()
{
  webSocket.loop();
  main_sm();
}
