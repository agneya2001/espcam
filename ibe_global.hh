#ifndef IBE_GLOBAL
#define IBE_GLOBAL

#include <NTPClient.h>

///BROWNOUT//
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
///BROWNOUT//


#define LED_BUILTIN 4
#define BELL_PIN GPIO_NUM_13

uint8_t _error=0;
uint8_t _auth_error =0;
uint16_t _pre_bell=0;

String _deviceId;

char _ts[19];

IPAddress _localIp;

uint16_t _motion_delta=600;

unsigned long udp_reg_time = millis();
volatile unsigned long motion_time = millis();
unsigned long udp_send_time = millis();
unsigned long bluetooth_time = millis();
unsigned long bell_time = millis();
unsigned long button_time = 0;
unsigned long reset_time = 0;

SemaphoreHandle_t camMutex;

enum global_states_one { NADA_ONE, SCAN_WIFI, INITWIFI, BLUETOOTH, CAM, WIFI, BELL_INTR, WEB, HTTP, TOKEN, UDP, RUNNING_ONE, MOTION_CAPTURE, MOTION_CHECK, MOTION_ALERT, 
                          BTH_PROBE, BTH_WAIT, BTH_RESET, REGISTER_NEW };
                           
  
enum global_states_zero { NADA_ZERO, RUNNING_ZERO, BELL, BELL_PIC, UPGRADE, RESET, ERRORS, AUTH_ERRORS, RESTART };

enum global_states_one state_one = NADA_ONE;
enum global_states_zero state_zero = NADA_ZERO;


char bufa[512];
char bufb[256];
char bufc[128];
char bufu[128];

void IRAM_ATTR globalBell(void* arg) {
  if (state_zero == BELL) return;
  _pre_bell++;
  if (_pre_bell > 60000){
    state_zero = BELL;
     _pre_bell = 0;
  }
}

void globalInit(){
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  
  pinMode(LED_BUILTIN, OUTPUT);
}


void globalBellInterrupt(){
  //BELL BUTTON
  int err = gpio_isr_handler_add(BELL_PIN, globalBell,  (void*) BELL_PIN);
  if (err != ESP_OK) {
    Serial.printf("handler add failed with error 0x%x \r\n", err);
  }
  gpio_set_direction(BELL_PIN, GPIO_MODE_INPUT);
  gpio_set_pull_mode(BELL_PIN, GPIO_PULLDOWN_ONLY);
  gpio_set_intr_type(BELL_PIN, GPIO_INTR_HIGH_LEVEL);
  
}

void authError(){
  _auth_error++;
  state_zero = AUTH_ERRORS;
}

void authGood(){
  _auth_error = 0;
}

void error(){
  _error++;
  state_zero = ERRORS;
}

void good(){
  _error = 0;
}

#endif
