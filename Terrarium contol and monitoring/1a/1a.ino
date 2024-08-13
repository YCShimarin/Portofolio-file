#include <DHT.h>
// #include <MQ135.h>

#define DHTPIN 2      // Pin where DHT11 is connected
#define TOUCHPIN 3    // Pin where touch sensor is connected
#define DHTTYPE DHT11 // Type of DHT sensor
#define SOILPIN A0
#define MQ135PIN A1
#define LDRPIN A2

#define RL 10 // Load resistance value on the sensor
#define m -0.417 // Gradient value
#define b 0.858 // Intercept value
#define RO 16 // Measured RO value

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); // Start serial communication with baud rate 9600
  dht.begin();        // Initialize the DHT sensor
  pinMode(TOUCHPIN, INPUT); // Set touch sensor pin as input
}

void loop() {
  // Read soil moisture sensor
  int soilMoistureValue = analogRead(SOILPIN);
  // Convert the analog value to a scale of 1-100
  soilMoistureValue = map(soilMoistureValue, 0, 1023, 1, 100);

  // Read MQ-135 sensor
  float mq135Value = analogRead(MQ135PIN);
  float VRL = mq135Value * (5.0 / 1023.0); // Convert analog value to voltage
  // float RS = ((5.0 * RL) / VRL) - RL; // Calculate RS
  // float ratio = RS / RO; 
  // float gas = pow(10, ((log10(ratio) - b) / m)); 
  // float ppm = abs(gas - 0.14);

  // Read LDR sensor
  int ldrValue = analogRead(LDRPIN);
  // float voltage = (float)ldrValue * (5.0 / 1023.0);
  // float Rldr = (7 * voltage) / (5 - voltage);
  // float ldrmap = abs(-90 * ((Rldr - 5) / 15) + 100);
  int ldrmap = map(ldrValue, 0, 1023, 1, 100);
 
  // Read temperature and humidity from DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Read status of touch sensor
  int touchStatus = digitalRead(TOUCHPIN);
  int touchVal = (touchStatus == HIGH) ? 1 : 0;

  // Create a comma-separated string with all sensor values
  String dataString = String(soilMoistureValue) + "," + String(mq135Value) + "," +
                      String(ldrmap) + "," + String(humidity) + "," + String(temperature) + "," + String(touchVal);

  // Print the data string to the serial monitor
  Serial.println(dataString);

  // Delay before the next reading
  delay(1000);
}
