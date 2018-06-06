#include "ProbeWLev.h"
#ifdef ProbeWLev_h

ProbeWLev::ProbeWLev(){}

void ProbeWLev::Setup(byte pin, byte setup_priority){
    probeWLev_port = pin;
    priority = setup_priority;
}

bool ProbeWLev::Schedule( byte atMin){
    tasks.push_back({ atMin, false});
    return true;
}

bool ProbeWLev::GetStatus(){
    return status;
}

byte ProbeWLev::GetJobLevel(){
    return priority;
}

void ProbeWLev::SetStatus(bool stat){
    status = stat;
}

bool ProbeWLev::readLev(){
}

byte ProbeWLev::Identify(){
    return probeWLev_port;
}

bool ProbeWLev::GetSchedule(){
    for (byte j = 0; j != tasks.size(); j++) {
        Serial.print(" at "); Serial.print(":"); Serial.print(tasks[j].atMin);
        Serial.println();
    }
    return true;
}
#endif