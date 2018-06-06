#ifndef Fan_h
#define Fan_h

#include "Arduino.h"
#include "StandardCplusplus.h"
#include <vector>
#include <iterator>
#include "Organizer.h"
#include "clock.h"

class Organizer;

using namespace std;
namespace std{}

struct FanScheduledJobs {
    byte fromHour;
    byte fromMin;
    byte toHour;
    byte toMin;
    bool enabled;
};

class Fan {
    public:
        Fan(Organizer* organizer, Clock& clock, byte fan_port = 0, bool enabled = false);
        void Setup(byte fan_port = 0, byte setup_priority = 0, bool enabled = false);
        void Loop();
        bool Schedule(byte fromHour, byte fromMin, byte toHour, byte toMin);
        bool GetStatus();
        bool GetSchedule();
        byte Identify();
        byte GetJobLevel();
        byte fan_port;
        bool fan_enabled;
        vector<FanScheduledJobs> tasks;
    private:
        byte priority;
        bool status;
        Clock& _clock;
        Organizer* _organizer;
};

#endif
