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

Under covers in HA, use this config Philips dynalite (Connect To PDEG)

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
