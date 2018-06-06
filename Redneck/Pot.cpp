#include "Pot.h"
#ifdef Pot_h

Pot::Pot(Organizer* organizer, Clock& clock, byte pot_id, unsigned long pot_day_start) : _organizer(organizer), _clock(clock), id(pot_id), day_start(pot_day_start),
	probeT(), probeEC(), probePH(), probeWLev(), pumpWaterLoop() {}

void Pot::Setup(byte pot_id, bool pot_enabled, unsigned long pot_day_start){
    id = pot_id;
    enabled = pot_enabled;
    day_start = pot_day_start;
    probeEC.connect(&probeT);
    n_weeks = getWeeksAge();
}

byte Pot::Identify(){
    return id;
}

byte Pot::getWeeksAge(){
	unsigned long timediff = _organizer->settings.date_time - day_start;
	Serial.print(" timediff => ");
	Serial.print(timediff);
	Serial.println();
	
	n_weeks = timediff / 604800; // seconds in one week
	return true;
}

bool Pot::GetEquivalentDoseId(){
	id_dose = 0;
	for (byte j = 0; j != _organizer->calendar.size(); j++) {
		if(n_weeks<_organizer->calendar[j].week_n){
			id_dose = _organizer->calendar[j].id_dose;
			break; 
		}
	}
	if(id_dose == 0){ id_dose = _organizer->calendar[_organizer->calendar.size()-1].id_dose; }
    return true;
}

bool Pot::GetEquivalentDoses(){
	if(GetEquivalentDoseId()){
		Serial.print(" nweeks ");
		Serial.println(n_weeks);
		
		for (byte j = 0; j != _organizer->doses.size(); j++) {
			if(_organizer->doses[j].id_dose==id_dose){
				Serial.print("id_dose ");Serial.println(_organizer->doses[j].id_dose);
				Serial.print("grow ");Serial.println(_organizer->doses[j].grow);
				Serial.print("micro ");Serial.println(_organizer->doses[j].micro);
				Serial.print("bloom ");Serial.println(_organizer->doses[j].bloom);
				Serial.print("ripen ");Serial.println(_organizer->doses[j].ripen);
				Serial.print("EC ");Serial.println(_organizer->doses[j].EC);
			}
		}
		return true;
	} else { return false; }
	
	
}

void Pot::Loop() {
	if(enabled){
		if(probeT.GetJobLevel()<= (_organizer->settings.work_mode - _organizer->is_night)){
	    	for (byte j = 0; j != probeT.tasks.size(); j++) {
		        if((_clock.currMin==probeT.tasks[j].atMin)&&(probeT.tasks[j].done != true)){
		            probeT.tasks[j].done = true;
		            probeT.readT();
		        }
		        if((_clock.currMin==probeT.tasks[j].atMin+1)&&(probeT.tasks[j].done != false)){
		            probeT.tasks[j].done = false;
		        }
		    }
		}
		if(probePH.GetJobLevel()<= (_organizer->settings.work_mode - _organizer->is_night)){
	    	for (byte j = 0; j != probePH.tasks.size(); j++) {
		        if((_clock.currMin==probePH.tasks[j].atMin)&&(probePH.tasks[j].done != true)){
		            probePH.tasks[j].done = true;
		            probePH.readPH();
		        }
		        if((_clock.currMin==probePH.tasks[j].atMin+1)&&(probePH.tasks[j].done != false)){
		            probePH.tasks[j].done = false;
		        }
		    }
	    }
	    if(probeEC.GetJobLevel()<= (_organizer->settings.work_mode - _organizer->is_night)){
	    	for (byte j = 0; j != probeEC.tasks.size(); j++) {
		        if((_clock.currMin==probeEC.tasks[j].atMin)&&(probeEC.tasks[j].done != true)){
		            probeEC.tasks[j].done = true;
		            probeEC.readEC();
		        }
		        if((_clock.currMin==probeEC.tasks[j].atMin+1)&&(probeEC.tasks[j].done != false)){
		            probeEC.tasks[j].done = false;
		        }
		    }
	    }
	    if(probeWLev.GetJobLevel()<= (_organizer->settings.work_mode - _organizer->is_night)){
	    	for (byte j = 0; j != probeWLev.tasks.size(); j++) {
		        if((_clock.currMin==probeWLev.tasks[j].atMin)&&(probeWLev.tasks[j].done != true)){
		            probeWLev.tasks[j].done = true;
		            probeWLev.readLev();
		        }
		        if((_clock.currMin==probeWLev.tasks[j].atMin+1)&&(probeWLev.tasks[j].done != false)){
		            probeWLev.tasks[j].done = false;
		        }
		    }
		}	
		if(pumpWaterLoop.GetJobLevel()<= (_organizer->settings.work_mode - _organizer->is_night)){
	    	for (byte j = 0; j != pumpWaterLoop.tasks.size(); j++) {
		        if((_clock.currHour == pumpWaterLoop.tasks[j].fromHour)&&(_clock.currMin==pumpWaterLoop.tasks[j].fromMin)&&(pumpWaterLoop.GetStatus() != true)){
		            pumpWaterLoop.SetStatus(true);
		            Serial.println("pumpWaterLoop status changed to true");
		        }
		        if((_clock.currHour == pumpWaterLoop.tasks[j].toHour)&&(_clock.currMin==pumpWaterLoop.tasks[j].toMin)&&(pumpWaterLoop.GetStatus() != false)){
		            pumpWaterLoop.SetStatus(false);
		            Serial.println("pumpWaterLoop status changed to false");
		        }
		    }
		}
		if(pumpWaterAdd.GetJobLevel()<= (_organizer->settings.work_mode - _organizer->is_night)){
		    for (byte j = 0; j != pumpWaterAdd.tasks.size(); j++) {
		        if((_clock.currHour == pumpWaterAdd.tasks[j].atHour)&&(_clock.currMin==pumpWaterAdd.tasks[j].atMin)&&(pumpWaterAdd.GetStatus() != true)){
		            pumpWaterAdd.SetStatus(true);
		            Serial.println("pumpWaterAdd status changed to true");
		        }
		        if((_clock.currHour == pumpWaterAdd.tasks[j].atHour)&&(_clock.currMin==pumpWaterAdd.tasks[j].atMin+1)&&(pumpWaterAdd.GetStatus() != false)){
		            pumpWaterAdd.SetStatus(false);
		            Serial.println("pumpWaterAdd status changed to false");
		        }
		    }
		}
		if(pumpFert.GetJobLevel()<= (_organizer->settings.work_mode - _organizer->is_night)){
		    for (byte j = 0; j != pumpFert.tasks.size(); j++) {
		        if((_clock.currHour == pumpFert.tasks[j].atHour)&&(_clock.currMin==pumpFert.tasks[j].atMin)&&(pumpFert.GetStatus() != true)){
		            pumpFert.SetStatus(true);
		            Serial.println("pumpFert status changed to true");
		        }
		        if((_clock.currHour == pumpFert.tasks[j].atHour)&&(_clock.currMin==pumpFert.tasks[j].atMin+1)&&(pumpFert.GetStatus() != false)){
		            pumpFert.SetStatus(false);
		            Serial.println("pumpFert status changed to false");
		        }
		    }
	    }
	}
}
#endif