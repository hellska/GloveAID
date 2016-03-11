#include <SoftwareSerial.h>

int sensor1Pin = A0;
int sensor2Pin = A1;
int sensor3Pin = A2;
int sensor4Pin = A3;
int sensor5Pin = A4;

int ledPin = 13;

int sensor1Value = 0;
int sensor2Value = 0;
int sensor3Value = 0;
int sensor4Value = 0;
int sensor5Value = 0;

char sensorSend[21];
//char sensor2Send[4];
//String sensorsString = "";

SoftwareSerial SerialBT(10, 11); // TX, RX

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  SerialBT.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  //Reading the sensor values
  sensor1Value = analogRead(sensor1Pin);
  sensor2Value = analogRead(sensor2Pin);  
  sensor3Value = analogRead(sensor3Pin);
  sensor4Value = analogRead(sensor4Pin);
  sensor5Value = analogRead(sensor5Pin);
  
  sprintf(sensorSend, "A%dB%dC%dD%dE%d,", sensor1Value, sensor2Value, sensor3Value, sensor4Value, sensor5Value);
  
  //strcat(sensorsString, 'F');
  //strcat(sensorsString, str(sensor1Value));
  //strcat(sensorsString, ',');
  //strcat(sensorsString, str(sensor2Value));
  
  
  SerialBT.print(sensorSend);
  //SerialBT.print(sensorsString);
  Serial.print(sensorSend);
  //SerialBT.print(sensor2Send);
  //Serial.print(sensor2Send);
  // 
  //digitalWrite(ledPin, HIGH);
  //delay(sensor1Value);
  //digitalWrite(ledPin, LOW);
}
