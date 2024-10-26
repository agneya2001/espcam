
#include "ibe_time.hh"
#include "ibe_global.hh"
#include "ibe_pref.hh"
#include "ibe_cam.hh"
#include "ibe_wifi.hh"
#include "ibe_udp.hh"
#include "ibe_http.hh"
#include "ibe_web.hh"
#include "ibe_bth.hh"
/**
    Using board version 2.0.0 rc2
    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json

    Arduino core 1
  **/

TaskHandle_t Task1;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Serial.setDebugOutput(true);

  initPref();

  globalInit();

  camMutex = xSemaphoreCreateMutex();
  state_one = INITWIFI;

  Serial.printf("version=%d, name=%s\n", _v, _cn);

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */
}



void Task1code( void * pvParameters ){
  
  while(1){

      switch (state_zero)
      {
        case RUNNING_ZERO:
          if (WiFi.status() != WL_CONNECTED) {
            Serial.println(" ====== Network down restarting");
            state_zero = RESTART;
            break;
          }
    
          
          if (millis() - udp_send_time > 100) {
            udpListen();
            if (_udpSend > 0) {
              motion_time = millis() + 4000; //delay motion sensing
              udpSendEventHandler();
            }
            udp_send_time = millis();
          }
    
          if ((millis() - udp_reg_time) > 25000) {
            _timeClient.update();
            udpRefreshWithBroker();
            udp_reg_time = millis();
          }
    
          break;
              
        case BELL:
          if (millis() - button_time > 10000) {
            digitalWrite(LED_BUILTIN, HIGH);
            button_time = millis();
            Serial.println("*");
            camCaptureStill();
            httpBellAlert();
            state_zero = BELL_PIC;
            digitalWrite(LED_BUILTIN, LOW);
          }
          else {
            Serial.print("-R-");
            state_zero = RUNNING_ZERO;
          }
          break;
    
        case BELL_PIC:
          httpMotionAlertTS();
          _pre_bell=0;
          bell_time = millis();
          state_zero = RUNNING_ZERO;
          break;
          
        case UPGRADE:
          httpFirmwareUpgrade();
          state_zero = RUNNING_ZERO;
          break;
    
        case RESET:
          removeWifiUser();
          Serial.println("Reset Restarting");
          state_zero = RESTART;
          break;
          
        case RESTART:
          digitalWrite(LED_BUILTIN, HIGH);
          Serial.println("Restarting");
          delay(10000);
          digitalWrite(LED_BUILTIN, LOW);
          ESP.restart();
          break;
    
    
        case ERRORS:
          if (_error > 10) {
            Serial.println("Errors Restarting");
            state_zero = RESTART;
          }
          break;
       
        case AUTH_ERRORS:
          if (_auth_error > 10) {
            Serial.println("Auth errors Restarting");
            state_zero = RESET;
          }
          break; 
            
        case NADA_ZERO:
        default:
            delay(50);
      }
      vTaskDelay(20);
  } 
}


uint8_t dt = 1;
////////////////////////////   LOOP   ///////////////////////////
void loop() {

  switch (state_one)
  {

    case INITWIFI:
      if (bthInit()) {
        state_one = BLUETOOTH;
      }
      else {
        state_one = WIFI;
      }
      break;

    case BLUETOOTH:
      digitalWrite(LED_BUILTIN, HIGH);
      bthStateMachine();
      digitalWrite(LED_BUILTIN, LOW);
      break;


    case WIFI:
      if (wifiInit()) {
        state_one = CAM;
        dt = 1;
      }
      else {
        state_one = BTH_PROBE;
      }
      break;

    case BTH_PROBE:
      SerialBT.register_callback(bthResetCallback);
      SerialBT.begin(_cn);
      Serial.println(_cn);
      state_one = BTH_WAIT;
      bluetooth_time = millis();
      break;

    case BTH_WAIT:
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      if (millis() - bluetooth_time > 10000 * dt) {
        if (dt < 20)dt++;
        Serial.println("Reconnect");
        digitalWrite(LED_BUILTIN, LOW);
        SerialBT.disconnect();
        SerialBT.end();
        Serial.println("BT stopped");
        state_one = WIFI;
      }
      break;

    case BTH_RESET:
      digitalWrite(LED_BUILTIN, HIGH);
      bthResetStateMachine();
      digitalWrite(LED_BUILTIN, LOW);
      break;

    case CAM:
      camInit();
      state_one = BELL_INTR;
      break;

    case BELL_INTR:
      globalBellInterrupt();
      state_one = WEB;
      break;

    case WEB:
      webInit();
      state_one = HTTP;
      break;

    case HTTP:
      if (webCredAvailable()){
        if (httpInit(false)) {
          state_one = TOKEN;
        }
        else {
          delay(5000);
        }
      }
      else {
        delay(5000);
        state_zero = RESET;
      }
      break;

    case TOKEN:
       strcpy(_token, httpGetToken().c_str());
       if (sizeof(_token) < 11){
          // bad token repeat
          delay(5000); 
          state_one = INITWIFI;
       }
       else {
         state_one = UDP;
       }
       break;

    case UDP:
      if (udpInit()) {
        state_one = RUNNING_ONE;
        state_zero = RUNNING_ZERO;
      }
      else {
        delay(5000);
      }
      break;

    case RUNNING_ONE:
      if (_ho && _httpOk &&  (millis() - motion_time) > _motion_delta) {
        state_one = MOTION_CAPTURE;
        motion_time = millis();
      }
          
      if (millis() - bell_time > 5000){
        _pre_bell=0;
        bell_time = millis();
      }
      delay(100);
      break;

    case MOTION_CAPTURE:
      if (camCaptureStill()) {
        if (state_one == MOTION_CAPTURE)
          state_one = MOTION_CHECK;
      }
      else {
        if (state_one == MOTION_CAPTURE)
          state_one = RUNNING_ONE;
      }
      break;


    case MOTION_CHECK:
      if (camMotionDetect()) {
        if (state_one == MOTION_CHECK)
          state_one = MOTION_ALERT;
      }
      else
      {
        camUpdateFrame();
        if (state_one == MOTION_CHECK)
          state_one = RUNNING_ONE;
      }
      break;

    case MOTION_ALERT:
      httpMotionAlertTS();
      camUpdateFrame();
      if (state_one == MOTION_ALERT)
        state_one = RUNNING_ONE;
      break;

    case REGISTER_NEW:
      if (httpInit(true)) {
        Serial.printf("Token = %s\n", _token);
        state_one = NADA_ONE;
        state_zero = RESTART;
      }
      else {
        delay(5000);
      }
      break;
      
    case NADA_ONE:
      delay(200);
      
    default:
      delay(200);
  }//END SWITCH
}//end loop
