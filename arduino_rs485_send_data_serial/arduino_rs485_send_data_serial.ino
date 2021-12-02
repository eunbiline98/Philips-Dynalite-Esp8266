#include <SoftwareSerial.h>
// assemble message
byte lamp_on [] = {
  0x1C,
  0x01,
  0x00,
  0x6B,
  0x00,
  0x64,
  0xFF,
  0x15
};
byte lamp_off [] = {
  0x1C,
  0x01,
  0x00,
  0x6B,
  0x0A,
  0x64,
  0xFF,
  0x0B
};

#define DirectionControl 9         // define the EIA-485 transmit driver pin
SoftwareSerial rs485(8, 9);       // receive pin, transmit pin

void setup() {
  pinMode(DirectionControl, OUTPUT);  // driver output enable
  rs485.begin (9600);                 // vitesse com
  Serial.begin(9600);
}
void loop() {
  for (int x = 0; x < 2000; x++) {
    delay(1);
    if (x == 1500) {
      active_high();
    }
  }

  for (int x = 0; x < 2000; x++) {
    delay(1);
    if (x == 1500) {
      active_low();
    }
  }
}
void active_high() {
  digitalWrite (DirectionControl, HIGH);     // enable transmit driver
  for (int i = 0; i < 8; i++) {
    Serial.print(lamp_on[i], HEX);          // console print the data
    rs485.write(lamp_on[i]);                // flip display print
    if (i < 7) {
      Serial.print(", ");             // print a comma between data for console only :smiley-draw:
    }
  }
  Serial.println();
  digitalWrite(DirectionControl, LOW);       // disable transmit driver
  delay(500);
}

void active_low() {
  digitalWrite (DirectionControl, HIGH);     // enable transmit driver
  for (int i = 0; i < 8; i++) {
    Serial.print(lamp_off[i], HEX);          // console print the data
    rs485.write(lamp_off[i]);                // flip display print
    if (i < 7) {
      Serial.print(", ");             // print a comma between data for console only :smiley-draw:
    }
  }
  Serial.println();
  digitalWrite(DirectionControl, LOW);       // disable transmit driver
  delay(500);
}
