#ifndef ProbeT_h
#define ProbeT_h

#include "Arduino.h"
#include "StandardCplusplus.h"
#include <vector>
#include <iterator>
#include <DallasTemperature.h>
#include <OneWire.h>

using namespace std;
namespace std{}

struct ProbeTScheduledJobs {
   byte atMin;
    bool done;
};

class ProbeT {
    friend class ProbeEC;

    public:
        ProbeT();
        void Setup(byte probeT_port = 0, byte setup_priority = 0, byte vcc = 0, byte gnd = 0);
        bool Schedule(byte atMin);
        bool GetSchedule();
        bool readT();
        byte GetJobLevel();
        bool GetStatus();
        byte Identify();
        void SetStatus(bool);
        void printAddress(DeviceAddress deviceAddress);
        vector<ProbeTScheduledJobs> tasks;
    private:
        byte probeT_port;
        DallasTemperature sensors;
        byte vcc;
        byte gnd;
        bool status;
        byte priority;
};
#endif