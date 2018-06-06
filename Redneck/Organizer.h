#ifndef Organizer_h
#define Organizer_h 

#include "Arduino.h"
#include "ArduinoJson.h"
#include "StandardCplusplus.h"
#include <vector>
#include "Clock.h"
#include "Light.h"
#include "Fan.h" 
#include "Pot.h"
#include "PumpFert.h"
#include "ScheduleClock.h"

class ScheduleClock;
class Pot;
class Light;
class Fan;
class ProbeT;
class ProbePH;
class ProbeEC;
class PumpFert;

struct Settings {
    unsigned long date_time;
    bool store_data;
    byte phvcc;
    byte phgnd;
    byte tvcc;
    byte tgnd;
    byte ecvcc;
    byte ecgnd;
    byte work_mode;
    byte night_mode_on;
    byte night_mode_off;

};
struct Calendar {
    byte week_n;
    byte id_dose;
};
struct Doses {
    byte id_dose;    
    float grow;       
    float micro;      
    float bloom;      
    float ripen;          
    float EC;
};

class Organizer {
    public:
        Organizer(Clock& clock, String json_text = "");
        bool schedule(String json_text = "");
        void Loop();
        void GetSchedule();
        void GetCalendar();
        void GetDoses();
        bool begin(Stream &strean = Serial1);
        //void checkNightMode();
        void checkWorkingMode();
        void printWifiStatus();
        bool sendRequestsToWebServer(char* action = "", String value = "");
        Settings settings;
        Calendar addCalendar(byte week_n, byte id_dose);
        Doses addDose(byte id_dose, float grow, float micro, float bloom, float ripen, float EC);
        vector<Doses> doses;
        vector<Calendar> calendar;
        byte WORKING_MODE_FULL = 5;
        byte WORKING_MODE_ON = 4;
        byte WORKING_MODE_SILENT = 3;
        byte WORKING_MODE_SUPER_SILENT = 2;
        byte WORKING_MODE_OFF = 1;
        bool is_night = 0;
    private:
        Stream *_stream;
       	Clock& _clock;
        vector<Light> lights;
        vector<Fan> fans;
        vector<Pot> pots;
        vector<ScheduleClock> scheduleClocks;
        
};
#endif