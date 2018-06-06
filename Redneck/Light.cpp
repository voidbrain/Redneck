#include "Light.h"
#ifdef Light_h

Light::Light(Organizer* organizer, Clock& clock, byte pin, bool enabled) : _organizer(organizer), _clock(clock), light_port(pin), light_enabled(enabled) {}

void Light::Setup(byte pin, byte setup_priority, bool enabled){
    light_port = pin;
    light_enabled = enabled;
    priority = setup_priority;
}

byte Light::GetJobLevel(){
    return priority;
}


bool Light::Schedule(byte fromHour, byte fromMin, byte toHour, byte toMin){
    tasks.push_back({fromHour, fromMin, toHour, toMin});
    return true;
}

bool Light::GetStatus(){
    return status;
}

byte Light::Identify(){
    return light_port;
}

bool Light::GetSchedule(){
    for (byte j = 0; j != tasks.size(); j++) {
        Serial.print(" from ");Serial.print(tasks[j].fromHour); Serial.print(":"); Serial.print(tasks[j].fromMin);Serial.print(" - ");
        Serial.print("to ");Serial.print(tasks[j].toHour); Serial.print(":"); Serial.print(tasks[j].toMin);
        Serial.println();
    }
    return true;
}

void Light::Loop() {
    if(priority <= (_organizer->settings.work_mode - _organizer->is_night)){
        for (byte j = 0; j != tasks.size(); j++) {
            if((_clock.currHour== tasks[j].fromHour)&&(_clock.currMin==tasks[j].fromMin)&&(light_enabled==true && status != true)){
                status = true;
            }
            if((_clock.currHour== tasks[j].toHour)&&(_clock.currMin==tasks[j].toMin)&&(light_enabled==false && status != false)){
                status = false;
            }
        } 
    }
}
#endif