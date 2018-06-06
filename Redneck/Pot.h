#ifndef Pot_h
#define Pot_h

#include "Arduino.h"
#include "Clock.h"
#include "ProbeEC.h"
#include "ProbePH.h"
#include "ProbeT.h"
#include "ProbeWLev.h"
#include "PumpWaterLoop.h"
#include "PumpWaterAdd.h"
#include "PumpFert.h"
#include "Organizer.h"
#include <algorithm>

struct PotDoses {
    byte id_dose;    
    float grow;       
    float micro;      
    float bloom;      
    float ripen;          
    float EC;
};
using namespace std;

class Organizer;

class Pot {
    public:
        Pot(Organizer* organizer, Clock& clock, byte pot_id = 0, unsigned long pot_day_start = 0);
        void Setup(byte pot_id = 0, bool pot_enabled = false, unsigned long pot_day_start = 0);
        void Loop();
        byte Identify();
        byte getWeeksAge();
        bool GetEquivalentDoseId();
        bool GetEquivalentDoses();
        ProbeT probeT;
        ProbeEC probeEC;
        ProbePH probePH;
        ProbeWLev probeWLev;
        PumpWaterLoop pumpWaterLoop;
        PumpWaterAdd pumpWaterAdd;
        PumpFert pumpFert;
    private:
        byte id;
        byte n_weeks;
        byte id_dose;
        PotDoses dose;
        bool enabled;
        unsigned long day_start;
        Clock& _clock;      
        Organizer* _organizer; 
};
#endif