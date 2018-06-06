#ifndef ScheduleClock_h
#define ScheduleClock_h

#include "Arduino.h"
#include "StandardCplusplus.h"
#include <vector>
#include <iterator>
#include "Clock.h"
#include "Organizer.h"

class Organizer;

using namespace std;
namespace std{}

struct ScheduleClockScheduledJobs {
    byte atHour;
    byte atMin;
    bool done;
};

class ScheduleClock {
    public:
        ScheduleClock(Organizer* organizer, Clock& clock, byte setup_priority);
        bool Schedule(byte atHour, byte atMin);
        bool GetSchedule();
        byte GetJobLevel();
        void Loop();
        vector<ScheduleClockScheduledJobs> tasks;
    private:
        Clock& _clock;      
        Organizer* _organizer;
        byte priority;
};
#endif