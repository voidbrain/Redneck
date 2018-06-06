#ifndef PumpWaterLoop_h
#define PumpWaterLoop_h

#include "Arduino.h"
#include "StandardCplusplus.h"
#include <vector>
#include <iterator>

using namespace std;
namespace std{}

struct PumpWaterLoopScheduledJobs {
    byte fromHour;
    byte fromMin;
    byte toHour;
    byte toMin;
};

class PumpWaterLoop {
    public:
        PumpWaterLoop(byte port = 0);
        void Setup(byte port = 0, byte setup_priority = 0);
        byte Identify();
        void start();
        void stop();
        void Loop();
        byte GetJobLevel();
        bool Schedule(byte fromHour, byte fromMin, byte toHour, byte toMin);
        bool GetStatus();
        void SetStatus(bool);
        bool GetSchedule();
        vector<PumpWaterLoopScheduledJobs> tasks;     
    private:
        bool status;
        byte port;
        byte priority;
        
};
#endif