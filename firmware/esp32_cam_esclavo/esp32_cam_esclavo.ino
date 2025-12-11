/*
  Proyecto: Nodo de Monitoreo Agrícola (Esclavo - ESP32-CAM)
  Versión: Producción V5
  
  Conexión con MAESTRO:
  - CAM RX (Pin 3/U0R) <--- Maestro TX (Pin 17)
  - CAM TX (Pin 1/U0T) ---> Maestro RX (Pin 16)
  - GND <---> GND
*/

#include "esp_camera.h"
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <HTTPClient.h>
#include <base64.h> 

// ==========================================
// 1. CREDENCIALES Y URL
// ==========================================
const char* ssid = "PruebaESP";
const char* password = "ESP32EVEYCAM";

// URL DE GOOGLE APPS SCRIPT
String GOOGLE_SCRIPT_URL = "https://script.google.com/macros/s/AKfycbyyV9ssSRekLMhbwNKal6BVfquDHs9KL3IBu9UqAfuq2kJcidr7m-zvfVFHBbpKtIb98Q/exec";

// ==========================================
// 2. DEFINICIÓN DE PINES (Modelo AI-Thinker)
// ==========================================
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define LED_ROJO          33 

// ==========================================
// 3. FUNCIONES
// ==========================================
void initCamera();
void initWiFi();
String tomarFotoBase64();
void enviarAGoogle(String jsonRecibido, String fotoBase64);

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Desactivar Brownout

  Serial.begin(115200); // Para escuchar al Maestro
  
  pinMode(LED_ROJO, OUTPUT);
  digitalWrite(LED_ROJO, HIGH); // Apagado inicial

  initCamera();
  initWiFi();
  
  for(int i=0; i<3; i++) { // Señal de vida
    digitalWrite(LED_ROJO, LOW); delay(100); 
    digitalWrite(LED_ROJO, HIGH); delay(100);
  }
}

void loop() {
  if (Serial.available()) {
    String incomingData = Serial.readStringUntil('\n');
    incomingData.trim(); 

    if (incomingData.startsWith("{") && incomingData.endsWith("}")) {
      
      digitalWrite(LED_ROJO, LOW); // LED ON: Trabajando
      
      bool requiereFoto = (incomingData.indexOf("\"FOTO\":1") >= 0);
      String imagenB64 = "";

      if (requiereFoto) {
        imagenB64 = tomarFotoBase64();
      }

      enviarAGoogle(incomingData, imagenB64);
      
      digitalWrite(LED_ROJO, HIGH); // LED OFF: Terminado
    }
  }
}

// ---------------------------------------------------------
// IMPLEMENTACIÓN DE FUNCIONES
// ---------------------------------------------------------

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000) {
    delay(500);
  }
}

void initCamera() {
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
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if(psramFound()){
    config.frame_size = FRAMESIZE_VGA; 
    config.jpeg_quality = 12;
    config.fb_count = 1;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 15;
    config.fb_count = 1;
  }

  esp_camera_init(&config);
}

String tomarFotoBase64() {
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  
  if(!fb) return "";

  String imageFile = base64::encode(fb->buf, fb->len);
  esp_camera_fb_return(fb);
  
  return imageFile;
}

void enviarAGoogle(String jsonRecibido, String fotoBase64) {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    
    http.begin(GOOGLE_SCRIPT_URL);
    http.addHeader("Content-Type", "application/json");
    
    String payloadFinal = jsonRecibido;
    if (fotoBase64.length() > 0) {
      payloadFinal = payloadFinal.substring(0, payloadFinal.lastIndexOf('}'));
      payloadFinal += ",\"fotoBase64\":\"" + fotoBase64 + "\"}";
    }
    
    http.POST(payloadFinal);
    http.end();
  } else {
    initWiFi();
  }
}