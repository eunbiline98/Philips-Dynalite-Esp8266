#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

const char *ssid = "..........";
const char *password = "........";
const char *MQTT_SERVER = ".........";
const char *MQTT_TOPIC = "tele/away/state";

const char *MQTT_CLIENT_ID = "esp_02";
const char *MQTT_USERNAME = "";
const char *MQTT_PASSWORD = "";

#define DirectionControl D6
#define relay D2

SoftwareSerial dynet(D1, D3);
WiFiClient espClient;
PubSubClient client(espClient);

byte data[8];
int len = 0;

void setup_wifi()
{
  pinMode(D4, OUTPUT); // WiFi Indicator
  Serial.print("Connecting To: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
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
  dynet.begin(9600);
  Serial.println("config dynet");
  Serial.println("set up done!!");

  pinMode(DirectionControl, OUTPUT);

  digitalWrite(DirectionControl, LOW); // preparing for receiption
  len = 0;
}

void readData()
{
  while (dynet.available())
  {
    byte RX = dynet.read();
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
  if (data[2] == 0)
  {
    digitalWrite(relay, LOW);
    Client.publish(MQTT_TOPIC, "0");
  }
  if (data[1] == 1)
  {
    digitalWrite(relay, HIGH);
    Client.publish(MQTT_TOPIC, "1");
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  client.setServer(MQTT_SERVER, 1883);
  pinMode(relay, OUTPUT);
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
