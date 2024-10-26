#ifndef IBE_HTTP
#define IBE_HTTP

////HTTP/////
#include <HTTPClient.h>
#include "esp_http_client.h"
#include "esp_https_ota.h"

#define pingURL "http://ping.ibeyonde.com/api/ping.php"
#define registerURL "http://ping.ibeyonde.com/api/dev_register.php"
#define unRegisterURL "http://ping.ibeyonde.com/api/dev_unregister.php"
#define morionURL "http://ping.ibeyonde.com/api/motionAlert.php"
#define tokenURL "http://ping.ibeyonde.com/api/token.php"
#define bellAlertURL "http://ping.ibeyonde.com/api/bellAlert.php"

////HTTP/////

#define _ca_cert ""

static char _token[11]={0};

static bool _httpOk=false;

bool webCredAvailable(){
  getWebUser();
  Serial.print(_webUser.uname);
  Serial.println(_webUser.passwd);
  if (_webUser.uname == NULL || _webUser.passwd == NULL ){
    return false;
  }
  else if (!strcmp(_webUser.uname, "null") || !strcmp(_webUser.uname, "bnVsbA==")){
    return false;
  }
  else {
    return true;
  }
}


bool httpInit(bool new_reg){
  WiFiClient client;
  HTTPClient http;

  char *url = bufa;
  url += sprintf(url, "%s?uuid=%s&name=%s&cap=CAMERA,MOTON&v=%d&tz=%s&ip=%u.%u.%u.%u&n=",
                        registerURL, _deviceId, _cn, _v, _tz, _localIp[0], _localIp[1], _localIp[2], _localIp[3]);
                        
  *url++ = (new_reg ? 'n' : 'o');
  *url++ = 0;
  http.begin(client, bufa);

  char *ct = bufb;
  if (webCredAvailable()){
    ct += sprintf(ct, "u=%s&p=%s&r=xyz",_webUser.uname ,_webUser.passwd); 
  }   
  else {
    ct += sprintf(ct, "u=none&p=bm9uZQ==");
  }
 *ct++ = 0;

 Serial.println(bufb);
  
  // Send HTTP POST request
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
  int httpResponseCode = http.POST(bufb);
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    payload = http.getString();
    Serial.println(payload);
    if (payload.indexOf("success") >= 0) {
        _httpOk = true;
        authGood();
        http.end();
        return true;       
    }
    else if (payload.indexOf("reset") >= 0) {
        state_zero = RESET;
        state_one = NADA_ONE;
        return true;       
    }
    else {
        authError();
        http.end();
        return false;
    }
  }
  else {
    http.end();
    Serial.println(payload);
    return false;  
  }
}


esp_err_t httpFirmwareUpgrade()
{
    WiFiClient client;
    
    HTTPClient http;
    Serial.print("-U-");
 
    char *url = bufb;
    url += sprintf(url, "http://ping.ibeyonde.com/api/esp32_scb.php?uuid=%s", _deviceId);
    *url++ = 0;
    http.begin(client, bufb);      
    
    uint16_t version;
    if (http.GET() > 0) { 
        String resp = http.getString();
        resp.trim();
        strcpy(bufb, resp.c_str());
        http.end(); 
        
        char *token = strtok(bufb, "-");
        version = atoi(token);
        token = strtok(NULL, "-");
        Serial.print(token);
        if (strcmp(token, _token) == 0 ){
          Serial.printf("cur_ver=%d, new_ver=%d \n", _v, version);
          if (_v < version){
            esp_http_client_config_t config = {
                .url = "http://ping.ibeyonde.com/api/esp32_scb.bin",
                .cert_pem = _ca_cert,
            };
            Serial.println("Upgrading ...");
            esp_err_t ret = esp_https_ota(&config);
            if (ret == ESP_OK) {
                Serial.println("Successfully downloaded upgrade ...");
                esp_restart();
            }
            else {
              Serial.println("Upgrade failed ...");
              return ESP_FAIL;  
            }
          }
          else {
            Serial.println("No upgrade required");
            return ESP_OK;
          }
        }
        else {
          Serial.print("bad veil");
          return ESP_FAIL;
        }
    }
    else {
        Serial.print("getting version from server failed");
        http.end(); 
        return ESP_FAIL;
    }
    
    return ESP_FAIL;
}

String httpGetToken(){
  WiFiClient client;
  HTTPClient http;

  char *url = bufa;
  url += sprintf(url, "%s?hn=%s",tokenURL, _deviceId);
  *url++ = 0;
  
  http.begin(client, bufa);
  
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
  char *ct = bufa;
  ct += sprintf(ct, "u=%s&p=%s",_webUser.uname , _webUser.passwd);
  *ct++ = 0;
  int httpResponseCode = http.POST(bufa);
  
  String payload; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    payload = "";
  }
  // Free resources
  http.end();

  if (payload.indexOf("sql_402") >= 0) {
      authError();
  }
  else {
    authGood();
  }

  Serial.println(payload);
  return payload;       
}

/**

POST /api/motionAlert.php?hn=3105613C&tz=Asia/Calcutta&tp=MOTION&grid=0 HTTP/1.1

Host: esp32.ibeyonde.com
Token: pdhbaNtKDg
Content-Type: multipart/form-data; boundary=k63ncap5d
Content-Length: 5934

--k63ncap5d
Content-Disposition: form-data; name="fileToUpload"; filename="motion_20211005123719.133.jpg"
Content-Type: image/jpeg

+++data+++
--k63ncap5d--

 */
void httpMotionAlert(){
  if (_token == ""){
    strcpy(_token, httpGetToken().c_str());
  }
  WiFiClient client;
  
  if (client.connect("ping.ibeyonde.com", 80) ) {
    String getAll;
    String getBody;


    char *head = bufb;
    head += sprintf(head, "--k63ncap5d\r\nContent-Disposition: form-data; name=\"fileToUpload\"; filename=\"motion_%s.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n", _ts);
    *head++ = 0;
    const char *tail = "\r\n--k63ncap5d--\r\n";
  
    char *pt1 = bufa;
    pt1 += sprintf(pt1, "POST /api/motionAlert.php?hn=%s&tz=%s&tp=MOTION&grid=0 HTTP/1.1\r\nHost: ping.ibeyonde.com\r\nToken: %s\r\nContent-Length: %d\r\nContent-Type: multipart/form-data; boundary=k63ncap5d\r\n\r\n",
                              _deviceId, _tz, _token, (_fb_len + strlen(bufb) + strlen(tail)));
    *pt1++ = 0;
    client.print(bufa);
    client.print(bufb);

    uint8_t *fbBuf = _camera_fb;
    for (size_t n=0; n<_fb_len; n=n+1024) {
      if (n+1024 < _fb_len) {
        client.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (_fb_len%1024>0) {
        size_t remainder = _fb_len%1024;
        client.write(fbBuf, remainder);
      }
    } 
    client.print(tail);
    
    int timoutTimer = 5000;
    long startTimer = millis();
    boolean state = false;
    
    while ((startTimer + timoutTimer) > millis()) {
      while (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (getAll.length()==0) { state=true; }
          getAll = "";
        }
        else if (c != '\r') { getAll += String(c); }
        if (state==true) { getBody += String(c); }
        startTimer = millis();
      }
      if (getBody.length()>0) { break; }
    }
    client.stop();
    
    if (getBody.indexOf("token_400") >= 0) {
        Serial.print("-BT-");
        memcpy(_token, 0, 11);
    }
    Serial.print("!");
  }
  else {
    Serial.print("-CF-");
  }
}

void httpMotionAlertTS(){
  if (_token == ""){
    strcpy(_token, httpGetToken().c_str());
  }
  wifiGetTimeStampString(); 

  httpMotionAlert();
}

void httpBellAlert(){
  if (_token == ""){
    strcpy(_token, httpGetToken().c_str());
  }

  WiFiClient client;
  HTTPClient http;

  wifiGetTimeStampString(); 
  
  char *pt1 = bufb;
  pt1 += sprintf(pt1, "%s?hn=%s&tz=Asia/Calcutta&ts=%s", bellAlertURL, _deviceId, _ts);
  *pt1++ = 0;

  http.begin(client, bufb);
  http.addHeader("Token", String(_token));
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print(">");
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
}

#endif
