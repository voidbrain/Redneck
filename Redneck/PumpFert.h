#ifndef PumpFert_h
#define PumpFert_h

#include "Arduino.h"
#include "StandardCplusplus.h"
#include <vector>
#include <iterator>

using namespace std;
namespace std{}

struct PumpFertScheduledJobs {
    byte atHour;
    byte atMin;
    bool done;
};

class PumpFert {
    public:
        PumpFert();
        void Setup(byte pumpFert_port = 0, byte setup_priority = 0);
        bool Schedule(byte atHour, byte atMin);
        bool GetSchedule();
        bool GetStatus();
         byte GetJobLevel();
        byte Identify();
        void start();
        void SetStatus(bool);
        void stop();
        vector<PumpFertScheduledJobs> tasks;
    private:
        byte pumpFert_port;
        bool status;
        byte priority;
};
#endif