#include <SoftwareSerial.h>

int sensor1Pin = A0;
int sensor2Pin = A1;

int ledPin = 13;

int sensor1Value = 0;
int sensor2Value = 0;

char sensorSend[9];
char sensor2Send[4];
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
  sensor1Value = analogRead(sensor1Pin);
  sensor2Value = analogRead(sensor2Pin);
  
  sprintf(sensorSend, "A%dB%d", sensor1Value, sensor2Value);
  
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
