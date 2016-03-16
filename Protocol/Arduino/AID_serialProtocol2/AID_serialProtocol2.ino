#include <SoftwareSerial.h>
#include "I2Cdev.h"

//Arduino Due pins: 20 (SDA), 21 (SCL), SDA1, SCL1

#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  #include "Wire.h"
#endif

MPU6050 mpu;

#define OUTPUT_READABLE_YAWPITCHROLL

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

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

int mpuYaw = 0;
int mpuPitch = 0;
int mpuRoll = 0;

char sensorSend[21];
//char sensor2Send[4];
//String sensorsString = "";
char mpuYPRSend[21];

SoftwareSerial SerialBT(10, 11); // TX, RX

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  SerialBT.begin(9600);

  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    TWBR = 24;
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  mpu.initialize();
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
  devStatus = mpu.dmpInitialize();

  if (devStatus == 0) {
    mpu.setDMPEnabled(true);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    Serial.print(devStatus);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  //Reading the sensor values
  sensor1Value = analogRead(sensor1Pin);
  sensor2Value = analogRead(sensor2Pin);  
  sensor3Value = analogRead(sensor3Pin);
  sensor4Value = analogRead(sensor4Pin);
  sensor5Value = analogRead(sensor5Pin);
  
  // sprintf(sensorSend, "A%dB%dC%dD%dE%d,", sensor1Value, sensor2Value, sensor3Value, sensor4Value, sensor5Value);
  
  //strcat(sensorsString, 'F');
  //strcat(sensorsString, str(sensor1Value));
  //strcat(sensorsString, ',');
  //strcat(sensorsString, str(sensor2Value));
  
  
  //SerialBT.print(sensorSend);
  //SerialBT.print(sensorsString);
  //Serial.print(sensorSend);
  //SerialBT.print(sensor2Send);
  //Serial.print(sensor2Send);
  // 
  //digitalWrite(ledPin, HIGH);
  //delay(sensor1Value);
  //digitalWrite(ledPin, LOW);

    if (dmpReady) {
        
    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
      
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));
        
      } else if (mpuIntStatus & 0x02) {
        
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            Serial.print("\nypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI);
            mpuYaw = ypr[0] * 180/M_PI;
            mpuPitch = ypr[1] * 180/M_PI;
            mpuRoll = ypr[2] * 180/M_PI;

//            Serial.print("\nypr\t");
//            Serial.print(ypr[0]);
//            Serial.print("\t");
//            Serial.print(ypr[1]);
//            Serial.print("\t");
//            Serial.println(ypr[2]);

        #endif
      }
    }
    // send the serial message 
    
    
    sprintf(sensorSend, "A%dB%dC%dD%dE%dY%dP%dR%d,", sensor1Value, sensor2Value, sensor3Value, sensor4Value, sensor5Value, char(mpuYaw), char(mpuPitch), char(mpuRoll));
    
    SerialBT.print(sensorSend);
    
    Serial.print(sensorSend);
}
