#include <TimerOne.h>

#include <PID_Beta6.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <wiring.h>

// period in microseconds
#define PERIOD_IN_MICRO_SECONDS 8000000

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// initial set point
#define INITIAL_SET_POINT 127

// pin to trigger relay
#define TRIGGER_PIN 10

// temp for pins that buttons are on to raise and lower temp
#define TEMP_UP_PIN 11
#define TEMP_DOWN_PIN 12

// seconds we want as a minimum time on or off
#define MINIMUM_ON_OFF_TIME 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

LiquidCrystal lcd(4,5,6,7,8,9);

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,200,90,300);

void setup()
{
 Serial.begin(9600);
 Timer1.initialize(PERIOD_IN_MICRO_SECONDS);
 
  //initialize the variables we're linked to
  Input = 10;
  Setpoint = INITIAL_SET_POINT;
  Output = 50;
  Timer1.pwm(TRIGGER_PIN,Output);
  //Timer1.attachInterrupt(ComputePID);
  //Timer1.start();
  myPID.SetOutputLimits(0,1023);
   // Start up the library
  sensors.begin();
  //turn the PID on
  myPID.SetMode(AUTO);
  lcd.begin(16,2);
  
  // Set pins as inputs
  pinMode(TEMP_UP_PIN, INPUT);
  pinMode(TEMP_DOWN_PIN, INPUT);
  // turn on pullup resistors
  digitalWrite(TEMP_UP_PIN, HIGH);
  digitalWrite(TEMP_DOWN_PIN, HIGH);
  
  Serial.println("Ready");
}

void loop()
{
  
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  Input = sensors.getTempFByIndex(0);
  Serial.println(Input);
  lcd.clear();
  lcd.print("Act: ");
  lcd.print(Input);
  lcd.setCursor(0,1);
  lcd.print("Set: ");
  lcd.print(Setpoint);
  Serial.println(Setpoint);
 
  //delay to keep wait between button checks and temp updates
  delay(1);
  
  if (digitalRead(TEMP_UP_PIN) == LOW)
  {
    Setpoint++;
  }
  else if (digitalRead(TEMP_DOWN_PIN) == LOW)
  {
    Setpoint--;
  }
  if (Input < Setpoint){
    digitalWrite(TRIGGER_PIN, HIGH);
    lcd.setCursor(12,1);
    lcd.print("Cook");
  }
  else {
    digitalWrite(TRIGGER_PIN, LOW);
    lcd.setCursor(12,1);
    lcd.print("Stew");
  }  
}


void ComputePID()
{
  Serial.println("Computing PID Output...");
  myPID.Compute();
  Timer1.setPwmDuty(TRIGGER_PIN,Output);
  Serial.println(Input);
  Serial.println(Output);
}

/*
boolean cook(){
  if (true){
    digitalWrite(TRIGGER_PIN, HIGH);
  }
  else {
    digitalWrite(TRIGGER_PIN, LOW);
  }
}*/
