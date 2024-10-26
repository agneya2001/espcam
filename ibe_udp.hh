#ifndef IBE_UDP
#define IBE_UDP

#define BROKER_UDP_PORT 5020 
#define BROKER_ADDRESS "broker.ibeyonde.com" 

int _localUdpPort=random(20000, 65535);

WiFiUDP Udp;

uint8_t _udpSend = 0;

int _udpRcvPort;
int _udpRcvIp[4];

int udpCombineBytes( unsigned char x_high, unsigned char x_low)
{
  int combined; 
  combined = x_high;              //send x_high to rightmost 8 bits
  combined = combined<<8;         //shift x_high over to leftmost 8 bits
  combined |= x_low;                 //logical OR keeps x_high intact in combined and fills in  //rightmost 8 bits
  return combined;
}

void udpSendEventHandler() {
  if (_udpSend>0){
    xSemaphoreTake( camMutex, portMAX_DELAY );
    
    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Udp Camera capture failed ");
    }
    else {
      if(fb->format == PIXFORMAT_JPEG){
          //Serial.printf("udp> %d.%d.%d.%d:%d  image<%d>\n", _udpRcvIp[0], _udpRcvIp[1], _udpRcvIp[2], _udpRcvIp[3], _udpRcvPort, fb->len);
          IPAddress rcvIp(_udpRcvIp[0], _udpRcvIp[1], _udpRcvIp[2], _udpRcvIp[3]);
          Udp.beginPacket(rcvIp, _udpRcvPort);
          char *msg = bufc;
          msg += sprintf(msg, "SIZE:%s.%d.",_deviceId, fb->len);
          *msg++ = 0;
          Udp.print(bufc);
          Udp.endPacket();
          
          Udp.beginPacket(rcvIp, _udpRcvPort);
          Udp.write(fb->buf, fb->len);
          Udp.flush();
          Udp.endPacket();
      } else {
          Serial.println("FATAL: Format is not JPEG");
      }
    }
    esp_camera_fb_return(fb);
    _udpSend--;
    
    delay(100);
    xSemaphoreGive( camMutex );
  }
}

bool udpInit(){

    char *msg = bufu;
    msg += sprintf(msg, "REGISTER:%s:", _deviceId);
    *msg++ = lowByte(_localIp[0]);
    *msg++ = lowByte(_localIp[1]);
    *msg++ = lowByte(_localIp[2]);
    *msg++ = lowByte(_localIp[3]);
    *msg++ = lowByte(_localUdpPort);
    *msg++ = highByte(_localUdpPort);
    *msg++ = 0;

    Serial.printf("udp>%s\n", bufu);
        
    Udp.beginPacket("broker.ibeyonde.com", BROKER_UDP_PORT);
    Udp.print(bufu);
    Udp.endPacket();

    int packetSize = Udp.parsePacket();
    int  j=0;
    while(packetSize == 0 && j < 10){
      packetSize = Udp.parsePacket();
      delay(100);
      j++;
    }
    
    if (packetSize == 0 ) return false;
    
    IPAddress remoteIp = Udp.remoteIP();
    
    // read the packet into packetBufffer
    packetSize = Udp.read(bufu, packetSize);
    if (packetSize > 0) bufu[packetSize] = 0;
    
    //Serial.printf("udp>%d.%d.%d.%d:%d  %s\n", remoteIp[0], remoteIp[1], remoteIp[2], remoteIp[3], Udp.remotePort(), bufu);
    
    int aI=-1;
    if (bufu[0] == 'R' && bufu[1] == 'R' && bufu[2] == 'E' && bufu[3] == 'G'){
      aI=5;
      Serial.println("Rcvd=RREG");
      
      _udpRcvIp[0] = bufu[aI];
      _udpRcvIp[1] = bufu[aI + 1];
      _udpRcvIp[2] = bufu[aI + 2];
      _udpRcvIp[3] = bufu[aI + 3];
      _udpRcvPort = udpCombineBytes(bufu[aI+5], bufu[aI+4]); 
      
      Udp.endPacket();
      return true;
    }
    else {
      Serial.println("Unknown command");
      Udp.endPacket();
      return false;
    }
}


void udpRefreshWithBroker(){
    Udp.beginPacket(BROKER_ADDRESS, BROKER_UDP_PORT);
    
    char *msg = bufc;
    msg += sprintf(msg, "REFRESH:%s:", _deviceId);
    *msg++ = lowByte(_localIp[0]);
    *msg++ = lowByte(_localIp[1]);
    *msg++ = lowByte(_localIp[2]);
    *msg++ = lowByte(_localIp[3]);
    *msg++ = lowByte(_localUdpPort);
    *msg++ = highByte(_localUdpPort);
    *msg++ = 0;

    //Serial.printf("udp>%s\n", bufc);
    Udp.print(bufc);
    Udp.endPacket();
    Udp.flush();
}


void udpListen(){
    Udp.flush();
    Udp.beginPacket("0.0.0.0", _localUdpPort);
    
    int packetSize = Udp.parsePacket();
  
    if (packetSize > 0 && packetSize < 128){
      
      IPAddress remoteIp = Udp.remoteIP();
      Udp.read(bufc, packetSize);
      bufc[packetSize] = 0;
      
      //Serial.printf("udp>%d:%d.%d.%d.%d  %s\n", remoteIp[0], remoteIp[1], remoteIp[2], remoteIp[3], Udp.remotePort(), bufc);
      
      int aI=-1;
      if (bufc[0] == 'R' && bufc[2] == 'I' && bufc[3] == 'N' && bufc[4] == 'I'){ //RHINI
          aI=6;
      }
      else if (bufc[0] == 'D' && bufc[2] == 'I' && bufc[3] == 'N' && bufc[4] == 'J'){ //DHINJ
          aI=6;
      }
      else if (bufc[0] == 'R' && bufc[1] == 'R' && bufc[2] == 'E' && bufc[3] == 'G'){ // RREG
          aI=5;
      }
      else if (bufc[0] == 'R' && bufc[1] == 'P' && bufc[2] == 'A' && bufc[3] == 'D' && bufc[4] == 'D' && bufc[5] == 'R'){ //RPADDR
          aI=7;
      }
      else {
          Serial.print("Unknown command");
      }
      
      
      if (aI != -1 ){
        if (bufc[4] == 'I' || bufc[4] == 'D'){
          _udpRcvIp[0] = bufc[aI];
          _udpRcvIp[1] = bufc[aI + 1];
          _udpRcvIp[2] = bufc[aI + 2];
          _udpRcvIp[3] = bufc[aI + 3];
          _udpRcvPort = udpCombineBytes(bufc[aI+5], bufc[aI+4]);
          _udpSend++;
        }
        else if ( bufc[0] == 'D'){
          _udpRcvIp[0] = remoteIp[0];
          _udpRcvIp[1] = remoteIp[1];
          _udpRcvIp[2] = remoteIp[2];
          _udpRcvIp[3] = remoteIp[3];
          _udpRcvPort = Udp.remotePort();
          _udpSend++;
        }
      }
    }//if (packetSize > 0 ){
    Udp.flush();
}//listenToUdp


#endif
