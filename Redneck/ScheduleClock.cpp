#include "ScheduleClock.h"
#ifdef ScheduleClock_h

ScheduleClock::ScheduleClock(Organizer* organizer, Clock& clock, byte setup_priority) : _organizer(organizer), _clock(clock), priority(setup_priority) {}

bool ScheduleClock::Schedule(byte atHour, byte atMin){
    tasks.push_back({atHour, atMin, false});
    return true;
}

bool ScheduleClock::GetSchedule(){
    for (byte j = 0; j != tasks.size(); j++) { 
        Serial.print(" at ");Serial.print(tasks[j].atHour); Serial.print(":"); Serial.print(tasks[j].atMin);
        Serial.println();
    }
    return true;
}

byte ScheduleClock::GetJobLevel(){
    return priority;
}

void ScheduleClock::Loop() {
    if(priority <= (_organizer->settings.work_mode - _organizer->is_night)){
        for (byte j = 0; j != tasks.size(); j++) {
            if((_clock.currHour== tasks[j].atHour)&&(_clock.currMin==tasks[j].atMin)&&(tasks[j].done != true)){
                bool jobDone = _organizer->sendRequestsToWebServer("TIME");
                if(jobDone){
                    bool jobReply = _organizer->sendRequestsToWebServer("CONFIRM");
                    tasks[j].done = true;
                }
            }
            if((_clock.currHour== tasks[j].atHour)&&(_clock.currMin==tasks[j].atMin+30)&&(tasks[j].done != false)){
                tasks[j].done = false;
            }
        }
    }
}
#endif