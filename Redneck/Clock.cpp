#include "Clock.h"
#ifdef Clock_h

RTC_DS1307 RTC;
byte currHour, currMin, currSec;
//unsigned long epoch;
//byte lastTimeSet = 0;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

Clock::Clock() {}

void Clock::loop(){
    DateTime now = RTC.now();
    currHour = now.hour();
    currMin = now.minute();
    currSec = now.second();
}

void Clock::begin(){
    RTC.begin();
    Serial.println("RTC begin");
    if (! RTC.isrunning()) { Serial.println("RTC NOT running! RESET REQUIRED"); }
}

bool Clock::isNight(){
    return is_night;
}

void Clock::setNightZone(byte fromHour, byte fromMin, byte toHour, byte toMin){
    //_clock.currHour
    //_clock.currMin
    if(1){
        is_night = true;
    }else{
        is_night = false;
    }
}

void Clock::computeTimeDifference(struct moment t1, struct moment t2, struct moment t3, struct moment *difference){
    
    if(t2.seconds > t1.seconds)
    {
        --t1.minutes;
        t1.seconds += 60;
    }

    difference->seconds = t1.seconds - t2.seconds;
    if(t2.minutes > t1.minutes)
    {
        --t1.hours;
        t1.minutes += 60;
    }
    difference->minutes = t1.minutes-t2.minutes;
    difference->hours = t1.hours-t2.hours;
}

void Clock::writeTime(){
    DateTime now = RTC.now();
    Serial.println(
        String(now.year(), DEC) + '-' +
        String(now.month(), DEC) + '-' +
        String(now.day(), DEC) + " (" +
        daysOfTheWeek[now.dayOfTheWeek()] + ") " +
        String(now.hour(), DEC) + ':' +
        String(now.minute(), DEC) + ':' +
        String(now.second(), DEC)
    );
}

void Clock::setClock( unsigned long epoch ) {
    currHour = (((epoch  % 86400L) / 3600));
    currMin = ((epoch % 3600) / 60);
    currSec = (epoch % 60);
    DateTime now = RTC.now();
    RTC.adjust(DateTime(now.year(), now.month(), now.day(), currHour, currMin, currSec));
}
#endif