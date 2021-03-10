#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); // RX, TX

//parameters for flexsensors
int FirstShotX , FirstShotY;

//Pins of buttons
const int buttonPin1 = 4;     // the number of the pushbutton pin
const int buttonPin2 = 5;
const int buttonPin3 = 2;
const int buttonPin4 = 3;
const int buttonPin5 = 6;


/**
 * 
 * 0 ist ringfinger
 * 1 ist kleiner finger
 * 2 ist zeigefinger
 * 3 ist mittelfinger
 * 
 */


//Pins of flex sensor
const int flexPin1 = 0;
const int flexPin2 = 1;
const int flexPin3 = 3;
const int flexPin4 = 2;

//Pins of joystick
const int joyPin1 = 4;
const int joyPin2 = 5;

// initialize button state:
int buttonState1 = 0;         // variable for reading the pushbutton status
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int buttonState5 = 0;


void setup() {
//  // Open serial communications and wait for port to open:
//  Serial.begin(9600); //hardware serial start
//
  FirstShotX = 0;
  FirstShotY = 0;
//
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200); //softwareserial start
  delay(100); //waiting...
  mySerial.println("AT+CWMODE=3");//set mode to access ponit+station
  delay(500);
  mySerial.println("AT+CWJAP=\"CAVE\",\"virtual.reality\"");//try to connect with access point or router with "ssid","password"
  //mySerial.println("AT+CWJAP=\"HonorAP\",\"12345678\"");
  delay(10000);
  //mySerial.println("AT+CIPSTART=\"TCP\",\"192.168.1.66\",3490");//TCP connection and port number is "3490"
  mySerial.println("AT+CIPSTART=\"TCP\",\"192.168.100.1\",3490");
  delay(5000);
  mySerial.println("AT+CIPSEND=2");//send message ,length=2
  delay(5000);
  mySerial.println("69");//message is 69
  delay(5000);
  mySerial.println("AT+CIPSEND=10");
  delay(10);
  mySerial.println("SetupDone");
}

void loop() { // run over and over
  //here to transport sensordata
  //read the buttonState1 of button 1
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);
  buttonState5 = digitalRead(buttonPin5);
  String data = "";
  data += (String)buttonState1;
  data += (String)buttonState2;
  data += (String)buttonState3;
  data += (String)buttonState4;
  data += (String)buttonState5;

  //read the sensorvalue of flexsensors
  int flexValue1;
  int flexValue2;
  int flexValue3;
  int flexValue4;
  flexValue1 = analogRead(flexPin1);
  flexValue2 = analogRead(flexPin2);
  flexValue3 = analogRead(flexPin3);
  flexValue4 = analogRead(flexPin4);

  int additional = 100;

  int max = 1000 + additional;
  int min = 490 + additional;
  int div = 2;
  int diff = 373 + additional;
  
  if (flexValue1 < min)
    flexValue1 = -128;
  else if (flexValue1 > max)
    flexValue1 = 127;
  else
    flexValue1 = flexValue1 / div - diff - 12;

  if (flexValue2 < min)
    flexValue2 = -128;
  else if (flexValue2 > max)
    flexValue2 = 127;
  else
    flexValue2 = flexValue2 / div - diff - 12;

  if (flexValue3 < min)
    flexValue3 = -128;
  else if (flexValue3 > max)
    flexValue3 = 127;
  else
    flexValue3 = flexValue3 / div - diff;

  if (flexValue4 < min)
    flexValue4 = -128;
  else if (flexValue4 > max)
    flexValue4 = 127;
  else
    flexValue4 = flexValue4 / div - diff - 17;

  data += (char)flexValue1;
  data += (char)flexValue2;
  data += (char)flexValue3;
  data += (char)flexValue4;

  //joyStick
  int joyStickX, joyStickY;
  int sensorValueX = analogRead(4);
  if (FirstShotX == 0)
  {
    FirstShotX = sensorValueX;
  }
  joyStickX = (sensorValueX - FirstShotX) / 4 - 90;
  if (joyStickX > 127)
    joyStickX = 127;
  if (joyStickX < -128)
    joyStickX = -128;

  int sensorValueY = analogRead(5);
  if (FirstShotY == 0)
  {
    FirstShotY = sensorValueY;
  }
  joyStickY = (sensorValueY - FirstShotY) / 1;
  if (joyStickY > 127)
    joyStickY = 127;
  if (joyStickY < -128)
    joyStickY = -128;


  data += (char)joyStickX;
  data += (char)joyStickY;


  mySerial.println("AT+CIPSEND=11");//send message ,length=11
  delay(10);
  mySerial.println(data);
  delay(20);
}
