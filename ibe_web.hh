#ifndef IBE_WEB
#define IBE_WEB

#include "esp_http_server.h"

typedef struct {
        httpd_req_t *req;
        size_t len;
} jpg_chunking_t;

#define PART_BOUNDARY "jpgboundary"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\n--" PART_BOUNDARY "\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\nContent-Length: %u\n\n";
static const char* _STREAM_END = "\n--" PART_BOUNDARY "--\n";

httpd_handle_t camera_httpd = NULL;

//https://github.com/espressif/esp-idf/blob/master/components/esp_http_server/include/esp_http_server.h

static esp_err_t parse_get(httpd_req_t *req, char **obuf)
{
    char *buf = NULL;
    size_t buf_len = 0;

    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = (char *)malloc(buf_len);
        if (!buf) {
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            *obuf = buf;
            return ESP_OK;
        }
        free(buf);
    }
    httpd_resp_send_404(req);
    return ESP_FAIL;
}

static int parse_get_var(char *buf, const char * key, int def)
{
    char _int[16];
    if(httpd_query_key_value(buf, key, _int, sizeof(_int)) != ESP_OK){
        return def;
    }
    return atoi(_int);
}

static size_t jpg_encode_stream(void * arg, size_t index, const void* data, size_t len){
    jpg_chunking_t *j = (jpg_chunking_t *)arg;
    if(!index){
        j->len = 0;
    }
    if(httpd_resp_send_chunk(j->req, (const char *)data, len) != ESP_OK){
        return 0;
    }
    j->len += len;
    return len;
}


static esp_err_t index_handler(httpd_req_t *req){
    strcpy(bufb, "Ibeyonde Cam");
    
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, bufb, HTTPD_RESP_USE_STRLEN);
}

static esp_err_t cmd_handler(httpd_req_t *req){
    Serial.print("{-ic-");
    char *buf = NULL;
    char var[24];
    char variable[24];
    char value[32];
    char veil[11];

    if (parse_get(req, &buf) != ESP_OK) {
        return httpd_resp_send_500(req);
    }

    if (httpd_query_key_value(buf, "name", var, sizeof(var)) == ESP_OK &&  httpd_query_key_value(buf, "veil", veil, sizeof(veil)) == ESP_OK) {
        httpd_query_key_value(buf, "var", variable, sizeof(variable));
        httpd_query_key_value(buf, "val", value, sizeof(value));
    }
    else {
        return httpd_resp_send_404(req);
    }
    free(buf);
    
    Serial.print(veil);
    Serial.print("<>");
    Serial.print(sizeof(veil));
    if ( sizeof(veil) != 11 || strcmp(veil, _token) != 0){
        return httpd_resp_send(req, "bad veil", HTTPD_RESP_USE_STRLEN);
    }

    if (!strcmp(var, "restart")){
      state_zero = RESTART;
      strcpy(bufa, "restart");
      httpd_resp_set_type(req, "text/html");
    }
    else if (!strcmp(var, "reset")){
      state_zero = RESET;
      strcpy(bufa, "reset");
      httpd_resp_set_type(req, "text/html");
    }
    else if (!strcmp(var, "upgrade")){
      httpFirmwareUpgrade();
      strcpy(bufa, "upgrade");
      httpd_resp_set_type(req, "text/html");
    }
    else if (!strcmp(var, "config")){
      Serial.print(variable);
      Serial.print("=");
      Serial.println(value);
      if (!strcmp(variable, "history")) {
          _preferences.begin(PNAME, false); 
          _ho = !strcmp(value,"true") ? true : false;
          _preferences.putBool(HO, _ho);
          httpInit(false);
          _preferences.end();
      }
      else if (!strcmp(variable, "cloud")){
      }
      else if (!strcmp(variable, "timezone")){
          _preferences.begin(PNAME, false); 
          _preferences.putString(TZ, value);
          strcpy(_tz, value);
          _preferences.end();
          _timeClient.setTimeOffset(timeOffset(value));
      }
      else if (!strcmp(variable, "name")){
          _preferences.begin(PNAME, false); 
          _preferences.putString(NM, value);
          strcpy(_cn, value);
          httpInit(false);
          _preferences.end();
      }
      else if (!strcmp(variable, "user")){
          _preferences.begin(PNAME, false); 
          _preferences.putString(WU, value);
          strcpy(_webUser.uname, value);
          _preferences.end();
      }
      else if (!strcmp(variable, "passwd")){
          _preferences.begin(PNAME, false); 
          _preferences.putString(WP, base64::encode(value));
          strcpy(_webUser.passwd, base64::encode(value).c_str());
          _preferences.end();
      }
      else if (!strcmp(variable, "reset")){
          char *cred = bufa;
          cred += sprintf(cred, "%s\%\%%s",  _webUser.uname, _webUser.passwd);
          *cred++ = 0;
          Serial.print(bufa);
          if (!strcmp(value, bufa)){
            Serial.println("Web reset Restarting");
            state_zero = RESET;
          }
      }
      else {
         return httpd_resp_send_500(req);
      }

      strcpy(bufa, "success");
      httpd_resp_set_type(req, "text/html");
    } //// END CONFIG
    else if (!strcmp(var, "camconf")){
      int val = atoi(value);
      Serial.print(variable);
      Serial.print("=");
      Serial.println(lowByte(val));
      sensor_t *s = esp_camera_sensor_get();
      int res = 0;
  
      if (!strcmp(variable, "framesize")) {
          Serial.print(_fs);
        if (lowByte(val) != _fs ){
          _preferences.begin(PNAME, false); 
          _preferences.putUChar(FS, lowByte(val));
          _preferences.end();
          Serial.println(" ...restarting");
          state_zero = RESTART;
        }
      }
      else if (!strcmp(variable, "quality"))
          res = s->set_quality(s, val);
      else if (!strcmp(variable, "contrast"))
          res = s->set_contrast(s, val);
      else if (!strcmp(variable, "brightness"))
          res = s->set_brightness(s, val);
      else if (!strcmp(variable, "saturation"))
          res = s->set_saturation(s, val);
      else if (!strcmp(variable, "gainceiling"))
          res = s->set_gainceiling(s, (gainceiling_t)val);
      else if (!strcmp(variable, "colorbar"))
          res = s->set_colorbar(s, val);
      else if (!strcmp(variable, "awb"))
          res = s->set_whitebal(s, val);
      else if (!strcmp(variable, "agc"))
          res = s->set_gain_ctrl(s, val);
      else if (!strcmp(variable, "aec"))
          res = s->set_exposure_ctrl(s, val);
      else if (!strcmp(variable, "hmirror")){
          _preferences.begin(PNAME, false); 
          _preferences.putUChar(HF, lowByte(val));
          _preferences.end();
          res = s->set_hmirror(s, val);
          _hf = val;
      }
      else if (!strcmp(variable, "vflip")){
          _preferences.begin(PNAME, false); 
          _preferences.putUChar(VF, lowByte(val));
          _preferences.end();
          res = s->set_vflip(s, val);
          _vf = val;
      }
      else if (!strcmp(variable, "awb_gain"))
          res = s->set_awb_gain(s, val);
      else if (!strcmp(variable, "agc_gain"))
          res = s->set_agc_gain(s, val);
      else if (!strcmp(variable, "aec_value"))
          res = s->set_aec_value(s, val);
      else if (!strcmp(variable, "aec2"))
          res = s->set_aec2(s, val);
      else if (!strcmp(variable, "dcw"))
          res = s->set_dcw(s, val);
      else if (!strcmp(variable, "bpc"))
          res = s->set_bpc(s, val);
      else if (!strcmp(variable, "wpc"))
          res = s->set_wpc(s, val);
      else if (!strcmp(variable, "raw_gma"))
          res = s->set_raw_gma(s, val);
      else if (!strcmp(variable, "lenc"))
          res = s->set_lenc(s, val);
      else if (!strcmp(variable, "special_effect"))
          res = s->set_special_effect(s, val);
      else if (!strcmp(variable, "wb_mode"))
          res = s->set_wb_mode(s, val);
      else if (!strcmp(variable, "ae_level"))
          res = s->set_ae_level(s, val);
      else {
         Serial.printf("Unknown command %s = %s\n", variable, val);
         res = -1;
      }
  
      if (res < 0) {
          return httpd_resp_send_500(req);
      }

      strcpy(bufa, "success");
      
      httpd_resp_set_type(req, "text/html");
    } // END CAMCONFIG
    else if (!strcmp(var, "getconf")){
      sensor_t *s = esp_camera_sensor_get();
      char *p = bufa;
      *p++ = '{';
      p += sprintf(p, "\"pixformat\":%u,", s->pixformat);
      p += sprintf(p, "\"framesize\":%u,", s->status.framesize);
      p += sprintf(p, "\"quality\":%u,", s->status.quality);
      p += sprintf(p, "\"brightness\":%d,", s->status.brightness);
      p += sprintf(p, "\"contrast\":%d,", s->status.contrast);
      p += sprintf(p, "\"saturation\":%d,", s->status.saturation);
      p += sprintf(p, "\"sharpness\":%d,", s->status.sharpness);
      p += sprintf(p, "\"special_effect\":%u,", s->status.special_effect);
      p += sprintf(p, "\"wb_mode\":%u,", s->status.wb_mode);
      p += sprintf(p, "\"awb\":%u,", s->status.awb);
      p += sprintf(p, "\"awb_gain\":%u,", s->status.awb_gain);
      p += sprintf(p, "\"aec\":%u,", s->status.aec);
      p += sprintf(p, "\"aec2\":%u,", s->status.aec2);
      p += sprintf(p, "\"ae_level\":%d,", s->status.ae_level);
      p += sprintf(p, "\"aec_value\":%u,", s->status.aec_value);
      p += sprintf(p, "\"agc\":%u,", s->status.agc);
      p += sprintf(p, "\"agc_gain\":%u,", s->status.agc_gain);
      p += sprintf(p, "\"gainceiling\":%u,", s->status.gainceiling);
      p += sprintf(p, "\"bpc\":%u,", s->status.bpc);
      p += sprintf(p, "\"wpc\":%u,", s->status.wpc);
      p += sprintf(p, "\"raw_gma\":%u,", s->status.raw_gma);
      p += sprintf(p, "\"lenc\":%u,", s->status.lenc);
      p += sprintf(p, "\"vflip\":%u,", s->status.vflip);
      p += sprintf(p, "\"hmirror\":%u,", s->status.hmirror);
      p += sprintf(p, "\"dcw\":%u,", s->status.dcw);
      p += sprintf(p, "\"colorbar\":%u,", s->status.colorbar);
      p += sprintf(p, "\"history\": \"%s\",", (_ho ? "true" : "false"));
      p += sprintf(p, "\"cloud\": \"%s\",", "true");
      p += sprintf(p, "\"timezone\": \"%s\",",_tz);
      p += sprintf(p, "\"name\": \"%s\",",_cn);
      p += sprintf(p, "\"version\": \"%u\"",_v);
      *p++ = '}';
      *p++ = 0;
      httpd_resp_set_type(req, "application/json");
    }
    else {
      strcpy(bufa, "unrecognized command");
    }
    
    Serial.print("}");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    return httpd_resp_send(req, bufa, HTTPD_RESP_USE_STRLEN);
}

static esp_err_t capture_handler(httpd_req_t *req){
    Serial.print("{");
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    size_t fb_len = 0;
    
    xSemaphoreTake( camMutex, portMAX_DELAY );
    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Image capture failed");
        httpd_resp_send_500(req);
        esp_camera_fb_return(fb);
        xSemaphoreGive( camMutex );
        return ESP_FAIL;
    }
    res = httpd_resp_set_type(req, "image/jpeg");
    if(res == ESP_OK){
        res = httpd_resp_set_hdr(req, "Content-Disposition", "inline; filename=capture.jpg");
    }

    if(res == ESP_OK){
        fb_len = fb->len;
        res = httpd_resp_send(req, (const char *)fb->buf, fb->len);
    }
    esp_camera_fb_return(fb);
    xSemaphoreGive( camMutex );
    
    Serial.print("}");
    return res;
}

static esp_err_t stream_handler(httpd_req_t *req){
    Serial.print("{-is-");
      
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if(res != ESP_OK){
        return res;
    }
    size_t hlen;
    for(size_t  i=0; i< 200; i++){
      motion_time = millis() + 4000;
      xSemaphoreTake( camMutex, portMAX_DELAY );
      fb = esp_camera_fb_get();
      if (!fb) {
          Serial.println("Stream capture failed");
          esp_camera_fb_return(fb);
          res = ESP_FAIL;
          xSemaphoreGive( camMutex );
          break;
      }
      
      if ( fb->len == 0) {
         Serial.println("Motion Camera capture 0 bytes ");
         esp_camera_fb_return(fb);
         xSemaphoreGive( camMutex );
         continue; 
      }
      
      if(res == ESP_OK){
          res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
      }
      if(res == ESP_OK){
          hlen = snprintf(bufa, 64, _STREAM_PART, fb->len);
          res = httpd_resp_send_chunk(req, (const char *)bufa, hlen);
      }
      if(res == ESP_OK){
          res = httpd_resp_send_chunk(req, (const char *)fb->buf, fb->len);
      }
      esp_camera_fb_return(fb);
      xSemaphoreGive( camMutex );
      if(res != ESP_OK){
          break;
      }
    }
    res = httpd_resp_send_chunk(req, _STREAM_END, strlen(_STREAM_END));
    Serial.println("}");
    return res;
}


void webInit(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;
  config.max_uri_handlers = 4;
  config.lru_purge_enable = true;

  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
  };
  
 httpd_uri_t cmd_uri = {
      .uri       = "/cmd",
      .method    = HTTP_GET,
      .handler   = cmd_handler,
      .user_ctx  = NULL
  };
  
  httpd_uri_t capture_uri = {
    .uri       = "/capture",
    .method    = HTTP_GET,
    .handler   = capture_handler,
    .user_ctx  = NULL
  };
  
 httpd_uri_t stream_uri = {
      .uri       = "/stream",
      .method    = HTTP_GET,
      .handler   = stream_handler,
      .user_ctx  = NULL
  };
  
  
  
  if (httpd_start(&camera_httpd, &config) == ESP_OK) {
      httpd_register_uri_handler(camera_httpd, &index_uri);
      httpd_register_uri_handler(camera_httpd, &cmd_uri);
      httpd_register_uri_handler(camera_httpd, &capture_uri);
      httpd_register_uri_handler(camera_httpd, &stream_uri);
      Serial.printf("HTTP server started at http://%d.%d.%d.%d:%d/\n",  _localIp[0], _localIp[1], _localIp[2], _localIp[3], config.server_port);
  }
  
}

#endif
