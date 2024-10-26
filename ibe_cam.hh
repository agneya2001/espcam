#ifndef IBE_CAM
#define IBE_CAM

#include "esp_camera.h"

#define CAMERA_MODEL_AI_THINKER
#define OV2640

#include "camera_pins.h"
#include "camera_index.h"

static size_t pixels, bmp_len, block_size, pixel_block_size, buf_len;

static const int BMP_HEADER_LEN = 54;

#define TOTAL_BLOCKS 2400
#define THRESHOLD 120 // 5% should be different
const int BLOCK_DIFFERENCE=5;  // diffrence between prev and cur block

static uint8_t prev[TOTAL_BLOCKS]={ 0 };
static uint8_t curr[TOTAL_BLOCKS];

static size_t out_len;
static uint8_t _camera_fb[42*1024];
static size_t _fb_len;


framesize_t setFrameSize(uint8_t fs){
  if (fs == 8){
    _fs=8;
    pixels=640*480;
    bmp_len=pixels*3;
    block_size=128;
    pixel_block_size=3*block_size;
    buf_len=pixels/block_size;
    _motion_delta = 800;
    return FRAMESIZE_VGA;
  }
  else if (fs == 5){
    _fs=5;
    pixels=320*240;
    bmp_len=pixels*3;
    block_size=32;
    pixel_block_size=3*block_size;
    buf_len=pixels/block_size;
    _motion_delta = 400;
    return FRAMESIZE_QVGA;
  }
  else {
    _fs=2;
    pixels=160*120;
    bmp_len=pixels*3;
    block_size=8;
    pixel_block_size=3*block_size;
    buf_len=pixels/block_size;
    _motion_delta = 300;
    return FRAMESIZE_QQVGA;
  }
}



int setXclk(uint8_t fs){
  if (fs == 8){
    return 8000000;
  }
  else if (fs == 5){
    return 10000000;
  }
  else {
    return 16000000;
  }
}


int setQuality(uint8_t fs){
  if (fs == 8){
    return 8;
  }
  else if (fs == 5){
    return 6;
  }
  else {
    return 2;
  }
}


bool camInit()
{
  Serial.println("init_camera start");
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.pixel_format = PIXFORMAT_JPEG;//YUV422,GRAYSCALE,RGB565,JPEG
  
  config.frame_size = setFrameSize(_fs); //FRAMESIZE_VGA , FRAMESIZE_QVGA, FRAMESIZE_QQVGA
  config.jpeg_quality = setQuality(_fs);
  config.fb_count = 2;
  config.xclk_freq_hz = setXclk(_fs);
  //https://github.com/espressif/esp32-camera/blob/master/driver/include/sensor.h
  
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    state_zero=RESTART;
  }

  sensor_t *s = esp_camera_sensor_get();
  
  if (_vf==1)
      s->set_vflip(s, 1);
  if (_hf==1)
    s->set_hmirror(s, 1);
    
  return true;
}

 /**
 * Capture image and do down-sampling
 */
bool camCaptureStill() {
    xSemaphoreTake( camMutex, portMAX_DELAY );
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb){
       error();
       Serial.println("Motion Camera capture failed ");
       xSemaphoreGive( camMutex );
       return false;
    }
    good();
    
    if ( fb->len == 0) {
       Serial.println("Motion Camera capture 0 bytes ");
        xSemaphoreGive( camMutex );
       return false; 
    }
    
    uint8_t *current_frame;
    if (!frame2bmp(fb,  &current_frame, &out_len)){
      Serial.println("Converion to bmp failed");
      esp_camera_fb_return(fb);
      xSemaphoreGive( camMutex );
      return false;
    }

    memcpy(_camera_fb, fb->buf,  fb->len);
    _fb_len=fb->len;

    memset(curr, 0, buf_len);
    
    for (size_t i = BMP_HEADER_LEN,k=0; i < out_len; i=i + pixel_block_size, k++) {
      size_t val=0;
      for (size_t j=0;j<pixel_block_size;j++){
        val += current_frame[i + j];
      }
      curr[k]=val/pixel_block_size; // average pixel value in the block
    }

    esp_camera_fb_return(fb);
    free(current_frame);
    xSemaphoreGive( camMutex );
    return true;
}
bool camMotionDetect() {
    if (prev[0] ==  0){
      //First frame ignore
      return false;
    }
    
    size_t total_diff=0;
    for (size_t i=0; i< buf_len && curr[i] > 25 && curr[i] < 230;i++){
         int diff = abs(curr[i] - prev[i]);
         if (diff > BLOCK_DIFFERENCE){
            //Serial.printf("Curr block=%d  Prev block=%d  diff=%d \n", curr[i], prev[i], diff);
            total_diff++;
         }
    }

    Serial.printf("%d\n",total_diff);
    
   return total_diff > THRESHOLD;
}

void camUpdateFrame() {
    memcpy(prev, curr,  buf_len);
}



#endif
