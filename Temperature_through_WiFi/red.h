
void red_setup();

byte red_send_data(float temperature_c, float temperature_f,float humidity, float indexHeat_c, float indexHeat_f, float particles_pm10, float particles_pm25);

unsigned char evaluar(String rec);

bool red_sacar_datos(float *temp_c, float *temp_f, float *hum, float *index_c, float *index_f, float *pm10, float *pm25, String datos);
