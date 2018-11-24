/*
 * Send Sensor data with HTTP POST
 * Humedity Sensor (DHT11)
 * Sensor conected to ESP8266 pin GPI02
 * Created November 23th, 2018
 * By Juan Jose Romero Mariscal
 * 
 */

// Librery for JSON
#include <ArduinoJson.h>

// Libreries for ESP2866 WiFi module
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

// Librery for DHT sensor
#include <DHT.h>

// Define name for Serial
#define USE_SERIAL Serial
// Define pin where sensor DHT conected (Remember the conection is to GPI02 in ESP8266 module)
#define DHTPIN 2
// Define DHT sensor type (In this case DHT11)
#define DHTTYPE DHT11

// Declare a new ESP9266WIFiMulti object
ESP8266WiFiMulti WiFiMulti;

// Declare a new DHT object
DHT dht(DHTPIN, DHTTYPE);

// Setting
void setup() {

    // Init Serial speed
    USE_SERIAL.begin(9600);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    // Add network to WiFiMulti
    WiFiMulti.addAP("SSID", "PASSWORD");
    
    // Init DHT
    dht.begin();
}

// Creating function for read temperature
float temperature(){
  // Reading temperature in Celcius degrees
  float t = dht.readTemperature();
  //dht.readTemperature(True) for Farenheit degrees
  return t;
}


void loop(){
    // Wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {
        HTTPClient http;
        USE_SERIAL.print("[HTTP] begin...\n");
        // Configure traged server and url
        // In this case the server is local
        http.begin("http://192.168.1.69:5000/temperature"); //HTTP
        // Adding headers
        http.addHeader("Authorization", "Basic YWRtaW46cGFzc3dvcmQ=");
        http.addHeader("Content-Type", "application/json");
        
        USE_SERIAL.print("[HTTP] POST...\n");
        
        //Declare JSON
        StaticJsonBuffer<200> jsonBuffer;
        char json[256];
        JsonObject& root = jsonBuffer.createObject();

        // Reading temperature and adding to JSON
        root["temperature"] = temperature();
        root.printTo(json, sizeof(json));
        Serial.print(json);
 
        // Start connection, adding json and send HTTP header
        int httpCode = http.POST(json);
        
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode);

            // File found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        // Ending conection
        http.end();
    }
    delay(10000);
}
