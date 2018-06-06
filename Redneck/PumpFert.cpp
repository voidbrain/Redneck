#include "PumpFert.h"
#ifdef PumpFert_h

PumpFert::PumpFert(){}

void PumpFert::Setup(byte pin, byte setup_priority){
    pumpFert_port = pin;
    priority = setup_priority;
}

byte PumpFert::Identify(){
    return pumpFert_port;
}

bool PumpFert::GetStatus(){
    return status;
}

byte PumpFert::GetJobLevel(){
    return priority;
}

void PumpFert::SetStatus(bool stat){
    status = stat;
}

bool PumpFert::Schedule(byte atHour, byte atMin){
    tasks.push_back({atHour, atMin, false});
    return true;
}

bool PumpFert::GetSchedule(){
    for (byte j = 0; j != tasks.size(); j++) {
        Serial.print(" at ");Serial.print(tasks[j].atHour); Serial.print(":"); Serial.print(tasks[j].atMin);
        Serial.println();
    }
    return true;
}

void start(){

}
void stop(){
    
}
#endif