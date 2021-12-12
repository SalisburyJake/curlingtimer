//DEFINES
#define SOCKMSG_STATUS 0x01
#define SOCKMSG_HOGTRIGGER 0x02

//VARIABLES
//const char* ssid = "famiLE";
//const char* password = "DATAD1999";
const char *ssid = "testtesttest";
const char *password = "Halo2maps";
const char* host = "192.168.237.151";
//IPAddress local_IP(192, 168, 1, 71);
//IPAddress gateway(192, 168, 1, 254);
//IPAddress subnet(255, 255, 255, 0);

/* 
 *  Function Name: setupClient
 * Description:    Setup the client
 * Inputs:         N/A
 * Outputs:        N/A
 */
void setupClient()
{
  webSocket.begin(host, 81,"/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
  // start heartbeat (optional)
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  webSocket.enableHeartbeat(15000, 3000, 2);
}

/* 
 *  Function Name: connectToWiFi
 * Description:    Connects to the wifi network
 * Inputs:         N/A
 * Outputs:        N/A
 */
void connectToWifi() {
  
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Configures static IP address
  //if (!WiFi.config(local_IP, gateway, subnet)) {
  //  Serial.println("STA Failed to configure");
  //}

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
    setupClient();
  }
}

/* 
 * Function Name:  sock_sendHogTriggerStatus
 * Description:    Send the trigger status of the hog line sensor
 * Inputs:         String:msg - String to be sent via txt
 * Outputs:        N/A
 */
void sock_sendHogTriggerStatus(int trigger)
{
  byte payload[2] = {SOCKMSG_HOGTRIGGER, trigger};
  webSocket.sendBIN(payload, sizeof(payload));
}

/* 
 * Function Name:  webSocketEvent
 * Description:    Send the State of the state machine
 * Inputs:         WStype_t type: type of event that was received
 *                 uint8_t * payload: The actual data that was sent along with the event
 *                 size_t length: Size of payload that was sent
 * Outputs:        N/A
 */
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
