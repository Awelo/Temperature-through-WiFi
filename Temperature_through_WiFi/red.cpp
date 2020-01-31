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

const size_t capacity = JSON_OBJECT_SIZE(7);

void red_setup(){

  for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.flush();
        delay(1000);
    }
    WiFiMulti.addAP("Reino Geek", "Cogito, ergo sum");
  
}


byte red_send_data(float temperature_c, float temperature_f,float humidity, float indexHeat_c, float indexHeat_f, float particles_pm10, float particles_pm25){     
    byte exito=0;
    if(WiFiMulti.run() == WL_CONNECTED) {
        HTTPClient http;
        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://167.172.231.230:3000/data"); //HTTP
        //http.begin("http://192.168.1.68:3000/data "); //HTTP
        
        http.addHeader("Content-Type", "application/json");
        
        //Declare JSON
        DynamicJsonDocument root(capacity);

        // Reading temperature and adding to JSON
        root["temperature_c"] = temperature_c;
        root["temperature_f"] = temperature_f;
        root["humidity"] = humidity;
        root["indexHeat_c"] = indexHeat_c;
        root["indexHeat_f"] = indexHeat_f;
        root["particles_pm10"] = particles_pm10;
        root["particles_pm25"] = particles_pm25;
        String datos="";
        serializeJson(root, datos);

        /*
         * {
         *  "temperature_c": 23.23,
         *  "temperature_f": 129.23,
         *  "humidity": 32,
         *  "indexHeat_c": 30.23,
         *  "indexHeat_f": 130.23,
         *  "particles_pm10": 23.23,
         *  "particles_pm25": 20.20
         * }
         * 
         */

         
        
        USE_SERIAL.print("[HTTP] POST...\n");
        // start connection and send HTTP header
        int httpCode = http.POST(datos);

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
                exito=1;
            }
        } else {
            //USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    return exito;
}


unsigned char evaluar(String rec){
  if(rec.charAt(0)=='I' && rec.charAt(1)=='N' && rec.charAt(2)=='I' && rec.charAt(3)=='T'){
    if(WiFiMulti.run()==WL_CONNECTED){
      return 1;
    }else{
      return 0;
    }
  }
  else if(rec[0]=='D' && rec[1]=='T'){
    return 2;
  }
  else{
    return 0;
  }
}


bool red_sacar_datos(float *temp_c, float *temp_f, float *hum, float *index_c, float *index_f, float *pm10, float *pm25, String datos){
  int ind=3;
  float conjun[7];
  unsigned char tam = datos.length();
  unsigned char prin[7];
  unsigned char fin[7];
  bool exito=false;
  unsigned char index=0;
  for(unsigned char x=0; x<tam; x++){
    if(datos.charAt(x)==','){
      prin[index]=x;
      index++;
    }
  }
    *temp_c=datos.substring(prin[0]+1,prin[1]).toFloat();
    *temp_f=datos.substring(prin[1]+1,prin[2]).toFloat();
    *hum=datos.substring(prin[2]+1,prin[3]).toFloat();
    *index_c = datos.substring(prin[3]+1,prin[4]).toFloat();
    *index_f=datos.substring(prin[4]+1, prin[5]).toFloat();
    *pm10=datos.substring(prin[5]+1,prin[6]).toFloat();
    *pm25=datos.substring(prin[6]+1).toFloat();
  return 1;
}
