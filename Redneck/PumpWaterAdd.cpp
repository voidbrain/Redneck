#include "PumpWaterAdd.h"
#ifdef PumpWaterAdd_h

PumpWaterAdd::PumpWaterAdd(){}

void PumpWaterAdd::Setup(byte pin, byte setup_priority){
    port = pin;
    priority = setup_priority;
}

byte PumpWaterAdd::Identify(){
    return port;
}

void PumpWaterAdd::start(){

}
void PumpWaterAdd::stop(){
    
}

byte PumpWaterAdd::GetJobLevel(){
    return priority;
}

bool PumpWaterAdd::GetStatus(){
    return status;
}

void PumpWaterAdd::SetStatus(bool stat){
    status = stat;
}

bool PumpWaterAdd::Schedule(byte atHour, byte atMin){
    tasks.push_back({atHour, atMin});
    return true;
}

bool PumpWaterAdd::GetSchedule(){
    for (byte j = 0; j != tasks.size(); j++) {
        Serial.print(" at ");Serial.print(tasks[j].atHour); Serial.print(":"); Serial.print(tasks[j].atMin);Serial.print(" - ");
        Serial.println();
    }
    return true;
}
#endif