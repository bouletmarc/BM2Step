
#include <SPI.h>
#include <Wire.h>
#include "Arduino.h"

int ontime,offtime,duty;
float freq,period;

#define pulse_ip 3
#define Cutting_Output 2
#define Activate_Input 4
#define LaunchRPM_Input A0
#define LaunchDelay_Input A1

#define BAUDRATE  115200

int LaunchRPM = 3500;
int CutRPMWindows = 500;
int CurrentRPM = 0;
int SmoothRPM[] = {0, 0, 0};
int SmoothRPMBuffer[] = {0, 0, 0};
bool Cutting2Step = false;
int PotRpmValue;

void setup() {
  Serial.begin(BAUDRATE);
  
  pinMode(pulse_ip,INPUT);
  pinMode(Cutting_Output, OUTPUT);
  pinMode(LaunchRPM_Input, INPUT);
  pinMode(LaunchDelay_Input, INPUT);
  pinMode(Activate_Input, INPUT);
}

void loop() {
  ontime = pulseIn(pulse_ip,HIGH);
  offtime = pulseIn(pulse_ip,LOW);
  period = ontime+offtime;
  freq = 1000000.0/period;
  duty = (ontime/period)*100;

  if (duty > 0 && duty < 100) {
    CurrentRPM = freq * 30;
    AddToSmooth(CurrentRPM);
    
    GetLaunchRPM();
    GetCutRPM();
    Check2Step();
    
    SerialPrinting();
  }
  else {
    Disable2Step();
    Serial.print("Unknown RPM detected!");
    Serial.println("");
  }

  delay(1);

}

void GetLaunchRPM() {
  PotRpmValue = analogRead(LaunchRPM_Input);
  if (PotRpmValue>203)
  {
    LaunchRPM = map(PotRpmValue, 203, 1023, 4500, 9000);
  }
  else
  {
    if (PotRpmValue>126)
    {
      LaunchRPM = map(PotRpmValue, 127, 203, 3200, 4500);
    }
    else
    {
      LaunchRPM = map(PotRpmValue, 0, 126, 2500, 3200);
    }
  }
  LaunchRPM = (LaunchRPM +24)/25;
  LaunchRPM = LaunchRPM * 25;
}

void GetCutRPM() {
  CutRPMWindows = analogRead(LaunchDelay_Input);
}

void SerialPrinting() {
  Serial.print("Freq:");
  Serial.print(freq);
  Serial.print("Hz");
  
  Serial.print("\tDuty");
  Serial.print(duty);
  Serial.print("%");

  
  Serial.print("\tRPM:");
  Serial.print(GetSmoothRPM());
  //Serial.print(CurrentRPM);

  
  Serial.print("\t2Step:");
  Serial.print(Cutting2Step);
  Serial.print("\tLaunch:");
  Serial.print(LaunchRPM);
  Serial.print("\tCut:");
  Serial.print(CutRPMWindows);
  
  /*Serial.print("\ton");
  Serial.print(ontime);
  Serial.print("\toff");
  Serial.print(offtime);
  Serial.print("\ttime");
  Serial.print(period);*/
  
  Serial.println("");
}

void Check2Step() {
  bool CanLaunch = false;
  if (digitalRead(Activate_Input) == LOW) {
    CanLaunch = true;
  }

  if (CanLaunch) {
    if (GetSmoothRPM() > LaunchRPM && !Cutting2Step) {
    //if (CurrentRPM > LaunchRPM && !Cutting2Step) {
      Cutting2Step = true;
      digitalWrite(Cutting_Output, HIGH);
    }
    if (GetSmoothRPM() < LaunchRPM - CutRPMWindows && Cutting2Step) {
    //if (CurrentRPM < LaunchRPM - CutRPMWindows && Cutting2Step) {
      Cutting2Step = false;
      digitalWrite(Cutting_Output, LOW);
    }
  } else {
    Cutting2Step = false;
    digitalWrite(Cutting_Output, LOW);
  }
}

void Disable2Step() {
  
    Cutting2Step = false;
    digitalWrite(Cutting_Output, LOW);
}

void AddToSmooth(int AddThisRPM) {
  for (int i = 0; i < 3; i++) {
    SmoothRPMBuffer[i] = SmoothRPM[i];
  }

  SmoothRPM[0] = AddThisRPM;
  
  for (int i = 0; i < 2; i++) {
    SmoothRPM[i+1] = SmoothRPMBuffer[i];
  }
}

int GetSmoothRPM() {
  int TotalRPM = 0;
  for (int i = 0; i < 3; i++) {
    TotalRPM += SmoothRPM[i];
  }

  return TotalRPM / 3;
}

//#########################################################
//#########################################################
//#########################################################
//#########################################################


/*#define rpmPin 2
#define outputRPM 3
#define potPinDelay A0
#define potPinRPM1 A1
#define Activate 6

const int refreshRate = 10;

unsigned int cuttingDelay = 60;
unsigned int cuttingRPM1 = 6000;
unsigned int cuttingRPM2 = 4500;
volatile int revs = 0;
unsigned long rpm = 0;
unsigned long lastmillis = 0;
unsigned long lastmillisCutting = 0;
bool CuttingLimiter = false;
bool CanLaunch = false;


int SmoothSize = 50;
int SmoothingRPM[50];
int CurrentSmooth = 0;


void setup() {
  Serial.begin(115200);
   
  pinMode(Activate, INPUT);
  pinMode(rpmPin, INPUT);
  pinMode(outputRPM, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(rpmPin), rpm_engine, FALLING);
}

void loop() {
  if (digitalRead(Activate) == LOW) {
    CanLaunch = true;
  }
  else {
    CanLaunch = false;
  }
  int potDelayRead = analogRead(potPinDelay);
  int potRPM1Read = analogRead(potPinRPM1);
  cuttingDelay = map(potDelayRead, 0, 1023, 0, 500);
  cuttingRPM1 = map(potRPM1Read, 0, 1023, 4000, 9000);
  
  if (millis() - lastmillis >= refreshRate) {
    //detachInterrupt(digitalPinToInterrupt(rpmPin)); // disable interrupt when calculating
    //rpm = (revs  *refreshRate);

    //CalcCutting();

    //SmoothingRPM[CurrentSmooth] = rpm;
    //if (CurrentSmooth < SmoothSize - 2) CurrentSmooth++;
    
    Serial.println("Cutting:" + String(CuttingLimiter) + ",\tRPM:" + String(rpm) + ",\tCut Delay:" + String(cuttingDelay) + ",\tCut RPM1:" + String(cuttingRPM1) + ",\tTest:" + String(revs));
    Serial.flush();     
     
    revs = 0; // restart the RPM counter
    lastmillis = millis(); // update lastmillis
    //attachInterrupt(digitalPinToInterrupt(rpmPin), rpm_engine, FALLING); // Enable interrupt
  }
}

int CalcSmoothRPM() {
  long AllRPM = 0;
   for (int i=0; i < SmoothSize; ++i ) {
    AllRPM += SmoothingRPM[i];
  }
  AllRPM = (AllRPM / SmoothSize);
  return (int) AllRPM;
}

void CalcCutting() {
  //Get Cutting
  if (CanLaunch) {
    if (CalcSmoothRPM() >= cuttingRPM1) {
      if (!CuttingLimiter) {
        CuttingLimiter = true;
        lastmillisCutting = millis();
      }
    }
    else {
      if (CuttingLimiter) {
        if (millis() - lastmillisCutting >= cuttingDelay) {
          CuttingLimiter = false;
        }
      }
    }
  }
  else {
    CuttingLimiter = false;
  }

  //Apply Cut
  if (CuttingLimiter) {
    digitalWrite(outputRPM, LOW);   
  }
  else {
    digitalWrite(outputRPM, HIGH);   
  }
}

void rpm_engine() {
  revs++; 
}*/
