#include "Fan.h"
#ifdef Fan_h

Fan::Fan(Organizer* organizer, Clock& clock, byte pin, bool enabled) : _organizer(organizer), _clock(clock), fan_port(pin), fan_enabled(enabled) {}

void Fan::Setup(byte pin, byte setup_priority, bool enabled){
    fan_port = pin;
    fan_enabled = enabled;
    priority = setup_priority;
}

byte Fan::GetJobLevel(){
    return priority;
}

bool Fan::Schedule(byte fromHour, byte fromMin, byte toHour, byte toMin){
    tasks.push_back({fromHour, fromMin, toHour, toMin});
    return true;
}

bool Fan::GetStatus(){
    return status;
}

byte Fan::Identify(){
    return fan_port;
}

bool Fan::GetSchedule(){
    for (byte j = 0; j != tasks.size(); j++) {
        Serial.print(" from ");Serial.print(tasks[j].fromHour); Serial.print(":"); Serial.print(tasks[j].fromMin);Serial.print(" - ");
        Serial.print("to ");Serial.print(tasks[j].toHour); Serial.print(":"); Serial.print(tasks[j].toMin);
        
        Serial.println();
    }
    return true;
}

void Fan::Loop() {
    if(priority <= (_organizer->settings.work_mode - _organizer->is_night)){
        for (byte j = 0; j != tasks.size(); j++) {
            if((_clock.currHour == tasks[j].fromHour)&&(_clock.currMin==tasks[j].fromMin)&&(fan_enabled==true && status != true)){
                status = true;
                Serial.println("fan status changed to true");
            }
            if((_clock.currHour == tasks[j].toHour)&&(_clock.currMin==tasks[j].toMin)&&(fan_enabled==false && status != false)){
                status = false;
                Serial.println("fan status changed to false");
            }
        }  
    }
}
#endif
