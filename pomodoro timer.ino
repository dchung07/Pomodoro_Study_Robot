#include <Wire.h>
#include <Digital_Light_TSL2561.h>
#include "rgb_lcd.h"
#define ledLight A0
const int buttonPin = A1;
bool pomodoroBool = false;
bool timerPaused = false;
unsigned long pauseTime = 0;
unsigned long buttonPressTime = 0;
#define ledPin A2
const int buzzer = A3;
rgb_lcd lcd;

int pomodoroCount = 0;

const int miniButtonPin = 4;
unsigned long miniButtonPressTime = 0;
const int colorR = 0;
const int colorG = 255;
const int colorB = 0;

//Pomodoro Timer Variables
const int minutes = 1;
const int seconds_per_minute = 60;
int remainingSeconds = minutes * seconds_per_minute;

//Timer Game Variables
unsigned long start = 0;
unsigned long end = 0;
unsigned long totalTime = 0;
unsigned long randomTimer = 0;
unsigned long currentTime = 0;
unsigned long FinalTime = 0;

int  sleepyFace[2][16]={
 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
 {0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0}
};

int  happyFace[2][16]={
 {0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0},
 {0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0}
};

int  angryFace[2][16]={
 {0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0},
 {0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0}
};

int  sadFace[2][16]={
 {0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0},
 {0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0}
};

int  normalFace[2][16]={
 {0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0},
 {0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,0}
};

String messagesTop[4] = {
  "Have a Good Day!",
  "You are the Best!",
  "You can Do It!",
  "Dream Bigger."
};

String messagesBottom[4] = {
  "Believe you can.",
  "Touch some Grass",
  "You're a Beast",
  "Make it Count!"
};

void setup() {
    Wire.begin();
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);
    pinMode(ledLight, INPUT);
    pinMode(buttonPin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(miniButtonPin, INPUT);
    Serial.begin(9600);

}

void loop() {
      delay(200);

  if(digitalRead(miniButtonPin) == LOW) {
    miniButtonPressTime = millis();
  }

  // Check if the button is pressed
  if (digitalRead(miniButtonPin) == HIGH && !pomodoroBool) {
    pomodoroBool = true;
  } 

  if(pomodoroBool){
    pomodoro();
  }



  // Check if the button is pressed
  if (digitalRead(buttonPin) == LOW) {
    buttonPressTime = millis(); // Record the time the button was pressed
  }

  // Check if the button has been held down for more than 2 seconds
  if (digitalRead(buttonPin) == HIGH && millis() - buttonPressTime >= 2000) {
    // Timer game begins if the button has been held down
    timerGameIntro();
    //Timer Game
    timerGame();
  }

  // Check if the button is pressed
  if (digitalRead(buttonPin) == HIGH) {
    motivationalMessages();
  } 



  if(!pomodoroBool) {
    faces();
  }

}

void motivationalMessages(){
  int randomNumberTop = random(4);
  int randomNumberBot = random(4);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(messagesTop[randomNumberTop]);
  lcd.setCursor(0, 1);
  lcd.print(messagesBottom[randomNumberBot]);
  delay(5000);
}

void pomodoroReset(){
    lcd.clear();
    pomodoroBool = false;
    remainingSeconds = minutes * seconds_per_minute;
    lcd.clear();
    delay(500);
    faces();
}

void timerGameIntro(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Timer Game Starts");
    lcd.setCursor(0, 1);
    lcd.print("Press when Light");

    delay(1000);
}

void timerGame(){
  Serial.println(ledLight);
    randomTimer = random(2000, 5000);
    currentTime = millis();
    totalTime = currentTime;

    start = millis();

    while (digitalRead(buttonPin) == LOW) {
      currentTime = millis();
      if (currentTime - totalTime >= randomTimer && digitalRead(buttonPin) == LOW) {
        digitalWrite(ledPin, HIGH);
      } else if (currentTime - totalTime >= randomTimer && digitalRead(buttonPin) == HIGH) {
        break;
      }
    }

    end = millis();

    lcd.clear();
    if (digitalRead(ledPin) == HIGH) {
      totalTime = (end - randomTimer) - start;
      // lcd.print("Good work! Time taken was ");

      if(totalTime < 150) {
        lcd.print("Perfect!");
        lcd.setCursor(0, 1);
        lcd.print("MS: ");
        lcd.print(totalTime);
        delay(2000);
      } else if(totalTime < 300) {
          lcd.print("Good Work!");
          lcd.setCursor(0, 1);
          lcd.print("MS: ");
          lcd.print(totalTime);
          delay(2000);
      } else if(totalTime < 600) {
          lcd.print("Nice Attempt!");
          lcd.setCursor(0, 1);
          lcd.print("MS: ");
          lcd.print(totalTime);
          delay(2000);
      } else if(totalTime <  1000) {
          lcd.print("Could Be Better!");
          lcd.setCursor(0, 1);
          lcd.print("MS: ");
          lcd.print(totalTime);
          delay(2000);
      } else {
          lcd.print("Too Slow!");
          lcd.setCursor(0, 1);
          lcd.print("MS: ");
          lcd.print(totalTime);
          delay(2000);
      }
    } else {
      lcd.print("Fail!");
      lcd.setCursor(0, 1);
      lcd.print("Early Start!");
      delay(2000);
    }

    digitalWrite(ledPin, LOW);
    lcd.clear();
    delay(100);
}

void pomodoro(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Study Timer:");

  lcd.setCursor(0, 1);
  lcd.print("     ");
  lcd.setCursor(0, 1);
  lcd.print(remainingSeconds / seconds_per_minute);
  lcd.print(":");
  if (remainingSeconds % seconds_per_minute < 10) {
    lcd.print("0");
  }
  lcd.print(remainingSeconds % seconds_per_minute);
  delay(1000);

  remainingSeconds--;

  if (remainingSeconds <= 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time is up!");
    buzzerBeep();
    delay(5000);
    remainingSeconds = minutes * seconds_per_minute;
    pomodoroBool = false;
  } 
  if(digitalRead(miniButtonPin) == HIGH) {
    pomodoroReset();
  }

  // Check if the button has been held down for more than 2 seconds
  if (digitalRead(miniButtonPin) == HIGH && millis() - miniButtonPressTime >= 1000) {
      // pomodoroReset();
      pomodoroCount++;
          lcd.clear();
          pomodoroBool = false;
          remainingSeconds = (minutes + (5 * pomodoroCount)) * seconds_per_minute;
          lcd.clear();
      if(pomodoroCount == 5) {
        pomodoroCount = 0;
        pomodoroReset();
      }
  }
}

void neutral(){
  printFace(normalFace);
}
void happy(){
  printFace(happyFace);
}

void angry(){
  printFace(angryFace);
}

void sad(){
  printFace(sadFace);
}

void sleepy(){
  printFace(sleepyFace);
}

void printFace(int emot[][16]){
  lcd.clear();
  for(int i=0;i<2;i++){
    for(int j=0; j<16;j++){
      lcd.setCursor(j, i);    
      if(emot[i][j]==1){
        lcd.write(255);
      }
    }
  }
}

void buzzerBeep(){
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(1000);        // ...for 1sec
}

void faces(){
  int value = analogRead(A0);
  if(value <= 100) //Tired Face
    { 

      lcd.clear();
      sleepy();
    } else if (value <= 200) //Normal Face
    { 
 
      lcd.clear();
      neutral();
    } else if (value <= 300) //Just Right
    { 

      lcd.clear();
      happy();
    } else if (value <= 400) 
    { 
      lcd.clear();
      sad();
    } else {

      lcd.clear();
      angry();
    }
}