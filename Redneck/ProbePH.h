#ifndef ProbePH_h
#define ProbePH_h

#include "Arduino.h"
#include "StandardCplusplus.h"
#include <vector>
#include <iterator>
#include "ProbeT.h"

class ProbeT;

using namespace std;
namespace std{}

struct ProbePHScheduledJobs {
    
    byte atMin;
    bool done;
};

class ProbePH {
    public:
        ProbePH();
        void Setup(byte probePH_port = 0, byte setup_priority = 0);
        bool Schedule(byte atMin);
        bool GetSchedule();
        byte GetJobLevel();
        bool GetStatus();
        void SetStatus(bool);
        bool readPH();
        byte Identify();
        void displayItem();
        vector<ProbePHScheduledJobs> tasks;
    private:
        byte probePH_port;
        bool status;
        byte priority;
};
#endif