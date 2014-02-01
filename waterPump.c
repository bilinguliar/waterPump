const int WATER_SENSOR = 2;     // the number of the switch pin
const int MANUAL_ON_BUTTON = 3; // button that will manually turn on pump if needed
const int PUMP =  0;      // the number of the pump control pin
const int WATER_LEVEL_LOW_LED = 1; // the number of waterlevel low indicator
const int PUMP_AUTO_WORKING_TIME = 10000; // pump will work for this period of time if sensor showed Low water level
const int PUMP_MANUAL_WORKING_TIME = 100; // pump will work for this period of time if button was pressed
const int ALARM_LED = 4; // Led that will be turned on if something went wrong
const int ALARM_LED_DELAY = 600; // Delay for alarm blinking
short autoCyclesCounter = 0;


void setup() {
  // initialize the pump control pin as an output:
  pinMode(PUMP, OUTPUT); 
  // initialize the water level low led pin as an output:
  pinMode(WATER_LEVEL_LOW_LED, OUTPUT); 
   // initialize alarm led as output:
  pinMode(ALARM_LED, OUTPUT); 
  // initialize the watersensor pin as an input:
  pinMode(WATER_SENSOR, INPUT);
  digitalWrite(WATER_SENSOR, HIGH); // pull-up resistor
  // initialize manual ON button as an input:
  pinMode(MANUAL_ON_BUTTON, INPUT);
  digitalWrite(MANUAL_ON_BUTTON, HIGH); // pull-up resistor
}


void turnPumpOff(){
  digitalWrite(PUMP, LOW); 
}


void turnPumpOn(int timeToWork){
  digitalWrite(PUMP, HIGH);
  delay(timeToWork);
  turnPumpOff();
}


void turnLowLevelIndicatorOn() {
  digitalWrite(WATER_LEVEL_LOW_LED, HIGH);
}


void turnLowLevelIndicatorOff() {
  digitalWrite(WATER_LEVEL_LOW_LED, LOW);
}


void turnAlarmLedOn() {
  digitalWrite(ALARM_LED, HIGH);
}


void turnAlarmLedOff() {
  digitalWrite(ALARM_LED, LOW);
}


void doAlarmState() {
  while(true) {
    turnAlarmLedOn();
    delay(ALARM_LED_DELAY);
    turnAlarmLedOff();
    delay(ALARM_LED_DELAY);
  }
}


boolean isManualyTurnedOn() {
  boolean turnedOn = false;
  int manualOnState = digitalRead(MANUAL_ON_BUTTON);
  
  if (manualOnState == LOW) {
    turnedOn = true;
  }
  
  return turnedOn;
}


boolean isWaterLevelLow() {
  boolean levelLow = false;

  for (int i = 0; i < 50; i++) {
    int waterLevelState = digitalRead(WATER_SENSOR);

    if (waterLevelState == LOW) {
      levelLow = true;
      turnLowLevelIndicatorOn();
      
      delay(100);
      
    } else {
      levelLow = false;
      turnLowLevelIndicatorOff();
      break;
    }
  }

  return levelLow;
}


void loop(){

  if (isWaterLevelLow()) {
    autoCyclesCounter++;
    
    // We allow only 2 automated fillings in a row in other case show alarm state
    if (autoCyclesCounter > 2) {
      turnLowLevelIndicatorOff();
      doAlarmState();
    } else {
      turnPumpOn(PUMP_AUTO_WORKING_TIME);
    }
    
  } else if (isManualyTurnedOn()) {
    turnPumpOn(PUMP_MANUAL_WORKING_TIME);
  } else {
    autoCyclesCounter = 0;
    turnPumpOff(); 
  }
}

