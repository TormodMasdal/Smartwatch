#include "Room_Reading_View.h"
#include "Room_Reading.h"

void Room_Reading_View::Print_Humidity(){
    Room_Reading ReadHum;
    float humidity = ReadHum.Humidity_Reading();
    printf("Humidity: %2.f\n", humidity);
}

void Room_Reading_View::Print_Temp(){
    Room_Reading ReadTemp;
    float temperature = ReadTemp.Temp_Reading();
    printf("Temperature: %2.f\n", temperature);
}