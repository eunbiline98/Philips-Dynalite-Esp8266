#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

const char *ssid = "LKIBIZ";
const char *password = "lkibiz2019";
const char *MQTT_SERVER = "192.168.10.172";
const char *MQTT_TOPIC = "home/away/state";
const char *MQTT_CURTAIN_TOPIC = "home/curtain/livingroom";

const char *MQTT_CLIENT_ID = "esp8266-dynet";
const char *MQTT_USERNAME = "laviz";
const char *MQTT_PASSWORD = "laviz810";

#define DirectionControl D6
#define indicator D4
#define relay D5

SoftwareSerial RS485(D1, D3);
WiFiClient espClient;
PubSubClient client(espClient);

byte data[8];
int len = 0;

void setup_wifi()
{
  pinMode(D4, OUTPUT); //WiFi Indicator
  Serial.print("Connecting To: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(D4, HIGH);
    delay(50);
    digitalWrite(D4, LOW);
    delay(50);
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setupSerial()
{
  RS485.begin(9600);
  Serial.println("configuring RS485" );
  Serial.println("set up done!!");

  pinMode(indicator, OUTPUT);
  pinMode(DirectionControl, OUTPUT);
  pinMode(relay, OUTPUT);

  digitalWrite(DirectionControl, LOW); //preparing for receiption
  len = 0;
}

void readData()
{
  while (RS485.available())
  {
    byte RX = RS485.read();   // Read the byte
    data[len] = RX;
    len++;
  }

  if (len > 7)
  {
    for (int i = 0; i < len; i++)
    {
      Serial.print(data[i]);
      Serial.print(",");
    }
    Serial.println("");
    len = 0;
  }
  if (data[2] == 5)
  {
    client.publish(MQTT_TOPIC, "0");
    digitalWrite(relay, LOW);
  }
  if (data[2] == 4)
  {
    client.publish(MQTT_TOPIC, "1");
    digitalWrite(relay, HIGH);
  }
  yield();
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  client.setServer(MQTT_SERVER, 1883);
  setupSerial();
}

void loop()
{
  // put your main code here, to run repeatedly:
  readData();
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
