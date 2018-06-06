#include "WiFiEsp.h"
#include "Organizer.h"
#ifdef Organizer_h

WiFiEspClient WiFiServiceClient;

const char* ssid         = "Vodafone-27001017";
//const char* ssid       = "Klekoo Telecom";
//const char* ssid       = "Linkem2.4GHz_75FD34";
const char* password     = "simplepwd";
//const char* password   = "Klekoo-44-cesare";
//const char* password   = "kavexoqb";

char webServerURI[] = "www.voidbrain.net";
char url[] = "/temp/grover/ajax/moduli/api/redneck/endpoint";
String params = "";
byte status = WL_IDLE_STATUS;

Organizer::Organizer(Clock& clock, String json_text) : _clock(clock) {
    Clock _clock(clock);
}

bool Organizer::begin(Stream &stream){
    _stream = &stream;
    Serial.println("client begin");
    WiFi.init(_stream);
    
    if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        while (true);   // don't continue
    }
    while ( status != WL_CONNECTED) {
        Serial.println("Attempting to connect to Wifi");
        status = WiFi.begin(ssid, password);
    }
    Serial.println("You're connected to the network");
}

void Organizer::GetCalendar(){
    //Serial.println(calendar.size());
    for (byte i = 0; i != calendar.size(); i++) {
        Serial.println();Serial.print("Calendar");Serial.println();
        Serial.print("settimana "); Serial.print(calendar[i].week_n);
        Serial.print(" id_dose "); Serial.print(calendar[i].id_dose);

        Serial.println();
    }
}

void Organizer::GetDoses(){
    for (byte i = 0; i != doses.size(); i++) {
        Serial.println();Serial.print("Doses");Serial.println();
        Serial.print(" id_dose ");Serial.print(doses[i].id_dose);
        Serial.print(" grow ");Serial.print(doses[i].grow);
        Serial.print(" micro ");Serial.print(doses[i].micro);
        Serial.print(" bloom ");Serial.print(doses[i].bloom);
        Serial.print(" ripen ");Serial.print(doses[i].ripen);
        Serial.print(" EC ");Serial.print(doses[i].EC);
        Serial.println();
    }
}

void Organizer::GetSchedule(){
    // for (byte i = 0; i != lights.size(); i++) {
    //     Serial.print("Light ");Serial.print(" port => ");Serial.print(lights[i].Identify());
    //     Serial.println();lights[i].GetSchedule();
    // }
    // for (byte i = 0; i != fans.size(); i++) {
    //     Serial.print("Fan ");Serial.print(" port => ");Serial.print(fans[i].Identify());
    //     Serial.println();fans[i].GetSchedule();
    // }
    for (byte i = 0; i != pots.size(); i++) {
        Serial.println();Serial.println();Serial.print("Pot ");Serial.print(" id => ");Serial.print(pots[i].Identify());
        //Serial.print(" day_start => ");Serial.print(pots[i].Age());Serial.print(" n_weeks => ");Serial.println(pots[i].getWeeksAge());
        // Serial.print(" equivalent dose id => ");Serial.println(pots[i].GetEquivalentDoseId());
        // Serial.print(" equivalent dosage => ");Serial.println(pots[i].GetEquivalentDoses(pots[i].GetEquivalentDoseId()));
        // Serial.println();
        pots[i].GetEquivalentDoses();

        // Serial.print("probePH ");Serial.print(" port => ");Serial.print(pots[i].probePH.Identify());
        // Serial.println();pots[i].probePH.GetSchedule();
        // Serial.println();
        // Serial.print("probeEC ");Serial.print(" port => ");Serial.print(pots[i].probeEC.Identify());
        // Serial.println();pots[i].probeEC.GetSchedule();
        // Serial.println();
        // Serial.print("probeT ");Serial.print(" port => ");Serial.print(pots[i].probeT.Identify());
        // Serial.println();pots[i].probeT.GetSchedule();
        // Serial.println();
        // Serial.print("probeWLev ");Serial.print(" port => ");Serial.print(pots[i].probeWLev.Identify());
        // Serial.println();pots[i].probeWLev.GetSchedule();
        // Serial.print("pumpFert ");Serial.print(" port => ");Serial.print(pots[i].pumpFert.Identify());
        // Serial.println();
        // Serial.print("pumpWaterAdd ");Serial.print(" port => ");Serial.print(pots[i].pumpWaterAdd.Identify());
        // Serial.println();
        // Serial.print("pumpWaterLoop ");Serial.print(" port => ");Serial.print(pots[i].pumpWaterLoop.Identify());
        // Serial.println();pots[i].pumpWaterLoop.GetSchedule();
    }
    // Serial.println();Serial.print("Clock ");
    // for (byte i = 0; i != scheduleClocks.size(); i++) {
    //     Serial.println();scheduleClocks[i].GetSchedule();
    //     Serial.println();
    // }
}

bool Organizer::sendRequestsToWebServer(char* key, String value){
    WiFiServiceClient.stop(); // close any connection before send a new request, this will free the socket on the WiFi shield
    if (WiFiServiceClient.connect(webServerURI, 80)) {
        WiFiServiceClient.println("GET /temp/grover/ajax/moduli/api/redneck/endpoint?action="+String(key)+" HTTP/1.0");
        Serial.println("GET /temp/grover/ajax/moduli/api/redneck/endpoint?action="+String(key)+" HTTP/1.0");
        WiFiServiceClient.println("Host: www.voidbrain.net");
        WiFiServiceClient.println("Connection: close");
        if (WiFiServiceClient.println() == 0) {
            Serial.println(F("Failed to send request"));
            return;
        }
        
        char status[32] = {0}; // Check HTTP status
        WiFiServiceClient.readBytesUntil('\r', status, sizeof(status));
        if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
            Serial.print(F("Unexpected response: "));
            Serial.println(status);
            return;
        }

        char endOfHeaders[] = "\r\n\r\n"; // Skip HTTP headers
        if (!WiFiServiceClient.find(endOfHeaders)) {
            Serial.println(F("Invalid response"));
            return;
        }
        DynamicJsonBuffer jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(WiFiServiceClient);
        if (!root.success()) {
            Serial.println(F("Parsing failed!"));
            return;
        }

        WiFiServiceClient.stop(); // Disconnect

        if(root.containsKey("settings")){ 
            //Serial.println("found settings in json");
            JsonObject& jsonSettings = root["settings"]; 
            if(jsonSettings.containsKey("date_time")){  settings.date_time = jsonSettings["date_time"];}
            if(jsonSettings.containsKey("store_data")){  settings.store_data = jsonSettings["store_data"];}
            if(jsonSettings.containsKey("ph_vcc")){  settings.phvcc = jsonSettings["ph_vcc"];}
            if(jsonSettings.containsKey("ph_gnd")){  settings.phgnd = jsonSettings["ph_gnd"];}
            if(jsonSettings.containsKey("t_vcc")){  settings.tvcc = jsonSettings["t_vcc"];}
            if(jsonSettings.containsKey("t_gnd")){  settings.tgnd = jsonSettings["t_gnd"];}
            if(jsonSettings.containsKey("ec_vcc")){  settings.ecvcc = jsonSettings["ec_vcc"];}
            if(jsonSettings.containsKey("ec_gnd")){  settings.ecgnd = jsonSettings["ec_gnd"];}
            if(jsonSettings.containsKey("work_mode")){  settings.work_mode = jsonSettings["work_mode"];}
            if(jsonSettings.containsKey("night_mode_on")){  settings.night_mode_on = jsonSettings["night_mode_on"];}
            if(jsonSettings.containsKey("night_mode_off")){  settings.night_mode_off = jsonSettings["night_mode_off"];}

            _clock.setClock(settings.date_time);
            Serial.print("setClock "); Serial.println(settings.date_time);
        }
        if(root.containsKey("calendar")){ 
            //Serial.println("found calendar in json");
            JsonArray& json_calendar = root["calendar"]; 
            for(byte i=0; i<json_calendar.size();i++){
                calendar.push_back(addCalendar(json_calendar[i]["week_n"],json_calendar[i]["id_dose"]));
            }
        }
        if(root.containsKey("doses")){ 
            //Serial.prbyteln("found doses in json");
            JsonArray& json_doses = root["doses"]; 
            for(byte i=0; i<json_doses.size();i++){
                doses.push_back(addDose(json_doses[i]["id_dose"], json_doses[i]["grow"], json_doses[i]["micro"], json_doses[i]["bloom"], json_doses[i]["ripen"], json_doses[i]["EC"]));
            }
        }
        if(root.containsKey("clock")){ 
            //Serial.prbyteln("found clock in json");
            JsonObject& json_clock = root["clock"]; 
            Serial.println();

            JsonArray& json_clockSchedules = json_clock["schedules"];
            ScheduleClock scheduleClock(this, _clock,json_clock["priority"]);
            for(byte j=0; j<json_clockSchedules.size();j++){
                const char* clockOn = json_clockSchedules[j]["on"];
                char* atOreString = strtok(clockOn, ":");
                char* atMinString = strtok(NULL, ":");
                byte atOre, atMin;
                if(atOreString[0]=='0'){ atOre = atOreString[1]  - '0'; } else { atOre = atoi( atOreString ); }
                if(atMinString[0]=='0'){ atMin = atMinString[1]  - '0'; } else { atMin = atoi( atMinString ); }
                scheduleClock.Schedule(atOre, atMin);
            }
            scheduleClocks.push_back(scheduleClock);
            
        }
        if(root.containsKey("lights")){ 
            //Serial.println("found lights in json");
            JsonArray& lightsArray = root["lights"]; 
            for(byte i=0; i<lightsArray.size();i++){
                JsonArray& lightsArraySchedules = lightsArray[i]["schedules"];
                Light light(this, _clock);
                light.Setup(lightsArray[i]["id_pin"],lightsArray[i]["priority"],lightsArray[i]["enabled"]);
                for(byte j=0; j<lightsArraySchedules.size();j++){
                    const char* lightsOn = lightsArraySchedules[j]["on"];
                    char* fromOreString = strtok(lightsOn, ":");
                    char* fromMinString = strtok(NULL, ":");
                    const char* lightsOff = lightsArraySchedules[j]["off"];
                    char* toOreString = strtok(lightsOff, ":");
                    char* toMinString = strtok(NULL, ":");
                    byte fromOre, fromMin, toOre, toMin;
                    if(fromOreString[0]=='0'){ fromOre = fromOreString[1]  - '0'; } else { fromOre = atoi( fromOreString ); }
                    if(fromMinString[0]=='0'){ fromMin = fromMinString[1]  - '0'; } else { fromMin = atoi( fromMinString ); }
                    if(toOreString[0]=='0'){ toOre = toOreString[1]  - '0'; } else { toOre = atoi( toOreString ); }
                    if(toMinString[0]=='0'){ toMin = toMinString[1]  - '0'; } else { toMin = atoi( toMinString ); }
                    light.Schedule(fromOre, fromMin, toOre, toMin);
                }
                lights.push_back(light);
            }
        }
        if(root.containsKey("fans")){ 
            //Serial.println("found fans in json");
            JsonArray& fansArray = root["fans"]; 
            for(byte i=0; i<fansArray.size();i++){
                JsonArray& fansArraySchedules = fansArray[i]["schedules"];
                Fan fan(this, _clock);
                fan.Setup(fansArray[i]["id_pin"],fansArray[i]["priority"],fansArray[i]["enabled"]);
                for(byte j=0; j<fansArraySchedules.size();j++){
                    const char* fansOn = fansArraySchedules[j]["on"];
                    char* fromOreString = strtok(fansOn, ":");
                    char* fromMinString = strtok(NULL, ":");
                    const char* fansOff = fansArraySchedules[j]["off"];
                    char* toOreString = strtok(fansOff, ":");
                    char* toMinString = strtok(NULL, ":");
                    byte fromOre, fromMin, toOre, toMin;
                    if(fromOreString[0]=='0'){ fromOre = fromOreString[1]  - '0'; } else { fromOre = atoi( fromOreString ); }
                    if(fromMinString[0]=='0'){ fromMin = fromMinString[1]  - '0'; } else { fromMin = atoi( fromMinString ); }
                    if(toOreString[0]=='0'){ toOre = toOreString[1]  - '0'; } else { toOre = atoi( toOreString ); }
                    if(toMinString[0]=='0'){ toMin = toMinString[1]  - '0'; } else { toMin = atoi( toMinString ); }
                    fan.Schedule(fromOre, fromMin, toOre, toMin);
                }
                fans.push_back(fan);
            }
        }
        if(root.containsKey("pots")){ 
            //Serial.println("found pot in json");
            JsonArray& potsArray = root["pots"]; 
            for(byte i=0; i<potsArray.size();i++){
                Pot pot(this, _clock);
                pot.Setup(potsArray[i]["id"],potsArray[i]["enabled"],potsArray[i]["day_start"]);

                JsonArray& potsArraySchedulesT = potsArray[i]["probes"]["t"]["schedules"];
                JsonArray& potsArraySchedulesPH = potsArray[i]["probes"]["ph"]["schedules"];
                JsonArray& potsArraySchedulesEC = potsArray[i]["probes"]["ec"]["schedules"];
                JsonArray& potsArraySchedulesWLev = potsArray[i]["probes"]["h2oLev"]["schedules"];
                JsonArray& schedulesPumpWaterLoop = potsArray[i]["pumps"]["waterloop"]["schedules"];
                JsonArray& schedulesPumpWaterAdd = potsArray[i]["pumps"]["wateradd"]["schedules"];
                JsonArray& schedulesPumpFert = potsArray[i]["pumps"]["fertilizer"]["schedules"];
                pot.probeT.Setup(potsArray[i]["probes"]["t"]["id_pin"], potsArray[i]["probes"]["t"]["priority"], settings.tvcc, settings.tgnd);
                pot.probePH.Setup(potsArray[i]["probes"]["ph"]["id_pin"], potsArray[i]["probes"]["ph"]["priority"]);
                pot.probeEC.Setup(potsArray[i]["probes"]["ec"]["id_pin"], potsArray[i]["probes"]["ec"]["priority"], settings.ecgnd, settings.ecvcc, settings.tvcc, settings.tgnd);
                pot.probeWLev.Setup(potsArray[i]["probes"]["h2oLev"]["id_pin"], potsArray[i]["probes"]["h2oLev"]["priority"]);
                pot.pumpFert.Setup(potsArray[i]["pumps"]["fertilizer"]["id_pin"], potsArray[i]["pumps"]["fertilizer"]["priority"]);
                pot.pumpWaterLoop.Setup(potsArray[i]["pumps"]["waterloop"]["id_pin"], potsArray[i]["pumps"]["waterloop"]["priority"]);
                pot.pumpWaterAdd.Setup(potsArray[i]["pumps"]["wateradd"]["id_pin"], potsArray[i]["pumps"]["wateradd"]["priority"]);
                
                for(byte j=0; j<potsArraySchedulesT.size();j++){
                    const char* probeTOn = potsArraySchedulesT[j]["on"];
                    char* atOreString = strtok(probeTOn, ":");
                    char* atMinString = strtok(NULL, ":");
                    byte atMin;
                    if(atMinString[0]=='0'){ atMin = atMinString[1]  - '0'; } else { atMin = atoi( atMinString ); }
                    pot.probeT.Schedule(atMin);
                }
                for(byte j=0; j<potsArraySchedulesPH.size();j++){
                    const char* probePHOn = potsArraySchedulesPH[j]["on"];
                    char* atOreString = strtok(probePHOn, ":");
                    char* atMinString = strtok(NULL, ":");
                    byte atMin;
                    if(atMinString[0]=='0'){ atMin = atMinString[1]  - '0'; } else { atMin = atoi( atMinString ); }
                    pot.probePH.Schedule(atMin);
                }
                for(byte j=0; j<potsArraySchedulesEC.size();j++){
                    const char* probeECOn = potsArraySchedulesEC[j]["on"];
                    char* atOreString = strtok(probeECOn, ":");
                    char* atMinString = strtok(NULL, ":");
                    byte atMin;
                    if(atMinString[0]=='0'){ atMin = atMinString[1]  - '0'; } else { atMin = atoi( atMinString ); }
                    pot.probeEC.Schedule(atMin);
                }
                for(byte j=0; j<potsArraySchedulesWLev.size();j++){
                    const char* probeWLevOn = potsArraySchedulesWLev[j]["on"];
                    char* atOreString = strtok(probeWLevOn, ":");
                    char* atMinString = strtok(NULL, ":");
                    byte atMin;
                    if(atMinString[0]=='0'){ atMin = atMinString[1]  - '0'; } else { atMin = atoi( atMinString ); }
                    pot.probeWLev.Schedule(atMin);
                }
                for(byte j=0; j<schedulesPumpWaterLoop.size();j++){
                    const char* schedulesPumpWaterLoopOn = schedulesPumpWaterLoop[j]["on"];
                    char* fromOreString = strtok(schedulesPumpWaterLoopOn, ":");
                    char* fromMinString = strtok(NULL, ":");
                    const char* schedulesPumpWaterLoopOff = schedulesPumpWaterLoop[j]["off"];
                    char* toOreString = strtok(schedulesPumpWaterLoopOff, ":");
                    char* toMinString = strtok(NULL, ":");
                    byte fromOre, fromMin, toOre, toMin;
                    if(fromOreString[0]=='0'){ fromOre = fromOreString[1]  - '0'; } else { fromOre = atoi( fromOreString ); }
                    if(fromMinString[0]=='0'){ fromMin = fromMinString[1]  - '0'; } else { fromMin = atoi( fromMinString ); }
                    if(toOreString[0]=='0'){ toOre = toOreString[1]  - '0'; } else { toOre = atoi( toOreString ); }
                    if(toMinString[0]=='0'){ toMin = toMinString[1]  - '0'; } else { toMin = atoi( toMinString ); }
                    pot.pumpWaterLoop.Schedule(fromOre, fromMin, toOre, toMin);
                }
                for(byte j=0; j<schedulesPumpWaterAdd.size();j++){
                    const char* schedulesPumpWaterAddOn = schedulesPumpWaterAdd[j]["on"];
                    char* atOreString = strtok(schedulesPumpWaterAddOn, ":");
                    char* atMinString = strtok(NULL, ":");
                    byte atOre, atMin;
                    if(atOreString[0]=='0'){ atOre = atOreString[1]  - '0'; } else { atOre = atoi( atOreString ); }
                    if(atMinString[0]=='0'){ atMin = atMinString[1]  - '0'; } else { atMin = atoi( atMinString ); }
                    pot.pumpWaterAdd.Schedule(atOre, atMin);
                }
                for(byte j=0; j<schedulesPumpFert.size();j++){
                    const char* schedulesPumpFertOn = schedulesPumpFert[j]["on"];
                    char* atOreString = strtok(schedulesPumpFertOn, ":");
                    char* atMinString = strtok(NULL, ":");
                    byte atOre, atMin;
                    if(atOreString[0]=='0'){ atOre = atOreString[1]  - '0'; } else { atOre = atoi( atOreString ); }
                    if(atMinString[0]=='0'){ atMin = atMinString[1]  - '0'; } else { atMin = atoi( atMinString ); }
                    pot.pumpFert.Schedule(atOre, atMin);
                }
                pots.push_back(pot);
            }
        }

        if (!root.success()) {
            Serial.println("parseObject() failed");
            return false;
        }

        return true;
        
    } else {
        Serial.println("Connection failed");
        return;
    }
}

Doses Organizer::addDose(byte id_dose, float grow, float micro, float bloom, float ripen, float EC){
    Doses dose_entry = { id_dose, grow, micro, bloom, ripen, EC };
    return dose_entry;
}
Calendar Organizer::addCalendar(byte week_n, byte id_dose){
    Calendar calendar_entry = { week_n, id_dose };
    return calendar_entry;
}

void Organizer::checkWorkingMode(){
    return settings.work_mode;
}

void Organizer::Loop(){
    Serial.println("Organizer loop");
    unsigned long currentMillis = millis();
    _clock.isNight();
    //checkNightMode();
    //checkWorkingMode();

    for (byte i = 0; i != scheduleClocks.size(); i++) {
        scheduleClocks[i].Loop();
    }    
    for (byte i = 0; i != lights.size(); i++) {
        lights[i].Loop();
    }
    for (byte i = 0; i != fans.size(); i++) {
            fans[i].Loop();
    }
    for (byte i = 0; i != pots.size(); i++) {
        pots[i].Loop();
    }
}

void Organizer::printWifiStatus() {
    Serial.println("SSID: ");
    Serial.println(WiFi.SSID());
    IPAddress ip = WiFi.localIP();
    Serial.println("IP Address: ");
    Serial.println(String(ip));
    long rssi = WiFi.RSSI();
    Serial.println("Signal strength (RSSI):");
    Serial.println(String(rssi));
    Serial.println(" dBm");
}
#endif