#ifndef IBE_BLUETOOTH
#define IBE_BLUETOOTH

#include "ibe_global.hh"
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Example: AT+XAPL=ABCD-1234-0100,10 (Supports battery reporting and Siri status)

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define MAX_NET 10
static uint8_t _ms;
static char _ssids_array[MAX_NET][32];
static int _rssis_array[MAX_NET];

BluetoothSerial SerialBT;

enum wifi_setup_stages { NONE, BTH_START, BTH_STARTED, SCAN_START, SCAN_COMPLETE, SSID_ENTERED, WAIT_PASS, PASS_ENTERED, 
                      WAIT_WEBUSER, WEBUSER_ENTERED, LOGIN_FAILED, WIFI_CONNECTED };
static enum wifi_setup_stages wifi_stage = NONE;


enum wifi_reset_stages { NAAH, CHECK_RESET, WAIT_RESET, CONTINUE };
static enum wifi_reset_stages reset_stage = NAAH;

void bthCallbackShowIp(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if (event == ESP_SPP_SRV_OPEN_EVT || event == ESP_SPP_DATA_IND_EVT) {
    Serial.print("ESP32 IP: ");
    Serial.println(_localIp);
    SerialBT.print("ESP32 IP: ");
    SerialBT.println(_localIp);
  }
}

bool bthInit(){
  if (wifiCredAvailable() && webCredAvailable()){
     wifi_stage = NONE;
     return false;
  }
  else {
    wifi_stage = BTH_START;
    return true;
  }
}

bool bthScanWifiNetworks()
{ 
  int n =  WiFi.scanNetworks();
  if (n <= 0 ) {
    Serial.println("Failed to get networks .. restarting");
    delay(200);
    return false;
  } else {
    Serial.print(n);
    Serial.println(" Networks found");
    int i = 0;
    for (; i < n && i < MAX_NET; i++) {
      strcpy(_ssids_array[i], WiFi.SSID(i).c_str());
      _rssis_array[i]= WiFi.RSSI(i);
    }
    _ms = i;
    return true;
  }
}

bool bthShowWifiNetworks()
{
  SerialBT.println();
  SerialBT.print(_ms);
  SerialBT.println(" Networks found");
  SerialBT.println();
  Serial.println();
  Serial.print(_ms);
  Serial.println(" Networks found");
  Serial.println();
  for (int i = 0; i < _ms; ++i) {
    strcpy(_ssids_array[i], WiFi.SSID(i).c_str());
    _rssis_array[i] = WiFi.RSSI(i);
    SerialBT.print(i+1);
    SerialBT.print(": ");
    SerialBT.print(_ssids_array[i]);
    SerialBT.print(" : Strength ");
    SerialBT.println(_rssis_array[i]);
    Serial.print(i+1);
    Serial.print(": ");
    Serial.print(_ssids_array[i]);
    Serial.print(" : Strength ");
    Serial.println(_rssis_array[i]);
  }
  return true;
}


void bthResetCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    SerialBT.print("Connected to ");
    SerialBT.println(_cn);
    Serial.print("Connected to ");
    Serial.println(_cn);
    state_one = BTH_RESET;
    reset_stage = CHECK_RESET;
  }
  else 
  if (event == ESP_SPP_DATA_IND_EVT && reset_stage == WAIT_RESET) { 
    int reset = SerialBT.readString().toInt();
    if (reset == 1){
      removeWifiUser();
      Serial.println("BTH reset Restarting");
      state_zero = RESTART;
    }
    else {
        reset_stage = CONTINUE;
    }
  }
}



void bthResetStateMachine(){
  
  switch (reset_stage)
  {
    case NAAH:
      delay(200);
      break;

    case CHECK_RESET:
      SerialBT.print("Unable to connect to the Wifi with SSID=");
      SerialBT.println(_wifiUser.uname);
      Serial.print("Unable to connect to the Wifi with SSID=");
      Serial.println(_wifiUser.uname);
      SerialBT.println("");
      Serial.println("");
      SerialBT.println("To change WiFi credentials please reset device");
      Serial.println("To change WiFi credentials please reset device");
      SerialBT.println("");
      Serial.println("");
      SerialBT.println("Type 1 to reset");
      Serial.println("Type 1 to reset");
      SerialBT.println("Type 0 to cancel.");
      Serial.println("Type 0 to cancel.");
      reset_stage = WAIT_RESET;
      break;

    case WAIT_RESET:
      delay(200);
      break;
      
   case CONTINUE:
      SerialBT.println("Please, ensure that the WiFi is available.");
      Serial.println("Please, ensure that the WiFi is available.");
      SerialBT.println("");
      Serial.println("");
      SerialBT.println("Bluetooth disconnecting...");
      SerialBT.println("");
      Serial.println("");
      SerialBT.println("Disconnected");
      SerialBT.flush();
      SerialBT.disconnect();
      SerialBT.end();
      Serial.println("BT stopped");
      state_one = WIFI;
      reset_stage = NAAH;
      break;
  };
}


void bthCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    SerialBT.print("Connected to ");
    SerialBT.println(_cn);
    Serial.print("Connected to ");
    Serial.println(_cn);
    state_one = BLUETOOTH;
    wifi_stage = SCAN_START;
  }
  else 
  if (event == ESP_SPP_DATA_IND_EVT && wifi_stage == SCAN_COMPLETE) { // data from phone is SSID
    int client_wifi_ssid_id = SerialBT.readString().toInt();
    strcpy(_wifiUser.uname, _ssids_array[client_wifi_ssid_id - 1]);
    SerialBT.print("SSID entered ");
    SerialBT.println(_wifiUser.uname);
    Serial.print("SSID entered ");
    Serial.println(_wifiUser.uname);
    SerialBT.flush();
    wifi_stage = SSID_ENTERED;
  }
  else
  if (event == ESP_SPP_DATA_IND_EVT && wifi_stage == WAIT_PASS) { // data from phone is password
    String pass = SerialBT.readString();
    pass.trim();
    strcpy(_wifiUser.passwd, pass.c_str());
    Serial.print("Password entered ");
    Serial.println(_wifiUser.passwd);
    SerialBT.print("Password entered ");
    SerialBT.println(_wifiUser.passwd);
    SerialBT.flush();
    wifi_stage = PASS_ENTERED;
  }
  else
  if (event == ESP_SPP_DATA_IND_EVT && wifi_stage == WAIT_WEBUSER) { // data from phone is webuser
    String up = SerialBT.readString();
    up.trim();
    strcpy(_webUser.uname, up.substring(0, up.indexOf('%')).c_str());
    strcpy(_webUser.passwd, up.substring(up.lastIndexOf('%') + 1, up.length()).c_str());
    Serial.print("Initialization complete ");
    Serial.println(_webUser.passwd);
    SerialBT.print("Initialization complete ");
    SerialBT.println(_webUser.uname);
    SerialBT.flush();
    wifi_stage = WEBUSER_ENTERED;
  }
}


void bthStateMachine(){
  
  switch (wifi_stage)
  {
    case NONE:
      delay(200);
      break;
      
    case BTH_START:
      SerialBT.register_callback(bthCallback);
      SerialBT.begin(_cn); //Bluetooth device name
      Serial.println(_cn);
      wifi_stage = BTH_STARTED;
      break;

    case BTH_STARTED:
      delay(200);
      break;
    
    case SCAN_START:
      SerialBT.println("Scanning Wi-Fi networks");
      Serial.println("Scanning Wi-Fi networks");
      if (bthScanWifiNetworks()) {
        bthShowWifiNetworks();
        SerialBT.println();
        SerialBT.println("Please enter the Number for your Wi-Fi");
        Serial.println();
        Serial.println("Please enter the Number for your Wi-Fi");
        wifi_stage = SCAN_COMPLETE;
      }
      break;

    case SCAN_COMPLETE:
      delay(200);
      break;
    
    case SSID_ENTERED:
      SerialBT.println("Please enter your Wi-Fi password");
      Serial.println("Please enter your Wi-Fi password");
      wifi_stage = WAIT_PASS;
      break;

    case WAIT_PASS:
      delay(200);
      break;
      
    case PASS_ENTERED:
      SerialBT.println("Please wait for Wi-Fi connection...");
      Serial.println("Please wait for Wi_Fi connection...");
      delay(200);
      if (wifiInit()) { // Connected to WiFi
        String connected_string = "IP: " + WiFi.localIP().toString();
        SerialBT.println(connected_string);
        Serial.println(connected_string);
        SerialBT.println();
        SerialBT.println("----Connected------");
        wifi_stage = WIFI_CONNECTED;
      } else { // try again
        SerialBT.println("Wi-Fi connection failed");
        Serial.println("Wi-Fi connection failed");
        SerialBT.flush();
        wifi_stage = LOGIN_FAILED;
      }
      break;

    case LOGIN_FAILED:
      SerialBT.println("Please, re-enter the wifi credentials");
      Serial.println("Please, re-enter the wifi credentials");
      wifi_stage = SCAN_START;
      break;

    case WIFI_CONNECTED:
      SerialBT.println("Initializing device...");
      Serial.println("Initializing device...");
      saveWifiUser(_wifiUser.uname,  _wifiUser.passwd);
      wifi_stage = WAIT_WEBUSER;
      digitalWrite(LED_BUILTIN, LOW);
      break;

    case WAIT_WEBUSER:
      delay(200);
      break;
      
    case WEBUSER_ENTERED:
      SerialBT.println("Bluetooth disconnecting...");
      SerialBT.println("Disconnected");
      SerialBT.flush();
      SerialBT.disconnect();
      SerialBT.end();
      Serial.println("BT stopped");
      saveWebUser(_webUser.uname,  _webUser.passwd);
      wifi_stage = NONE;
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
      state_one = REGISTER_NEW;
      break;
  }//Switch 
}//bthStateMachine

#endif
