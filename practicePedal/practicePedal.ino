#include <StopWatch.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define THRESHOLD 2.5 //Used to determine threshold for if someone is playing (voltage will always flow, but there is an acceptable amount of noise and past that the guitar is being used)
#define LIMIT 5 //Hard set to establish a goal-time for playing on the guitar (future implementation would allow for custom goal times)

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //Setting our pins for the Arduino Nano (LCD)
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //Using those pins from line above

uint8_t customChar[8] = { //Custom music note character that we implemented
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

int readInput() { //Reading clean/processed signals (from the op amp, rectifier, etc...) through Arduino's analog pin, using that for voltage to determine if someone is playing the guitar
  int value = analogRead(A0);
  int voltage = (value * (5.0 / 1023));

  if (voltage >= THRESHOLD) { //This is the case where we are hitting the threshold voltage or more, meaning we are getting enough to determine that someone is playing
    if (!sw.isRunning()) {
      sw.start();
    }
  } else { //This is the case where we are only receiving noise
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
    if (!dispenced) { //Dispensing candy by rotating the central servo in our dropping mechanism, and returing to grab another
      arm.write(180);
      delay(3000);
      arm.write(-180);
      dispenced = true;
    }
  }

  return dispenced;
}

void updateScreen(bool dispenced) { //For the display on our LCD screen, gives a victory screen if time milestone is hit
  if (!dispenced) { //This is the case when the milestone is not yet hit
    totalTimeHr = totalTime/3600;
	  totalTimeMin = (totalTime - totalTimeHr*3600)/60;
	  totalTimeSec = totalTime - totalTimeHr*3600 - totalTimeMin*60;
    sprintf(totalTimeString, "%02d:%02d:%02d", totalTimeHr, totalTimeMin, totalTimeSec);
    lcd.clear();
    lcd.print("Time: ");
    lcd.print(totalTimeString); //Shows the time-elapsed in a "Time: 00:00:00" format
    lcd.setCursor(0, 1);
    int goalHr = LIMIT/3600;
    int goalMin = (LIMIT - goalHr*3600)/60;
    int goalSec = LIMIT - goalHr*3600 - goalMin*60;
    sprintf(goalTimeString, "%02d:%02d:%02d", goalHr, goalMin, goalSec);
    lcd.print("Goal: ");
    lcd.print(goalTimeString); //Shows the goal time in a "Goal: 00:00:00" format
  } else { //This is the case as soon as the milestone is hit, showing the victory screen
    lcd.clear();
    lcd.print(" \x03 "); //Custom char (from createChar())
    lcd.print("Congrats!");
    lcd.print(" \x03 "); //Custom char (from createChar())
  }
}

void setup() { //Setting up servo, analog pins, lcd, etc...
  Serial.begin(9600);
  pinMode(A0, INPUT);
  arm.attach(9);
  arm.write(pos);
  lcd.begin(16, 2);
  lcd.createChar(3, customChar);
}

void loop() { //Frequently checking the guitar input into our pedal to see how we should update the screen and/or give candy with the servo
  int voltage = readInput();
  bool dispenced = checkCandyDispense(voltage);
  updateScreen(dispenced);
  delay(1000);
}
