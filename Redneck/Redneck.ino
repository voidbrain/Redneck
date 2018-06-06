#include "MemoryFree.h"
#include "Clock.h"
#include "Server.h"
#include "Organizer.h"

const long MAIN_LOOP_TIMEOUT = 5000;
unsigned long previousMillis= 0;

Clock clock;
Server webServer;
Organizer organizer(clock);
bool parseJSON = false;

void setup(){
  	Serial.begin(115200);
    Serial1.begin(115200);
    Serial2.begin(115200);

    organizer.begin(Serial1);
    webServer.begin(Serial2);
    clock.begin();
    
    parseJSON = organizer.sendRequestsToWebServer("SETUP");
    if(parseJSON) { Serial.println("parse SETUP OK"); }
    parseJSON = organizer.sendRequestsToWebServer("CALENDAR");
    if(parseJSON) { Serial.println("parse CALENDAR OK"); }
    parseJSON = organizer.sendRequestsToWebServer("SETUP_POT_1");
    if(parseJSON) { Serial.println("parse SETUP_POT 1 OK"); }
    parseJSON = organizer.sendRequestsToWebServer("SETUP_POT_2");
    if(parseJSON) { Serial.println("parse SETUP_POT 2 OK"); }
    parseJSON = organizer.sendRequestsToWebServer("SETUP_POT_3");
    if(parseJSON) { Serial.println("parse SETUP_POT 3 OK"); }
    parseJSON = organizer.sendRequestsToWebServer("SETUP_POT_4");
    if(parseJSON) { Serial.println("parse SETUP_POT 4 OK"); }

    Serial.println("\nSCHEDULE");
    organizer.GetSchedule();
    // organizer.GetCalendar();
    // organizer.GetDoses();

    Serial.println();
    Serial.print("freeMemory()=");
    Serial.println(freeMemory());
}

void loop() { 
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= MAIN_LOOP_TIMEOUT) {
        clock.loop();
        previousMillis = currentMillis;
        
        Serial.print("MAIN LOOP ");Serial.print(clock.currHour);Serial.print(":");Serial.print(clock.currMin);Serial.print(":");Serial.print(clock.currSec);Serial.println(); 
        organizer.Loop();
        Serial.print("freeMemory()=");
        Serial.println(freeMemory());
    }
    webServer.receiveWebRequests();
}
