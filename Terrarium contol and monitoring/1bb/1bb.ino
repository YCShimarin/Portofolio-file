#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address might differ, adjust accordingly

// Pins for relays
#define RELAY_DOORLOCK 3   // Relay pin for door lock solenoid
#define RELAY_VALVE 4       // Relay pin for valve solenoid
#define RELAY_MIST_MAKER 2 // Relay pin for mist maker

// DC Fan pin
#define DC_FAN_PIN 7

// Servo pins
#define SERVO1_PIN 9
#define SERVO2_PIN 10

// Servo objects
Servo servo1;
Servo servo2;

void setup() {
  Serial.begin(9600); // Start serial communication with baud rate 9600

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize pins for relays
  pinMode(RELAY_DOORLOCK, OUTPUT);
  pinMode(RELAY_VALVE, OUTPUT);
  pinMode(RELAY_MIST_MAKER, OUTPUT);

  // Turn off all relays initially
  digitalWrite(RELAY_DOORLOCK, LOW);
  digitalWrite(RELAY_VALVE, LOW);
  digitalWrite(RELAY_MIST_MAKER, LOW);

  // Initialize DC Fan pin
  pinMode(DC_FAN_PIN, OUTPUT);
  digitalWrite(DC_FAN_PIN, LOW); // Turn off fan initially

  // Initialize servos
  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  // Set servos to initial position
  servo1.write(90); // Center position
  servo2.write(90); // Center position
}

void loop() {
  if (Serial.available() >= 6) { // Adjust the number of bytes received

    String dataString = Serial.readStringUntil('\n');

    int soilMoistureValue = 0;
    int mq135Value = 0;
    int luxmeter = 0;
    int humidity = 0;  // Using int for simplicity
    int temperature = 0;  // Using int for simplicity
    int touchVal = 0;

    Serial.println(dataString);

    int commaIndex1 = dataString.indexOf(',');
    int commaIndex2 = dataString.indexOf(',', commaIndex1 + 1);
    int commaIndex3 = dataString.indexOf(',', commaIndex2 + 1);
    int commaIndex4 = dataString.indexOf(',', commaIndex3 + 1);
    int commaIndex5 = dataString.indexOf(',', commaIndex4 + 1);

    // Convert string to integer
    soilMoistureValue = dataString.substring(0, commaIndex1).toInt();
    mq135Value = dataString.substring(commaIndex1 + 1, commaIndex2).toInt();
    luxmeter = dataString.substring(commaIndex2 + 1, commaIndex3).toInt();
    humidity = dataString.substring(commaIndex3 + 1, commaIndex4).toInt();
    temperature = dataString.substring(commaIndex4 + 1, commaIndex5).toInt();
    touchVal = dataString.substring(commaIndex5 + 1).toInt();

    // Display data on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Soil: ");
    lcd.print(soilMoistureValue);
    lcd.print("%");

    lcd.setCursor(10, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");

    lcd.setCursor(0, 2);
    lcd.print("Light: ");
    lcd.print(luxmeter);

    lcd.setCursor(10, 2);
    lcd.print("Touch: ");
    lcd.print(touchVal);

    lcd.setCursor(0, 3);
    lcd.print("MQ135: ");
    lcd.print(mq135Value);

    // Control door lock solenoid based on touch sensor status
    if (touchVal == 1) {
      // If touch sensor is activated, turn on door lock relay
      digitalWrite(RELAY_DOORLOCK, HIGH);
      delay(1000);
    } else {
      // Turn off door lock relay
      digitalWrite(RELAY_DOORLOCK, LOW);
    }

    // Control DC fan based on temperature
    if (temperature > 27) {
      // If temperature is above 25 degrees, turn on fan
      digitalWrite(DC_FAN_PIN, HIGH);
    } else {
      // Turn off fan
      digitalWrite(DC_FAN_PIN, LOW);
    }

    // Control servos based on light sensor status
    if (mq135Value < 350 ) {
      // If light intensity is in range, set servos to center position
      servo1.write(90);
      servo2.write(90);
    } else {
      // Turn servos to position 0
      servo1.write(0);
      servo2.write(0);
    }

    // Control relay for mist maker based on humidity
    if (humidity < 50) {
      // Turn on relay for mist maker if humidity is greater than 50%
      digitalWrite(RELAY_MIST_MAKER, HIGH);
    } else {
      // Turn off relay for mist maker
      digitalWrite(RELAY_MIST_MAKER, LOW);
    }

    // Control valve solenoid based on soil moisture value
    if (soilMoistureValue <= 70) {
      // Turn on valve relay if soil moisture is in range
      digitalWrite(RELAY_VALVE, LOW);
    } else{
      // Turn off valve relay
      digitalWrite(RELAY_VALVE, HIGH);
    }
  }
}
