#ifndef ProbeEC_h
#define ProbeEC_h

#include "Arduino.h"
#include "StandardCplusplus.h"
#include <vector>
#include <iterator>
#include "ProbeT.h"
#include <DallasTemperature.h>
#include <OneWire.h>

class ProbeT;

using namespace std;
namespace std{}

struct ProbeECScheduledJobs {

    byte atMin;
    bool done;
};

class ProbeEC {
    public:
        ProbeEC();
        void Setup(byte pin=0, byte setup_priority = 0, byte ecgnd=0, byte ecvcc=0, byte tvcc=0, byte tgnd=0);
        bool connect(ProbeT* probeT);
        bool Schedule(byte atMin);
        bool GetStatus();
        bool GetSchedule();
        void SetStatus(bool);
        byte GetJobLevel();
        bool readEC();
        byte Identify();
        void displayItem();
        void GetEC();
        void PrintReadings();
        void calibrate();
        vector<ProbeECScheduledJobs> tasks;
    private:
        byte probeEC_port;
        byte priority;
        bool status;
        ProbeT* _probeT;
        DallasTemperature sensors;
        float Temperature = 10;
        byte R1 = 510;
        byte Ra = 25; //Resistance of powering Pins
        byte ECPin;
        byte ECGround;
        byte ECPower;
        float EC = 0;
        float EC25 = 0;
        byte ppm = 0;
        float raw = 0;
        float Vin = 5;
        float Vdrop = 0;
        float Rc = 0;
        float buffer = 0;
        byte TempProbeNegative;
        byte TempProbePositive;

        //*********** Converting to ppm [Learn to use EC it is much better**************//
        // Hana      [USA]        PPMconverion:  0.5
        // Eutech    [EU]          PPMconversion:  0.64
        //Tranchen  [Australia]  PPMconversion:  0.7
        // Why didnt anyone standardise this?
        float PPMconversion = 0.7;
        //*************Compensating for temperature ************************************//
        //The value below will change depending on what chemical solution we are measuring
        //0.019 is generaly considered the standard for plant nutrients [google "Temperature compensation EC" for more info
        float TemperatureCoef = 0.019; //this changes depending on what chemical we are measuring
        //********************** Cell Constant For Ec Measurements *********************//
        //Mine was around 2.9 with plugs being a standard size they should all be around the same
        //But If you get bad readings you can use the calibration script and fluid to get a better estimate for K
        //float K = 2.88; // valore dell'esempio
        //float K = 0.93; // valore calcolato da calibration 
        //float K = 1.21; // vaolore empirico su liquido di controllo. K scende Ec sale
        float Kempiric = 1;



        // CALIBRATION
        float CalibrationEC = 1.413; //EC value of Calibration solution is s/cm
        float TemperatureFinish = 0;
        float TemperatureStart = 0;
        
        float Kcalibration = 0;
};
#endif