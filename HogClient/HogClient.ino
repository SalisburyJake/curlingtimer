//INCLUDES
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

//DEFINES
#define LIGHT_SENSOR A0
#define MIN_SENS_CUTOFF 512

//TYPEDEF
typedef enum {
  NOT_ARMED,
  ARMED, 
  T_SET,
  FINISHED,
  FAULT  
} TimerState;

TimerState state = NOT_ARMED;

WebSocketsClient webSocket; //Initialize the cloent variable

int lightRead = 0;  //Variable to store the analog input

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  connectToWifi();
}


void loop()
{
  webSocket.loop();
  //main_sm();
}
