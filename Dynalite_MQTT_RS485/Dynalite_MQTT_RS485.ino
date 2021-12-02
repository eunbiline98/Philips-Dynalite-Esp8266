#include <WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#define directionControl 19
#define MaxByte 8

//MQTT Setup
const char* ssid = "LKIBIZ";
const char* password = "lkibiz2019";
const char* MQTT_SERVER = "192.168.10.111";
const char* MQTT_TOPIC = "esp/dynalite/1";

const char* MQTT_CLIENT_ID = "ESP32Client";
const char* MQTT_USERNAME = "esp32";
const char* MQTT_PASSWORD = "admin";

int len = 0;
uint8_t data[8];

WiFiClient espClient;
PubSubClient client(espClient);

SoftwareSerial dynalite(4, 15);// rx(RO),tx(DI)

void setup_wifi() {
  // Connect to Wifi Network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void readData() {
  if (dynalite.available()) {
    byte rx = dynalite.read(); // read byte
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

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD )) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  dynalite.begin(9600);

  pinMode(directionControl, OUTPUT);
  digitalWrite(directionControl, LOW); //preparing for receiption

  Serial.println("configuring RS485" );
  Serial.println("Setup Done...!!!");
}

void loop() {
  readData();
  if (data [3] == 254) {// On 100%
    client.publish(MQTT_TOPIC, "1");
  }
  if (data [3] == 242) {// Morning scene
    client.publish(MQTT_TOPIC, "2");
  }
  if (data [3] == 250) {// Movie Scene
    client.publish(MQTT_TOPIC, "3");
  }
  if (data [3] == 253) {// Off
    client.publish(MQTT_TOPIC, "0");
  }
}
