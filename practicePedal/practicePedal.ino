#include <StopWatch.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define THRESHOLD 2.5
#define LIMIT 5

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

uint8_t customChar[8] = {
  B00001,
  B00011,
  B00101,
  B01001,
  B01001,
  B01011,
  B11011,
  B11000,
};

unsigned long totalTime = 0;
StopWatch sw(StopWatch::SECONDS);
Servo arm;
float pos = 0.0;
bool dispenced = false;
char* totalTimeString = "00:00:00";
char* goalTimeString = "00:00:00";
int totalTimeHr = 0;
int totalTimeMin = 0;
int totalTimeSec = 0;

int readInput() {
  int value = analogRead(A0);
  int voltage = (value * (5.0 / 1023));

  if (voltage >= THRESHOLD) {
    if (!sw.isRunning()) {
      sw.start();
    }
  } else {
    if (sw.isRunning()) {
      sw.stop();
    }
  }
}

bool checkCandyDispense(int voltage) {
  totalTime = sw.elapsed();

  if (totalTime >= LIMIT + 1) {
    if (sw.isRunning()) {
      sw.stop();
    }
    if (!dispenced) {
      arm.write(180);
      delay(3000);
      arm.write(-180);
      dispenced = true;
    }
  }

  return dispenced;
}

void updateScreen(bool dispenced) {
  if (!dispenced) {
    totalTimeHr = totalTime/3600;
	  totalTimeMin = (totalTime - totalTimeHr*3600)/60;
	  totalTimeSec = totalTime - totalTimeHr*3600 - totalTimeMin*60;
    sprintf(totalTimeString, "%02d:%02d:%02d", totalTimeHr, totalTimeMin, totalTimeSec);
    lcd.clear();
    lcd.print("Time: ");
    lcd.print(totalTimeString);
    lcd.setCursor(0, 1);
    int goalHr = LIMIT/3600;
    int goalMin = (LIMIT - goalHr*3600)/60;
    int goalSec = LIMIT - goalHr*3600 - goalMin*60;
    sprintf(goalTimeString, "%02d:%02d:%02d", goalHr, goalMin, goalSec);
    lcd.print("Goal: ");
    lcd.print(goalTimeString);
  } else {
    lcd.clear();
    lcd.print(" \x03 ");
    lcd.print("Congrats!");
    lcd.print(" \x03 ");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
  arm.attach(9);
  arm.write(pos);
  lcd.begin(16, 2);
  lcd.createChar(3, customChar);
}

void loop() {
  // put your main code here, to run repeatedly:
  int voltage = readInput();
  bool dispenced = checkCandyDispense(voltage);
  updateScreen(dispenced);
  delay(1000);
}