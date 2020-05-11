#define rpmPin 2
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
}
