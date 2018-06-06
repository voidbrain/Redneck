#ifndef ProbeWLev_h
#define ProbeWLev_h

#include "Arduino.h"
#include "StandardCplusplus.h"
#include <vector>
#include <iterator>
#include "ProbeT.h"

using namespace std;
namespace std{}

struct ProbeWLevScheduledJobs {
    byte atMin;
    bool done;
};

class ProbeWLev {
    public:
        ProbeWLev();
        void Setup(byte probeWLev_port = 0, byte setup_priority = 0);
        bool Schedule( byte atMin);
        bool GetSchedule();
        bool readLev();
        byte GetJobLevel();
        byte Identify();
        bool GetStatus();
        void SetStatus(bool);
        void displayItem();
        vector<ProbeWLevScheduledJobs> tasks;
    private:
        byte probeWLev_port;
        byte priority;
        bool status;
};
#endif