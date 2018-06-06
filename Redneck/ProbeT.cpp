
#include "ProbeT.h"
#ifdef ProbeT_h

ProbeT::ProbeT(){
    
}

bool ProbeT::GetStatus(){
    return status;
}

void ProbeT::SetStatus(bool stat){
    status = stat;
}

byte ProbeT::GetJobLevel(){
    return priority;
}

void ProbeT::Setup(byte pin, byte setup_priority, byte vcc, byte gnd){
    probeT_port = pin;
    priority = setup_priority;
    digitalWrite( vcc , LOW ); // VCC
    pinMode( vcc  , OUTPUT );
    digitalWrite( gnd , LOW ); // GND
    pinMode( gnd , OUTPUT );
    OneWire oneWire(probeT_port);
    DallasTemperature sensors(&oneWire);
    status = false;
}

bool ProbeT::Schedule( byte atMin){
    tasks.push_back({ atMin, false});
    return true;
}

bool ProbeT::readT(){
    #ifdef BUSFAIL
    Serial.print(" Test:");
    if (sensors.reset()) {
        Serial.print("good");
    } else {
        if (sensors.busFail()) {
        Serial.print("fail");
        } else {    
            Serial.print("empty");
        }
    }
    #endif 
    byte numberOfDevices; // Number of temperature devices found
    DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address    
    sensors.begin();
    // Grab a count of devices on the wire
    numberOfDevices = sensors.getDeviceCount();
    Serial.print(" Parasite:"); 
    if (sensors.isParasitePowerMode()) Serial.print("ON ");
    else Serial.print("OFF ");
    Serial.print("Count:");
    Serial.print(numberOfDevices, DEC);
    sensors.requestTemperatures(); // Send the command to get temperatures
    // Loop through each device, print out temperature data
    for(int i=0;i<numberOfDevices; i++){
        if(sensors.getAddress(tempDeviceAddress, i)){
        // Output the device ID
            Serial.print(" #");
            Serial.print(i,DEC);
            Serial.print("=");
            float tempC = sensors.getTempC(tempDeviceAddress);
            Serial.print(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
        } 
        //else ghost device! Check your power requirements and cabling
    }
    Serial.println("");
}

void ProbeT::printAddress(DeviceAddress deviceAddress){
    for (byte i = 0; i < 8; i++){
        if (deviceAddress[i] < 16) Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
}

byte ProbeT::Identify(){
    return probeT_port;
}

bool ProbeT::GetSchedule(){
    for (byte j = 0; j != tasks.size(); j++) {
        Serial.print(" at ");Serial.print(":"); Serial.print(tasks[j].atMin);
        Serial.println();
    }
    return true;
}
#endif