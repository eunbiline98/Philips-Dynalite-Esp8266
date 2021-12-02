#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <Ticker.h>

const char *MQTT_SERVER = "xxx.xxx.xxx.xxx";
const char *MQTT_TOPIC_RELAY = "home/main/switch/state";
const char *MQTT_TOPIC_HOME_STATUS = "home/main/away/state";
const char *MQTT_STATUS = "home/main/status";
const char *SWITCH1 = "home/main/switch/command";

const char *MQTT_CLIENT_ID = "esp8266-main-switch";
const char *MQTT_USERNAME = "..........";
const char *MQTT_PASSWORD = "..........";

WiFiClient espClient;
PubSubClient client(espClient);
Ticker ticker;

// Pin Setup
#define mainRelay D2

void tick()
{
  // toggle state
  digitalWrite(2, !digitalRead(2)); // set pin GPIO2 Led Wemos On-board
}

void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  // if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  // entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

void setup_wifi()
{
  WiFi.mode(WIFI_STA);
  pinMode(2, OUTPUT);
  ticker.attach(0.6, tick);

  WiFiManager wm;
  wm.setAPCallback(configModeCallback);

  if (!wm.autoConnect())
  { // SSID from IDChip
    Serial.println("failed to connect");
    wm.resetSettings();
    ESP.restart();
    delay(1000);
  }

  Serial.println("connected...");
  ticker.detach();
  digitalWrite(2, LOW);
}

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    Serial.println();
    if ((char)message[i] != '"')
      messageTemp += (char)message[i];
  }

  if (String(topic) == SWITCH1)
  {
    if (messageTemp == "1")
    {
      digitalWrite(mainRelay, HIGH);
      client.publish(MQTT_TOPIC_RELAY, "1");
      client.publish(MQTT_TOPIC_HOME_STATUS, "1");
    }
    else if (messageTemp == "0")
    {
      digitalWrite(mainRelay, LOW);
      client.publish(MQTT_TOPIC_RELAY, "0");
      client.publish(MQTT_TOPIC_HOME_STATUS, "0");
    }
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD, MQTT_STATUS, 1, 1, "Offline"))
    {
      Serial.println("connected");
      // subscribe
      client.subscribe(SWITCH1);
      client.publish(MQTT_STATUS, "Online", true);
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

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  setup_wifi();

  // MQTT Server Setup
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);

  pinMode(2, OUTPUT); // WiFi Indicator
  pinMode(mainRelay, OUTPUT);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}
