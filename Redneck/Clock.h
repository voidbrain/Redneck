#ifndef Clock_h
#define Clock_h

#include "Arduino.h"
#include "RTClib.h"

struct moment
{
  byte seconds;
  byte minutes;
  byte hours;
};

class Clock {
    public:
        Clock();
        void begin();
        void loop();
        void writeTime();
        void setClock( unsigned long epoch );
        void Clock::syncTime(byte packetBuffer[40]);
        byte currHour; 
        byte currMin;
        byte currSec;
        bool isNight();
        void setNightZone(byte fromHour, byte fromMin, byte toHour, byte toMin);
        void computeTimeDifference(struct moment, struct moment, struct moment, struct moment *);
    private:
        bool is_night;
};
#endif
