#ifndef Light_h
#define Light_h

#include "Arduino.h"
#include "StandardCplusplus.h"
#include <vector>
#include <iterator>
#include "Organizer.h"
#include "clock.h"

class Organizer;

using namespace std;
namespace std{}

struct LightScheduledJobs {
    byte fromHour;
    byte fromMin;
    byte toHour;
    byte toMin;
    bool enabled;
};

class Light {
    public:
        Light(Organizer* organizer, Clock& clock, byte light_port = 0, bool enabled = false );
        void Setup(byte light_port = 0, byte setup_priority = 0, bool enabled = false);
        void Loop();
        bool Schedule(byte fromHour, byte fromMin, byte toHour, byte toMin);
        bool GetStatus();
        bool GetSchedule();
        byte Identify();
        byte GetJobLevel();
        byte light_port;
        bool light_enabled;
        vector<LightScheduledJobs> tasks;
    private:
        bool status;
        byte priority;
        Clock& _clock;
        Organizer* _organizer;
};
#endif
