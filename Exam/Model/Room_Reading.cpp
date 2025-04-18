#include "Room_Reading.h"

float Room_Reading::Temp_Reading(){
    
    float temperature = 0.0f;
    hts221.get_temperature(&temperature);

    // CALIBRATE TEMPERATURE
    temperature -= 5;

    return temperature;
}

float Room_Reading::Humidity_Reading(){

    float humidity = 0.0f;
    hts221.get_humidity(&humidity);

    return humidity;
}