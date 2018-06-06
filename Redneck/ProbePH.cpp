#include "ProbePH.h"
#ifdef ProbePH_h

ProbePH::ProbePH() {}

bool ProbePH::GetStatus(){
    return status;
}

void ProbePH::Setup(byte pin, byte setup_priority){
    probePH_port = pin;
    priority = setup_priority;
}

void ProbePH::SetStatus(bool stat){
    status = stat;
}

byte ProbePH::GetJobLevel(){
    return priority;
}

bool ProbePH::Schedule(byte atMin){
    tasks.push_back({ atMin, false});
    return true;
}

bool ProbePH::readPH(){
    byte measure = analogRead(probePH_port);
    Serial.println("port ");
    Serial.print(probePH_port);
    double voltage = 5 / 1024.0 * measure; //classic digital to voltage conversion
    Serial.println("Voltage: ");
    Serial.print(voltage, 3);
    float Po = 7 + ((3 - voltage) / 0.187);
    Serial.println("PH: ");
    Serial.print(Po, 3);
    return Po;
}

byte ProbePH::Identify(){
    return probePH_port;
}

bool ProbePH::GetSchedule(){
    for (byte j = 0; j != tasks.size(); j++) {
        Serial.print(" at ");Serial.print(":"); Serial.print(tasks[j].atMin);
        Serial.println();
    }
    return true;
}

#endif