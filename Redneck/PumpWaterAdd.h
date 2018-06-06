#ifndef PumpWaterAdd_h
#define PumpWaterAdd_h

#include "Arduino.h"
#include "StandardCplusplus.h"
#include <vector>
#include <iterator>

using namespace std;
namespace std{}

struct PumpWaterAddScheduledJobs {
    byte atHour;
    byte atMin;
};

class PumpWaterAdd {
    public:
        PumpWaterAdd();
        void Setup(byte port = 0, byte setup_priority = 0);
        byte Identify();
        void start();
         byte GetJobLevel();
        void stop();
        bool Schedule(byte atHour, byte atMin);
        vector<PumpWaterAddScheduledJobs> tasks;
        bool GetSchedule();
        void SetStatus(bool);
        bool GetStatus();

    private:
        byte port;
        bool status;
        byte priority;
};
#endif