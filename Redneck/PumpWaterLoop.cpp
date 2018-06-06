#include "PumpWaterLoop.h"
#ifdef PumpWaterLoop_h

PumpWaterLoop::PumpWaterLoop(byte pin) : port(pin) {}

void PumpWaterLoop::Setup(byte pin, byte setup_priority){
    port = pin;
    priority = setup_priority;
}

byte PumpWaterLoop::Identify(){
    return port;
}

void PumpWaterLoop::start(){

}
void PumpWaterLoop::stop(){
    
}

byte PumpWaterLoop::GetJobLevel(){
    return priority;
}

bool PumpWaterLoop::GetStatus(){
    return status;
}

void PumpWaterLoop::SetStatus(bool stat){
    status = stat;
}

bool PumpWaterLoop::Schedule(byte fromHour, byte fromMin, byte toHour, byte toMin){
    tasks.push_back({fromHour, fromMin, toHour, toMin});
    return true;
}

bool PumpWaterLoop::GetSchedule(){
    for (byte j = 0; j != tasks.size(); j++) {
        Serial.print(" from ");Serial.print(tasks[j].fromHour); Serial.print(":"); Serial.print(tasks[j].fromMin);Serial.print(" - ");
        Serial.print("to ");Serial.print(tasks[j].toHour); Serial.print(":"); Serial.print(tasks[j].toMin);
        Serial.println();
    }
    return true;
}
#endif