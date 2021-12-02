#include<SoftwareSerial.h>
#define DirectionControl 3
#define indicator 13
int rx;

SoftwareSerial RS485(8, 9);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("configuring serial:\n");
  RS485.begin(9600);
  Serial.println("configuring RS485" );
  Serial.println("set up done!!");

  pinMode(indicator, OUTPUT);
  pinMode(DirectionControl, OUTPUT);

  digitalWrite(DirectionControl, LOW); //preparing for receiption
}

void loop() {
  // put your main code here, to run repeatedly:
  if (RS485.available())
  {
    rx = RS485.read();   // Read the byte

    digitalWrite(indicator, HIGH);  // Show activity
    delay(1000);
    digitalWrite(indicator, LOW);
    delay(1000);
    Serial.print("Data Masuk :");
    Serial.println(rx, HEX);
  }
  else {
    Serial.println("Tidak Ada Data Masuk!!!");  // print this if serial is not available
    delay(1000);
  }
}
