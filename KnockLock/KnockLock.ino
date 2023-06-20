#include <Servo.h>

Servo servo;

const int piezo = A0;
const int switchPin = 2;
const int yellowLed = 3;
const int greenLed = 4;
const int redLed = 5;
const int n = 3;
long time_to_switch_LED = 0;

// analog read
int knockVal;
// digital read
int switchVal;

// variables for the high and low limits of the knock value
const int low = 400;
const int high = 5500;

bool locked = false;
int numberOfKnocks = 0;

void setup() {
  servo.attach(9);

  // LED Outputs
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);


  pinMode(switchPin, INPUT);

  Serial.begin(9600);
  
  digitalWrite(greenLed, HIGH);

  Serial.println("Unlocked");
}

void loop() {

  // if unlocked
  if (locked == false) {
    switchVal = digitalRead(switchPin);

    // if the button is pressed, lock
    if (switchVal == HIGH) {
      locked = true;  

      // change the status LEDs
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);

      // move the servo to the locked position
      servo.write(153);
      delay(1000);
      servo.write(93);
      
      // print out status
      Serial.println("Locked");
      
      delay(1000);
    }
  }

  // if locked
  if (locked == true) {

    knockVal = analogRead(piezo);

    if (numberOfKnocks < n) {
      
      if (checkForKnock(knockVal) == true) {

        // increment the number of valid knocks
        numberOfKnocks++;
      }

      // print status of knocks
      Serial.print(n - numberOfKnocks);
      Serial.println(" more knocks to go");
    }

    // if there are three knocks
    if (numberOfKnocks >= n) {
      // unlock the box
      locked = false;

      // move the servo to the unlocked position
      servo.write(33);
      delay(1000);
      servo.write(93);

      // wait for it to move
      delay(20);

      // change status LEDs
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
      Serial.println("Unlocked");

      numberOfKnocks = 0;
      delay(10000);
      locked = true;
      servo.write(153);
      delay(1000);
      servo.write(93);
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println("Locked");
      
    }
  }
}

bool checkForKnock(int value) {
  if (value >=950  ) {
    // turn the status LED on
    digitalWrite(yellowLed, HIGH);
    delay(50);
    digitalWrite(yellowLed, LOW);
    // print out the status
    Serial.print("Valid knock of value ");
    Serial.println(value);
    // return true
    return true;
  }
  // if the knock is not within range
  else {
    // print status
    Serial.print("Bad knock value ");
    Serial.println(value);
    // return false
    return false;
  }   
}
