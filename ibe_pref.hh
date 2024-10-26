#ifndef IBE_PREF
#define IBE_PREF

#include <Preferences.h>
#include <base64.h>

#define PNAME "test"
#define NU "ssid"
#define NP "pass"
#define WU "user"
#define WP "cred"
#define HO "hist"
#define TZ "tz"
#define NM "nm"
#define FS "fs"
#define VF "vf"
#define HF "hf"

Preferences _preferences;

bool _ho=true;
char _tz[36]= { 'A', 's', 'i', 'a', '/', 'C', 'a', 'l', 'c', 'u', 't', 't', 'a' }; //36
char _cn[20]= { 'S', 'm', 'a', 'r', 't', 'C', 'a', 'm', '8' };  //8
uint8_t _fs=5;
uint8_t _hf=0;
uint8_t _vf=0;
uint16_t _v = 8;


struct cred {
  char uname[20]={0};
  char passwd[20]={0};
};

struct cred _webUser;
struct cred _wifiUser;

void initPref(){
  _preferences.begin(PNAME, false); 
  strcpy(_tz, _preferences.getString(TZ, "Asia/Calcutta").c_str());
  Serial.println(_tz);
  
  _ho = _preferences.getBool(HO, true);
  Serial.print("History = ");
  Serial.println(_ho);
    
  _fs = _preferences.getUChar(FS, 5);
  Serial.print("FS = ");
  Serial.println(_fs);
  _vf = _preferences.getUChar(VF, 0);
  Serial.print("VF = ");
  Serial.println(_vf);
  _hf = _preferences.getUChar(HF, 0);
  Serial.print("HF = ");
  Serial.println(_hf);

  strcpy(_cn, _preferences.getString(NM, "SmartCam8").c_str());
  Serial.println(_cn);

  _preferences.end();
}


struct cred getWifiUser(){
  _preferences.begin(PNAME, false); 
  strcpy(_wifiUser.uname, _preferences.getString(NU).c_str());
  strcpy(_wifiUser.passwd, _preferences.getString(NP).c_str());
  Serial.print(_wifiUser.uname);
  Serial.println(_wifiUser.passwd);
  _preferences.end();
  return _wifiUser;
}

void saveWifiUser(String uname, String pass){
  Serial.println("Wifi save Ssid=" +  uname);
  Serial.println("Wifi save pass=" +  pass);
  _preferences.begin(PNAME, false); 
  _preferences.putString(NU, uname);
  _preferences.putString(NP, pass);
  _preferences.end();
}

void removeWifiUser(){
  _preferences.begin(PNAME, false); 
  _preferences.remove(NU);
  _preferences.remove(NP);
  _preferences.end();
}


struct cred getWebUser(){
  _preferences.begin(PNAME, false); 
  strcpy(_webUser.uname, _preferences.getString(WU).c_str());
  strcpy(_webUser.passwd, _preferences.getString(WP).c_str());
  _preferences.end();
  return _webUser;
}

void saveWebUser(String uname, String pass){
  _preferences.begin(PNAME, false); 
  _preferences.putString(WU, uname);
  _preferences.putString(WP, base64::encode(pass));
  _preferences.end();
}

void removeWebUser(){
  _preferences.begin(PNAME, false); 
  _preferences.remove(WU);
  _preferences.remove(WP);
  _preferences.end();
}


#endif
