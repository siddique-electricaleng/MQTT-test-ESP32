#include <Arduino.h>
#include <WiFi.h>
#include <MQTT.h>

// put function declarations here:
void setup_wifi();
void setup_mqtt_connect();
void messageReceived(String &topic, String &payload);

// Put constants here:
const char ssid[] = "A5";
const char password[] = "tan519690823";
char pub_msg[50];

WiFiClient net;    // Manages WiFi connection (TCP)
MQTTClient client; // Manages MQTT messaging

unsigned long lastMillis = 0;
int msgNo = 0;
const char mqtt_topic_sub[] = "home/lamp";
const char mqtt_topic_pub[] = "esp32/pub1";
const char *mqtt_client_ID = "Esp32 Client 1";
void setup()
{
  Serial.begin(115200);

  // Connect to WiFi
  setup_wifi();

  // Initialize MQTT client and define stuff for the mqtt broker
  client.begin("192.168.0.147", 1883, net);
  client.onMessage(messageReceived);

  // Connect the mqtt client to the mqtt broker
  setup_mqtt_connect();

  // subscribe to a topic:
  client.subscribe(mqtt_topic_sub);

  Serial.println("Subscribed to topic: " + String(mqtt_topic_sub));
}

void loop()
{
  client.loop();
  msgNo++;
  if (msgNo < 16)
  {
    delay(1000);
    snprintf(pub_msg, sizeof(pub_msg), "From ESP32 Client 1 Message No: %d", msgNo);
    client.publish(mqtt_topic_pub, pub_msg);
  }
}

void setup_wifi()
{
  delay(10);
  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address of ESP32: ");
  Serial.print(WiFi.localIP());
}

void setup_mqtt_connect()
{
  Serial.println("\nConnecting to MQTT broker");
  int retry = 0;
  while (!client.connect(mqtt_client_ID, "mosquittoatHome", "1forg01") && retry <= 10)
  {
    Serial.print(".");
    delay(500);
    retry++;
  }
  if (retry > 10)
  {
    Serial.println("Failed to Connect to MQTT Broker");
  }
  else
  {
    Serial.println("Connected to MQTT broker");
  }
}

// Callback function to handle incoming MQTT messages
void messageReceived(String &topic, String &payload)
{
  Serial.println("\n Message received:");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.println(payload);
  Serial.println("______________________________");
}