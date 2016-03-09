import processing.serial.*;
Serial imputPort;
int value;
boolean sensor1 = false;
boolean sensor2 = false;
String s1 = "";
String s2 = "";
char input;
int counter = 0;


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
}

void draw() {
  
}

void serialEvent(Serial p) {
  // get message till line break (ASCII > 13)
  // signal = input.read();
  // String message = p.readStringUntil(13);
    

    value = p.read();
    //println(char(value));
    input = char(value);
    
        
    switch(input) {
      case 'A':
        sensor1 = true;
        sensor2 = false;
        counter = 0;  
        println("sensor 2: ",s2);
        s1 = "";
        break;
      case 'B':
        sensor2 = true;
        sensor1 = false;
        counter = 0;  
        println("sensor 1: ",s1);
        s2 = "";
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
        break;
    }
}