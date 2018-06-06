#include "ProbeEC.h"
#ifdef ProbeEC_h

ProbeEC::ProbeEC(){}

bool ProbeEC::GetStatus(){
    return status;
}

void ProbeEC::SetStatus(bool stat){
    status = stat;
}

byte ProbeEC::GetJobLevel(){
    return priority;
}

void ProbeEC::Setup(byte pin, byte setup_priority, byte ecgnd, byte ecvcc, byte tvcc, byte tgnd){
    probeEC_port = pin;
    priority = setup_priority;
    ECPin = probeEC_port;
    ECGround = ecgnd; // A1
    ECPower =  ecvcc; //A2;
    //************ Temp Probe Related *********************************************//
    const byte TempProbePositive = tvcc; //Temp Probe power connected to pin 9
    const byte TempProbeNegative = tgnd;  //Temp Probe Negative connected to pin 8
    OneWire oneWire(_probeT->probeT_port);// Setup a oneWire instance to communicate with any OneWire devices
    DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.
}

void ProbeEC::calibrate(){
    //** Adding Digital Pin Resistance to [25 ohm] to the static Resistor *********//
    // Consule Read-Me for Why, or just accept it as true
    R1 = (R1 + Ra);
    byte i = 1;
    buffer = 0;
    sensors.requestTemperatures();// Send the command to get temperatures
    TemperatureStart = sensors.getTempCByIndex(0); //Stores Value in Variable


    //************Estimates Resistance of Liquid ****************//
    while (i <= 10) {
        digitalWrite(ECPower, HIGH);
        raw = analogRead(ECPin);
        raw = analogRead(ECPin); // This is not a mistake, First reading will be low
        digitalWrite(ECPower, LOW);
        buffer = buffer + raw;
        i++;
        delay(5000);
    };
    raw = (buffer / 10);

    sensors.requestTemperatures();// Send the command to get temperatures
    TemperatureFinish = sensors.getTempCByIndex(0); //Stores Value in Variable

    //*************Compensating For Temperaure********************//
    EC = CalibrationEC * (1 + (TemperatureCoef * (TemperatureFinish - 25.0))) ;

    //***************** Calculates R relating to Calibration fluid **************************//
    Vdrop = (((Vin) * (raw)) / 1024.0);
    Rc = (Vdrop * R1) / (Vin - Vdrop);
    Rc = Rc - Ra;
    Kcalibration = 1000 / (Rc * EC);

    Serial.print("Calibration Fluid EC: ");
    Serial.print(CalibrationEC);
    Serial.print(" S  ");  //add units here
    Serial.print("Cell Constant K");
    Serial.print(Kcalibration);
    Serial.print(" - ");
    Serial.print(TemperatureStart);
    Serial.print(" ");
    Serial.print(TemperatureFinish);


    if (TemperatureStart == TemperatureFinish) {
        Serial.println("  Results are Trustworthy");
        Serial.println("  Safe To Use Above Cell Constant in Main EC code");
    } else {
        Serial.println("  Error -Wait For Temperature To settle");
    }
}

bool ProbeEC::connect(ProbeT *probeT){
    _probeT = probeT;
    return true;
}

bool ProbeEC::Schedule( byte atMin){
    tasks.push_back({ atMin, false});
    return true;
}

bool ProbeEC::readEC(){
    GetEC();          //Calls Code to Go into GetEC() Loop [Below Main Loop] dont call this more that 1/5 hhz [once every five seconds] or you will polarise the water
    PrintReadings();  // Cals Print routine [below main loop]
}

void ProbeEC::GetEC() {
    /**/
    pinMode(TempProbeNegative , OUTPUT ); //seting ground pin as output for tmp probe
    digitalWrite(TempProbeNegative , LOW );//Seting it to ground so it can sink current
    pinMode(TempProbePositive , OUTPUT );//ditto but for positive
    digitalWrite(TempProbePositive , HIGH );
    /**/

    pinMode(ECPin, INPUT);
    pinMode(ECPower, OUTPUT); //Setting pin for sourcing current
    pinMode(ECGround, OUTPUT); //setting pin for sinking current
    digitalWrite(ECGround, LOW); //We can leave the ground connected permanantly

    delay(100);// gives sensor time to settle
    sensors.begin();
    delay(100);
    //** Adding Digital Pin Resistance to [25 ohm] to the static Resistor *********//
    // Consule Read-Me for Why, or just accept it as true
    R1 = (R1 + Ra); // Taking into acount Powering Pin Resitance

    //*********Reading Temperature Of Solution *******************//
    sensors.requestTemperatures();// Send the command to get temperatures
    Temperature = sensors.getTempCByIndex(0); //Stores Value in Variable

    //************Estimates Resistance of Liquid ****************//
    digitalWrite(ECPower, HIGH);
    raw = analogRead(ECPin);
    raw = analogRead(ECPin); // This is not a mistake, First reading will be low beause if charged a capacitor
    digitalWrite(ECPower, LOW);

  //***************** Converts to EC **************************//
    Vdrop = (Vin * raw) / 1024.0;
    Rc = (Vdrop * R1) / (Vin - Vdrop);
    Rc = Rc - Ra; //acounting for Digital Pin Resitance
    EC = 1000 / (Rc * Kempiric);


    //*************Compensating For Temperaure********************//
    EC25  =  EC / (1 + TemperatureCoef * (Temperature - 25.0));
    ppm = (EC25) * (PPMconversion * 1000);
}
//************************** End OF EC Function ***************************//


//***This Loop Is called From Main Loop- Prints to serial usefull info ***//
void ProbeEC::PrintReadings() {
    Serial.print("Rc: ");
    Serial.print(Rc);
    Serial.print(" EC: ");
    Serial.print(EC25);
    Serial.print(" Simens  ");
    Serial.print(ppm);
    Serial.print(" ppm  ");
    Serial.print(Temperature);
    Serial.println(" *C ");
};

byte ProbeEC::Identify(){
    return probeEC_port;
}

bool ProbeEC::GetSchedule(){
    Serial.print(" port => ");Serial.print(probeEC_port);Serial.println();
        for (byte j = 0; j != tasks.size(); j++) {
            Serial.print(" at "); Serial.print(":"); Serial.print(tasks[j].atMin);
            Serial.println();
        }
    return true;
}

void ProbeEC::displayItem(){
    byte a = _probeT->Identify();
    Serial.print("--");Serial.print(a);Serial.print("--");
}
#endif