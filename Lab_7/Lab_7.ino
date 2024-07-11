#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

//WiFi
const char *ssid = "name";
const char *password = "password";
WiFiClient espClient;
PubSubClient client(espClient);

//MQTT
const char *mqtt_brocker = "mqtt.by";
const char *topic_T = "/user/PiroJOJO/dht/temp";
const char *topic_H = "/user/PiroJOJO/dht/hum";
const char *mqtt_username = "PiroJOJO";
const char *mqtt_password = "e4pjooso";
const int mqtt_port = 1883;

//DHT
#define DHTPIN 2
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  //WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serail.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network!");

  //MQTT
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  while(client.connected())
  {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.println("The client %s connects to the public mqtt broker \n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
    {
      Serial.println("Public mqtt broker connected");
    }
    else
    {
      Serial.println("failed with state ");
      Serila.print(client.state());
      delay(2000);
    }
  }

  client.subscribe(topic_T);
  client.subscribe(topic_H);

  //DHT
  dht.begin();
}

void callback(char *topic, byte *payload, unsigned_int length){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for(int i = 0; i < length; i++)
  {
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("------------------");
}

void loop() {
  client.loop();

  // Wait a few seconds between measurements.
  delay(2000);

  // float h = dht.readHumidity();
  float h =random(5, 20);
  // float t = dht.readTemperature();
  float t = random(20, 30);

  if (isnan(h) || isnan(t)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  client.publish(topic_T, (char*)String(t).c_str());
  client.publish(topic_H, (char*)String(h).c_str());

  // Serial.print(F("Humidity: "));
  // Serial.print(h);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(t);

}
