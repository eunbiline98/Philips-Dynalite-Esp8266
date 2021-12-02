
std::vector<REMOTE> const remotes = {{0x184623, "home/curtain/livingroom", 1, 0}};
const bool reset_rolling_codes = false;

const char *mqtt_server = "xxx.xxx.xxx.xxx";
const unsigned int mqtt_port = 1883;
const char *mqtt_user = ".........";
const char *mqtt_password = ".........";
const char *mqtt_id = "esp8266-somfy-remote";

const char *status_topic = "home/curtain/livingroom/status"; // Online / offline
const char *state_topic = "home/curtain/livingroom/state";
const char *ack_topic = "home/curtain/livingroom/ack"; // Commands ack "id: 0x184623, cmd: u"

#define PORT_TX 5 // Output data on pin 23 (can range from 0 to 31). Check pin numbering on ESP8266.
