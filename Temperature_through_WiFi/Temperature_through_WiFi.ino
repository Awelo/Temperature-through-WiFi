/*
 * Send Sensor data with HTTP POST
 * Humedity Sensor (DHT11)
 * Sensor conected to ESP8266 pin GPI02
 * Created November 23th, 2018
 * By Juan Jose Romero Mariscal
 * 
 */

#include "red.h"


#define USE_SERIAL Serial
// Setting
void setup() {

    // Init Serial speed
    USE_SERIAL.begin(9600);
    USE_SERIAL.setTimeout(10);
    red_setup();
}


void loop(){
  
    // Wait for Data
    String recibido = "";
    unsigned char response=0;
    if(USE_SERIAL.available()>0){
      recibido = USE_SERIAL.readStringUntil('\n');
      response = evaluar(recibido);
      delay(100);
      if(!recibido.equals("")){
        if(response==1){
          USE_SERIAL.println("OK");
        }
        else if (response==2){
          float temp_c = 0;
          float temp_f = 0;
          float hum = 0;
          float index_c = 0;
          float index_f = 0;
          float pm10 = 0;
          float pm25 = 0;
          
          if(red_sacar_datos(&temp_c, &temp_f, &hum, &index_c, &index_f, &pm10, &pm25, recibido)){
            red_send_data(temp_c, temp_f, hum, index_c, index_f, pm10, pm25);
            USE_SERIAL.println("SENDOK");   
          }else{
            USE_SERIAL.println("FALL");
          }
        }
        else{
          USE_SERIAL.println("COMMAND");
        }
      }
    }
}
