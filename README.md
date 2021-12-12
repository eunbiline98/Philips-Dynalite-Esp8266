# Philips-Dynalite-Esp8266
Philips Dynalite communication With Esp-8266 MQTT protocol

# Dynalite-ESP8266 RS485 Interface (List Item)

- Esp8266 (Wemos D1 Mini)
- RS485 Interface
- Optocoupler PC817
- Transistor 2n2222a
- R 1K, R 10K
- Led 5mm (indicator)
- Relay Omron my2n 12V
- socket relay my2n
- Datasheet Omron (https://www.omron.co.id/products/family/948/lineup.html)
- Mini Psu Hi-Link 220VAC -> 5VDC
- Mini Psu Hi-Link 220VAC -> 12VDC

# Somfy Controller Use ESP8266 (List Item)

- Esp8266 (Wemos D1 Mini)
- Rf Transmitter 433 Mhz (https://digiwarestore.com/id/transmiter/rf-transmitter-module-433-mhz-432001.html?search_query=rf+module&results=37)
- Mini Psu Hi-Link 220VAC -> 5VDC
- Led 5mm (indicator)

# Home Assistant Setup (Example)

Under covers in HA, use this config Philips Dynalite (Connect To PDEG)

```
dynalite:
  bridges:
    - host: 192.168.10.150
      port: 50000
      autodiscover: true
      polltimer: 1
      area:
        "10":
          name: Master Bedroom
          template: room
          nodefault: true
          room_on: 1
          room_off: 2
          preset:
            "3":
              name: Relax
              fade: 5.0
            "4":
              name: Night Mode
              fade: 5.0
            "5":
              name: Morning
              fade: 5.0
            "6":
              name: Cloud Day Light
              fade: 5.0
        "11":
          name: Curtain Control
          template: room
          nodefault: true
          room_on: 1
          room_off: 2

```

Under covers in HA, use this config MQTT

```
mqtt:
  broker: "Your IP Broker/server"
  port: Your port MQTT
  username: "Your username"
  password: Your password
  
binary_sensor:
  - platform: mqtt
    name: Away Status
    state_topic: "home/away/state"
    payload_on: 1
    payload_off: 0
    device_class: presence
```

example task philips dynalite

```
Task1()
{
	Name="Welcome Mode"
	dynet(0x1c,200, 4, 0,0,0,0xff)
	//Living Room Channel
	ChannelLevel(A=11,C=2,F=3,L=100)
	ChannelLevel(A=11,C=3,F=3,L=100)
	ChannelLevel(A=11,C=4,F=3,L=100)
	//Kitchen & Dining Channel
	ChannelLevel(A=12,C=5,F=3,L=100)
	//Master Bedroom channel
	Channellevel(A=13,C=6,F=3,L=30)
	ChannelLevel(A=13,C=7,F=3,L=30)
	ChannelLevel(A=13,C=8,F=3,L=30)
	LedOnOff(0x87,0x0C,0x00,0x00)			//Turn On [ 1 ] And Turn Off [ 2 3 4 5 6 ]

}

Task2()
{
	Name="Away Mode"
	LedOnOff(0x0F,0x48,0x00,0x00)			//Turn On [ 6 ] And Turn Off [ 1 2 3 4 5 ]
	delay(5)
	dynet(0x1c,200, 5, 0,0,0,0xff)
	//foyer Channel
	ChannelLevel(A=10,C=1,L=20)
	//Living Room Channel
	ChannelLevel(A=11,C=2,F=5,L=0)
	ChannelLevel(A=11,C=3,F=5,L=0)
	ChannelLevel(A=11,C=4,F=5,L=0)
	//Kitchen & Dining Channel
	ChannelLevel(A=12,C=5,F=5,L=0)
	//Master Bedroom channel
	Channellevel(A=13,C=6,F=5,L=0)
	ChannelLevel(A=13,C=7,F=5,L=0)
	ChannelLevel(A=13,C=8,F=5,L=0)
}


```
example get hex code dynet (Arduino Code)

```

void readData() {
  if (rs485_dynet.available()) {
    byte rx = rs485_dynet.read(); // read byte
    data[len] = rx;
    len++;
  }
  if (len > 7) {
    Serial.print("Data From DyNet : ");
    for (int i = 0; i < len; i++) {
      Serial.print(data[i]);
      Serial.print(",");
    }
    Serial.println("");
    len = 0;
  }
}

```


