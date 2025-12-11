/*
  Proyecto: Nodo de Monitoreo Agrícola (MAESTRO - SENSORES)
  Versión: Producción V5 / Debug Rápido (15s)
  
  Conexión Física:
  - TX2 (Pin 17) -> va al RX de la ESP32-CAM
  - RX2 (Pin 16) -> va al TX de la ESP32-CAM
  - GND -> va al GND de la ESP32-CAM
*/

#include <WiFi.h>
#include <Wire.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "RTClib.h"
#include "time.h"

// ==========================================
// 1. CREDENCIALES
// ==========================================
const char* ssid     = "PruebaESP";
const char* password = "ESP32EVEYCAM";

// Servidor NTP (Hora Internet)
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000; // Colombia UTC -5
const int   daylightOffset_sec = 0; 

// ==========================================
// 2. CONFIGURACIÓN DE PINES
// ==========================================
#define DHTPIN 2
#define DHTTYPE DHT22
const int oneWireBus = 15;  
#define SENSOR_PIN 32  
#define RXD2 16 // Comunicación con CAM
#define TXD2 17 // Comunicación con CAM

// Coordenadas Fijas (Popayán)
const float LATITUD_FIJA = 2.444; 
const float LONGITUD_FIJA = -76.614;

// ==========================================
// 3. OBJETOS
// ==========================================
DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
RTC_DS3231 rtc; 

// ==========================================
// 4. VARIABLES GLOBALES
// ==========================================
float tempAire = 0.0, humAire = 0.0;
float temperatureC = 0.0;
int humedadSuelo = 0;
DateTime now;

// ==========================================
// 5. CONTROL DE TIEMPO (MODO PRUEBA)
// ==========================================
unsigned long previousMillisDatos = 0;
// *** ¡ATENCIÓN! 15 SEGUNDOS PARA PRUEBAS. Para producción cambiar a 300000 (5 Min) ***
const long intervalDatos = 15000; 

int contadorCiclos = 0;
// Cada 2 envíos (30s) pedimos foto. Para producción cambiar a 240 (60 min)
const int ciclosParaFoto = 2; 

// ==========================================
// 6. DECLARACIÓN DE FUNCIONES
// ==========================================
void leerSensores();
void enviarDatosACamara(bool ordenFoto);
void mostrarDatosSerialPC();
void sincronizarRTCconNTP();

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); // Iniciar puerto Serial2 para hablar con la CAM
  Wire.begin(21, 22);

  dht.begin();
  sensors.begin();
  sensors.setWaitForConversion(false); // No bloquear

  if (!rtc.begin()) {
    Serial.println("Error crítico: No se encuentra RTC DS3231");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println(">> RTC perdió energía. Buscando hora en internet...");
    sincronizarRTCconNTP();
  }

  Serial.println("\n--- MAESTRO INICIADO (MODO PRUEBA 15s) ---");
}

// ==========================================
// LOOP PRINCIPAL
// ==========================================
void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillisDatos >= intervalDatos) {
    previousMillisDatos = currentMillis;
    
    leerSensores();
    
    contadorCiclos++;
    bool tomarFoto = false;
    
    if (contadorCiclos >= ciclosParaFoto) {
      tomarFoto = true;
      contadorCiclos = 0;
      Serial.println(">> CICLO CUMPLIDO: Ordenando FOTO.");
    }

    mostrarDatosSerialPC();
    enviarDatosACamara(tomarFoto);
    
    sensors.requestTemperatures(); 
  }
}

// ==========================================
// FUNCIONES
// ==========================================

void leerSensores() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t)) tempAire = t;
  if (!isnan(h)) humAire = h;

  float tempS = sensors.getTempCByIndex(0);
  if (tempS != -127.00) temperatureC = tempS;

  long sum = 0;
  for(int i=0; i<10; i++){
      sum += analogRead(SENSOR_PIN);
      delay(2);
  }
  int avgValue = sum / 10;
  humedadSuelo = map(avgValue, 4095, 0, 0, 100); 
  humedadSuelo = constrain(humedadSuelo, 0, 100);

  now = rtc.now();
}

void enviarDatosACamara(bool ordenFoto) {
  // Construir JSON
  String payload = "{";
  payload += "\"temp\":" + String(tempAire) + ",";
  payload += "\"hum\":" + String(humAire) + ",";
  payload += "\"suelo\":" + String(humedadSuelo) + ",";
  
  char fechaBuffer[25];
  sprintf(fechaBuffer, "%04d-%02d-%02d %02d:%02d:%02d", 
          now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  payload += "\"fecha\":\"" + String(fechaBuffer) + "\",";

  if (ordenFoto) {
    payload += "\"FOTO\":1";
  } else {
    payload += "\"FOTO\":0";
  }
  
  payload += "}";

  Serial2.println(payload);
  Serial.print(">> Enviado a CAM: ");
  Serial.println(payload);
}

void mostrarDatosSerialPC() {
  Serial.println("----------------------------------");
  Serial.printf("Temp: %.2f | Hum: %.2f | Suelo: %d\n", tempAire, humAire, humedadSuelo);
  Serial.println("----------------------------------");
}

void sincronizarRTCconNTP() {
  Serial.printf("Conectando WiFi (%s)...\n", ssid);
  WiFi.begin(ssid, password);
  
  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos < 20) {
    delay(500);
    Serial.print(".");
    intentos++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi OK. Sincronizando NTP...");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, 
                          timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
      Serial.println("RTC Actualizado.");
    } else {
      Serial.println("Fallo NTP.");
    }
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
  } else {
    Serial.println("\nFallo WiFi. Usando hora interna.");
  }
}