/*
  Backend Proyecto IoT - Monitoreo Climático (Pasto, Nariño)
  Funciones: 
    - doPost: recibe datos del ESP32 + API.
    - fetchApiDataOnly: ejecuta la consulta API sin el ESP32 (PARA EJECUCIÓN MANUAL).
  Autor: Énfasis 2
*/

// =====================================================
// 1. CONFIGURACIÓN GLOBAL
// =====================================================
// *** ¡ATENCIÓN! REEMPLAZA ESTE ID con el ID de TU hoja de cálculo ***
var ID_HOJA = "1j-cqReINek2gllWAopJKMUmSFpJiBqFM-dtAaAGLXrE"; 
var ID_CARPETA = ""; 

// Coordenadas de PASTO, NARIÑO
var LATITUD = 1.2136;
var LONGITUD = -77.2811;
var API_KEY_OWM = "637b6c96194459c872185532208f47d1"; 

// =====================================================
// FUNCIÓN PRINCIPAL: RECIBE DATOS DEL ESP32 (doPost)
// =====================================================
function doPost(e) {
  try {
    var sheet = SpreadsheetApp.openById(ID_HOJA).getSheets()[0];
    var data = JSON.parse(e.postData.contents);
    
    // --- DATOS LOCALES (ESP32) ---
    var fecha = data.fecha || new Date();
    var tSens = data.temp;
    var hSens = data.hum;
    var suelo = data.suelo;
    
    // --- DATOS API EXTENDIDOS ---
    var api = obtenerClimaExtendidoOWM(LATITUD, LONGITUD, API_KEY_OWM);

    // --- FOTO ---
    var imgLink = "Sin Imagen"; 
    var imgName = "";

    if (data.fotoBase64 && data.fotoBase64.length > 100) {
      var folder;
      if (ID_CARPETA === "") {
        folder = DriveApp.getRootFolder();
      } else {
        try { folder = DriveApp.getFolderById(ID_CARPETA); } 
        catch(err) { folder = DriveApp.getRootFolder(); }
      }

      var decodedImage = Utilities.base64Decode(data.fotoBase64);
      var cleanDate = String(fecha).replace(/:/g, "-").replace(/ /g, "_");
      var fileName = "IMG_" + cleanDate + ".jpg";
      
      var blob = Utilities.newBlob(decodedImage, MimeType.JPEG, fileName);
      var file = folder.createFile(blob);
      file.setSharing(DriveApp.Access.ANYONE_WITH_LINK, DriveApp.Permission.VIEW);
      
      imgLink = file.getUrl();
      imgName = fileName;
    }
    
    // --- SEPARACIÓN DE FECHA/HORA PARA CONSISTENCIA ---
    var dateObj = new Date(fecha); 
    // Aseguramos la zona horaria GMT-5 (Colombia)
    var soloFecha = Utilities.formatDate(dateObj, "GMT-5", "yyyy-MM-dd");
    var soloHora = Utilities.formatDate(dateObj, "GMT-5", "HH:mm:ss");

    // --- GUARDADO FINAL ---
    // A:FechaCompleta, B:T.Sen, C:H.Sen, D:Suelo, E:Link, F:Nombre, G:T.API ... N:Lluvia, O:SoloFecha, P:SoloHora
    sheet.appendRow([
      fecha, 
      tSens, hSens, suelo, 
      imgLink, imgName,
      api.temp, api.hum, api.desc, 
      api.feels_like, api.pressure, api.wind_speed, api.clouds, api.rain_1h,
      soloFecha,
      soloHora
    ]);

    return ContentService.createTextOutput("Exito: Datos + OWM integrados para Pasto");

  } catch (error) {
    Logger.log("Error en doPost: " + error.message);
    return ContentService.createTextOutput("Error Backend: " + error.message);
  }
}

// =======================================================
// NUEVA FUNCIÓN: INSERCIÓN MANUAL DE DATOS API
// =======================================================
function fetchApiDataOnly() {
  var sheet = SpreadsheetApp.openById(ID_HOJA).getSheets()[0];
  
  Logger.log("Ejecutando fetchApiDataOnly...");

  // --- DATOS API EXTENDIDOS ---
  var api = obtenerClimaExtendidoOWM(LATITUD, LONGITUD, API_KEY_OWM);
  
  // --- PLACEHOLDERS PARA DATOS DEL SENSOR/FOTO ---
  var fechaActual = new Date();
  
  var tSens_NA = "NA_API_ONLY"; 
  var hSens_NA = "NA_API_ONLY";
  var suelo_NA = "NA_API_ONLY";
  
  var imgLink_NA = "NA_API_ONLY";
  var imgName_NA = "NA_API_ONLY";
  
  // --- SEPARACIÓN DE FECHA/HORA ---
  var soloFecha = Utilities.formatDate(fechaActual, "GMT-5", "yyyy-MM-dd");
  var soloHora = Utilities.formatDate(fechaActual, "GMT-5", "HH:mm:ss");

  // --- GUARDADO FINAL ---
  sheet.appendRow([
    fechaActual, 
    tSens_NA, hSens_NA, suelo_NA, 
    imgLink_NA, imgName_NA, 
    api.temp, api.hum, api.desc, 
    api.feels_like, api.pressure, api.wind_speed, api.clouds, api.rain_1h,
    soloFecha,
    soloHora
  ]);
  
  Logger.log("Fila de datos solo API insertada correctamente para Pasto.");
}


// =======================================================
// FUNCIÓN AUXILIAR DE EXTRACCIÓN MASIVA (REUSADA)
// =======================================================
function obtenerClimaExtendidoOWM(lat, lon, key) {
  try {
    var url = "https://api.openweathermap.org/data/2.5/weather?lat=" + lat + "&lon=" + lon + "&units=metric&lang=es&appid=" + key;
    var response = UrlFetchApp.fetch(url);
    var json = JSON.parse(response.getContentText());
    
    // Mapeo seguro de variables
    var main = json.main || {};
    var wind = json.wind || {};
    var clouds = json.clouds || {};
    var weather = json.weather && json.weather[0] ? json.weather[0] : {};
    
    var rainVal = 0;
    if (json.rain && json.rain["1h"]) {
      rainVal = json.rain["1h"];
    }

    return {
      temp: main.temp,              
      hum: main.humidity,           
      desc: weather.description,    
      feels_like: main.feels_like,  
      pressure: main.pressure,      
      wind_speed: wind.speed,       
      clouds: clouds.all,           
      rain_1h: rainVal              
    };
    
  } catch (e) {
    Logger.log("Error API: " + e.message);
    return { 
      temp: "Err", hum: "Err", desc: e.message, 
      feels_like: "Err", pressure: "Err", wind_speed: "Err", clouds: "Err", rain_1h: "Err"
    };
  }
}