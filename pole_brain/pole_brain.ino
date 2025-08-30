/**
 * @file Pole System Code
 *
 * @brief Contains the logic and code necessary to run the created Alarm
 * System model of the overall design.  Also includes the necessary
 * connections and setups to connect this Uno with the similar User 
 * Interface for sharing info across both boards and systems.
 */

// Include libraries and definitions used
#include <Servo.h>
#define println Serial.println


// Set up global variables used throughout program
const int servoPin = 13;
Servo myServo;
const int alarmPin = 8;

const int RECIEVE_offPin = 11;
const int RECIEVE_resetPin = 10;
int off = false;
int reset = false;
bool on_state = false;

float water_level = 0;
int water_percentage = 0;
bool alarm = false;


// Set up code for running on startup
void setup(){
  // Servo
  myServo.attach(servoPin);
  myServo.write(0);

  // Set pin modes
  pinMode(alarmPin, OUTPUT);
  digitalWrite(alarmPin, LOW);

  pinMode(RECIEVE_offPin, INPUT);
  pinMode(RECIEVE_resetPin, INPUT);

  // Serial.print functionality for testing
  Serial.begin(9600);
}

// Main section ran during execution of program
void loop(){
  // Check water info
  water_level = analogRead(0);
  water_percentage = static_cast<int>((water_level / 800)*100);
  println(water_percentage);

  // Check button states from user interface
  off = digitalRead(RECIEVE_offPin);
  reset = digitalRead(RECIEVE_resetPin);

  if (off == HIGH){on_state = false; alarm = false;}
  else if (reset == HIGH){on_state = true; alarm = false;}

  delay(100);

  // If not off (ON pushed), check for flood levels and execute alarm
  // if detected
  if (on_state){
    if (water_percentage > 60) {alarm = true;}
    else{alarm = false;}

    while (alarm == true){
      tone(alarmPin, 1000);
      myServo.write(180);
      off = digitalRead(RECIEVE_offPin);
      if (off == HIGH){on_state = false; alarm = false;}
    }
    noTone(alarmPin);
    myServo.write(0);
    } else{
    // Make sure everything turned off if system off
    myServo.write(0);
    alarm = false;
    noTone(alarmPin);
  }
}