import processing.serial.*;
import oscP5.*;
import netP5.*;

OscP5 AID_serial_osc;
NetAddress wekinatorOut;

Serial imputPort;
int value;
boolean sensor1 = false;
boolean sensor2 = false;
boolean sensor3 = false;
boolean sensor4 = false;
boolean sensor5 = false;
String s1 = "";
String s2 = "";
String s3 = "";
String s4 = "";
String s5 = "";
char input;
int counter = 0;
int now = 0;
boolean send;


void setup() {
 boolean isSerial = false;
 for(int i=0;i<Serial.list().length;i++) {
    // show all the serial ports available
    //println(Serial.list()[i]);
    // Connect only to an usbserial if 
    String[] m1 = match(Serial.list()[i], "tty.HC-06");
    if (m1 != null) {  // If not null, then a match was found
      // This will print to the console, since a match was found.
      println("Found usbserial '" + Serial.list()[i] + "'");
      imputPort = new Serial(this, Serial.list()[i], 9600);
      isSerial = true;    
    } 
    
  }  
  if (!isSerial) {
     println("No usb serial found");
  }
  AID_serial_osc = new OscP5(this, 9000);
  //wekinatorOut = new NetAddress("127.0.0.1", 6448);
  wekinatorOut = new NetAddress("192.168.0.159", 6448);
  size(200,200);
  now = millis();
}
  
void draw() {
  // frameRate(25);  
  // ellipse(100,100,random(50),random(50));
  //if (send) {
  //  OscMessage sensorsList = new OscMessage("/wek/inputs");
  //  sensorsList.add(s1);
  //  sensorsList.add(s2);
  //  sensorsList.add(s3);
  //  sensorsList.add(s4);
  //  sensorsList.add(s5);
  //  AID_serial_osc.send(sensorsList, wekinatorOut);
  //  send = false;
  //  println("Message sent");
  //}
}

void serialEvent(Serial p) {
  // get message till line break (ASCII > 13)
  // signal = input.read();
  // String message = p.readStringUntil(13);
  if (millis()-now>10000) {
    

    value = p.read();
    //println(char(value));
    input = char(value);
    
        
    switch(input) {
      case 'A':
        sensor1 = true;
        counter = 0;
        s1 = "";
        break;
      case 'B':
        sensor2 = true;
        sensor1 = false;
        counter = 0;
        s2 = "";
        break;
      case 'C':
        sensor3 = true;
        sensor2 = false;
        counter = 0;
        s3 = "";
        break;
      case 'D':
        sensor4 = true;
        sensor3 = false;
        counter = 0;
        s4 = "";
        break;
      case 'E':
        sensor5 = true;
        sensor4 = false;
        counter = 0;  
        //println("sensor 1: ",s1);
        s5 = "";
        break;
      case ',':
        println("sensor 1: ",s1);
        println("sensor 2: ",s2);
        println("sensor 3: ",s3);
        println("sensor 4: ",s4);
        println("sensor 5: ",s5);
        println("NEXT SET ");
        sensor5 = false;
        send = true;
        OscMessage sensorsList = new OscMessage("/wek/inputs");
        sensorsList.add(float(s1));
        sensorsList.add(float(s2));
        sensorsList.add(float(s3));
        sensorsList.add(float(s4));
        sensorsList.add(float(s5));
        AID_serial_osc.send(sensorsList, wekinatorOut);
        //send = false;
   println("Message sent");
        break;
      default:
        if (sensor1) {
          s1 += input;
          counter +=1;
        }
        else if (sensor2) {
          s2 += input;
          counter +=1;
        }
        else if (sensor3) {
          s3 += input;
          counter +=1;
        }
        else if (sensor4) {
          s4 += input;
          counter +=1;
        }
        else if (sensor5) {
          s5 += input;
          counter +=1;
        }
        break;
    }
  }
}