/*
  Proyecto: Nodo de monitoreo agrícola
  Autores: Evelin Ortiz- Juan Lopez
  Descripción:
    Lee temperatura y humedad del aire (DHT22),
    temperatura del suelo (DS18B20),
    humedad del suelo (sensor capacitivo),
    ubicación GPS (NEO-6M) y hora (RTC DS3231).
    Muestra los datos en formato JSON.
*/

#include <WiFi.h>
#include <Wire.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include "RTClib.h"


// -------- CONFIG WIFI --------
const char* ssid = "TU_WIFI";
const char* password = "TU_PASSWORD";

// URL del API (PHP, Node.js, Flask, etc.)
const char* serverURL = "http://TU_SERVIDOR/api/guardar.php";

// -------- CONFIGURACIÓN DE PINES --------
#define DHTPIN 2
#define DHTTYPE DHT22
// GPIO where the DS18B20 is connected to
const int oneWireBus = 15;  
#define SENSOR_PIN 32  // Pin analógico del ESP32 conectado al A0 del sensor
#define RXD2 16
#define TXD2 17

// -------- OBJETOS DE SENSORES --------
DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
TinyGPSPlus gps;
HardwareSerial SerialGPS(2);
RTC_DS3231 rtc;

// -------- VARIABLES --------
float tempAire, humAire;
float temperatureC;
float humedad;
int sensorValue;
float latitud = 0.0, longitud = 0.0;
DateTime now;

// -------- CONFIGURACIÓN INICIAL --------
void setup() {
  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Wire.begin(21, 22);

  dht.begin();
  sensors.begin();

  // --- Configuración del RTC ---
  if (!rtc.begin()) {
    Serial.println("No se pudo encontrar el RTC DS3231");
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC perdió energía, se ajusta la hora actual de compilación.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("\n--- NODO AGRÍCOLA INICIADO (Versión 3) ---");
  delay(2000);
}

// -------- BUCLE PRINCIPAL --------
void loop() {
  leerSensores();
  leerGPS();
  mostrarDatos();
  delay(5000); // Cada 5 segundos
}

// -------- FUNCIONES --------

// Lectura de sensores
void leerSensores() {
  // DHT22
  tempAire = dht.readTemperature();
  humAire = dht.readHumidity();

  // DS18B20
  sensors.requestTemperatures();
  temperatureC = sensors.getTempCByIndex(0);

  // Sensor capacitivo (ADC → %)
  sensorValue = analogRead(SENSOR_PIN); // Lee el valor ADC (0-4095)
  
  // Convertir a porcentaje (inverso: más agua -> menor resistencia -> mayor lectura)
  humedad = map(sensorValue, 4095, 0, 0, 100);
  
  // RTC
  now = rtc.now();
}

// Lectura del GPS
void leerGPS() {
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
    if (gps.location.isUpdated()) {
      latitud = gps.location.lat();
      longitud = gps.location.lng();
    }
  }
}

// Mostrar datos en formato JSON
void mostrarDatos() {
  Serial.println("==================================");
  Serial.print("Fecha/Hora: ");
  Serial.print(now.day()); Serial.print("/");
  Serial.print(now.month()); Serial.print("/");
  Serial.print(now.year()); Serial.print(" ");

  // --- Formato 12 horas con AM/PM ---
  int hour12 = now.hour();
  String ampm = "am";
  if (hour12 == 0) {
    hour12 = 12;
  } else if (hour12 == 12) {
    ampm = "pm";
  } else if (hour12 > 12) {
    hour12 -= 12;
    ampm = "pm";
  }

  if (hour12 < 10) Serial.print("0");
  Serial.print(hour12); Serial.print(":");
  if (now.minute() < 10) Serial.print("0");
  Serial.print(now.minute()); Serial.print(":");
  if (now.second() < 10) Serial.print("0");
  Serial.print(now.second());
  Serial.print(" ");
  Serial.println(ampm);

  Serial.println("{");
  Serial.printf("  \"TempAire\": %.2f,\n", tempAire);
  Serial.printf("  \"HumAire\": %.2f,\n", humAire);
  Serial.printf("  \"TempSuelo\": %.2f,\n", temperatureC); 
  Serial.printf("  \"HumedadSuelo\": %.1f,\n", humedad); // Porcentaje 0–100 %
  Serial.printf("  \"Latitud\": %.6f,\n", latitud);
  Serial.printf("  \"Longitud\": %.6f\n", longitud);
  Serial.println("}");
  Serial.println("==================================");
}
