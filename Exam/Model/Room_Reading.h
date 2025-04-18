#ifndef ROOM_READING_H
#define ROOM_READING_H

#include "HTS221Sensor.h"
#include "mbed.h"

// GATHER ROOM DATA
class Room_Reading
{

private:
    I2C i2c_sensor;
    DevI2C devI2C;
    HTS221Sensor hts221;

public: 
    float Temp_Reading();   
    float Humidity_Reading();
    
    // INITIALIZING SENSOR CLASSES AND SENSOR
    Room_Reading() : 
        i2c_sensor(PB_11, PB_10),
        devI2C(PB_11, PB_10),
        hts221(&devI2C)
    {
        hts221.init(NULL);
        hts221.enable();
    }

};

#endif