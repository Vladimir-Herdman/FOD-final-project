/**
 * @file User Interface Code
 *
 * @brief Contains the logic and code necessary to run the created User
 * Interface model in the overall design.  Also includes the necessary
 * connections and setups to connect this Uno with the similar pole Uno
 * for sharing info across both boards and systems.
 */

// Include libraries and definitions used
#include <LiquidCrystal.h>

#define println Serial.println


// Set up global variables used throughout program
const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int SEND_offPin = 13;
const int SEND_resetPin = 11;
const int offPin = 10;
const int resetPin = 12;
int off = false;
int reset = false;
bool on_state = false;

float water_level = 0;
int water_percentage = 0;
int lastLED = 0;
bool alarm = false;

// Set up code for running on startup
void setup(){
  // Set lights off
  for (int i = 2; i <= 3; i++){pinMode(i, OUTPUT);digitalWrite(i, LOW);}

  // LCD Screen and display
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("System: ");
  lcd.setCursor(8, 0);
  lcd.print("OFF");
  lcd.setCursor(0, 1);
  lcd.print("Depth: ");

  // Set pin modes
  pinMode(SEND_offPin, OUTPUT);
  pinMode(SEND_resetPin, OUTPUT);
  digitalWrite(SEND_offPin, HIGH);
  digitalWrite(SEND_resetPin, LOW);

  pinMode(offPin, INPUT);
  pinMode(resetPin, INPUT);

  // Serial moniter baud rate for debugging and data testing
  Serial.begin(9600);
}

// Main section ran during execution of program
void loop(){
  // Get water level and convert to percentage of water sensor depth
  water_level = (analogRead(0));
  water_percentage = static_cast<int>((water_level / 800)*100);
  //println(water_percentage);

  // Normalize water_percentage readings for later showing
  if (water_percentage > 100){
    water_percentage = 100;
  }
  if (water_percentage < 0){
    water_percentage = 0;
  }

  // Show on user interface depth percentage
  lcd.setCursor(8, 1);
  lcd.print(water_percentage);
  if (water_percentage > 9){
    lcd.print("%");
  } else{
    lcd.print("% ");
  }

  // Get button readings for OFF and ON (reset)
  off = digitalRead(offPin);
  reset = digitalRead(resetPin);

  if (off == HIGH){on_state = false; digitalWrite(SEND_offPin, HIGH); digitalWrite(SEND_resetPin, LOW);}
  else if (reset == HIGH){on_state = true; digitalWrite(SEND_offPin, LOW); digitalWrite(SEND_resetPin, HIGH);}

  // Update LCD screen with current system status
  lcd.setCursor(8, 0);
  if (off == HIGH){
    lcd.print("OFF");
  } else if (reset == HIGH) {
    lcd.print("ON ");
  }

  delay(100);
  
  // Adjust lights and system status on collected water info
  if (on_state) {
    if (water_percentage > 60) {lastLED = 3; alarm = true;}
    else{lastLED = 2; alarm = false;}

    lcd.setCursor(8, 1);
    lcd.print(water_percentage);

    if (water_percentage < 25){
        for (int i = 2; i <= 3; i++){pinMode(i, OUTPUT);digitalWrite(i, LOW);}
      } else{
        for (int i = 2; i <= lastLED; i++){pinMode(i, OUTPUT);digitalWrite(i, HIGH);}
      }
  } else {
    alarm = false;
    for (int i = 2; i <= 3; i++){pinMode(i, OUTPUT);digitalWrite(i, LOW);} 
  }
}