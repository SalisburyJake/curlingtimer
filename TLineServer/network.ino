//VARIABLES
//const char *ssid = "famiLE";
//const char *password = "DATAD1999";
const char *ssid = "testtesttest";
const char *password = "Halo2maps";
//IPAddress local_IP(192, 168, 1, 70);
//IPAddress gateway(192, 168, 1, 254);
//IPAddress subnet(255, 255, 255, 0);

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
void connectToWifi() {
  Serial.println();
  
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
    setupServer();
  }

   Serial.print("MAC: ");
   Serial.println(WiFi.macAddress());
}

/* 
 * Function Name: sock_sendState
 * Description:    Send the State of the state machine
 * Inputs:         String:msg - String to be sent via txt
 * Outputs:        N/A
 */
void sock_sendState(TimerState state)
{
  byte payload[2] = {SOCKMSG_STATUS, state};
  webSocket.broadcastBIN(payload, sizeof(payload));
}

/* 
 * Function Name:  webSocketEvent
 * Description:    Send the State of the state machine
 * Inputs:         uint8_t num: the ID of the connected device where the event came from
 *                 WStype_t type: type of event that was received
 *                 uint8_t * payload: The actual data that was sent along with the event
 *                 size_t length: Size of payload that was sent
 * Outputs:        N/A
 */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      clients[num] = 0; 
      break;
    case WStype_CONNECTED: 
      {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      
      clients[num] = 1; 
      webSocket.sendTXT(num, "Connected");
      break; 
    case WStype_TEXT: 
    {
      String text = String((char *) &payload[0]);
      Serial.printf("[%u] get Text: %s\n", num, payload);
      if(text.equals("TLINE_TIME"))
      {
        tlineTimeTriggered = true;
      }
      else if(text.equals("HOG_ARMED"))
      {
        hogArmed = true;
      }
      else if(text.equals("HOG_UNARMED"))
      {
        hogArmed = false;
      }
    }
      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);
      break;
  }
}
