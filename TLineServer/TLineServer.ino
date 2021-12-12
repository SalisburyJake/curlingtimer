//INCLUDES
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>

//DEFINES
#define LIGHT_SENSOR A0
#define MIN_SENS_CUTOFF 512
#define SOCKMSG_STATUS 0x01 //States that the status is in the socket message

//TYPEDEF
typedef enum {
  NOT_ARMED,
  ARMED, 
  T_SET,
  FINISHED,
  FAULT  
} TimerState;

//VARIABLES            
int lightRead = 0; 
bool tlineTimeTriggered = false;
bool hogArmed = false;
unsigned long t_curr = 0;
TimerState state = NOT_ARMED;
int clients[5] = {0,0,0,0,0};


WebSocketsServer webSocket = WebSocketsServer(81);

/* 
 *  Function Name: setup
 * Description:    SETUP
 * Inputs:         N/A
 * Outputs:        N/A
 */
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  displayInit();
  connectToWifi();
} 

/* 
 *  Function Name: loop
 * Description:    LOOP
 * Inputs:         N/A
 * Outputs:        N/A
 */
void loop() {
  t_curr = millis();  
  webSocket.loop();
  if(clients[0] == 1 || clients[1] == 1 || clients[2] == 1 || clients[3] == 1 || clients[4] == 1)
    main_sm();  
}
