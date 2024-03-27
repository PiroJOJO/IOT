#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
 

const char* ssid = "Sanya";    //  Your Wi-Fi Name
const char* password = "012345678";   // Wi-Fi Password
const char* serverIP = "http://192.168.147.72/"; 

int RED_LED = 4;
int BLUE_RED = 5;
int GREEN_LED = 16;


void setup()
{

  Serial.begin(9600); //Default Baudrate

  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_RED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);


  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_RED, LOW);
  digitalWrite(GREEN_LED, LOW);

  Serial.print("Connecting to the Newtork");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)

  {

    delay(500);

    Serial.print(".");

  }

  Serial.println("WiFi connected"); 
  Serial.print("IP Address of network: "); // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://"); // Will print IP address in URL format

}

 

void loop()
{
  if(WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http; 
      WiFiClient client;

      http.begin(client, serverIP);
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        
        const char* JSONMessage = http.getString().c_str();
        StaticJsonDocument<300> parsed; 
        DeserializationError error = deserializeJson(parsed, JSONMessage);
        if (error) 
        {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }
        else 
        {   
          Serial.println("There are no errors");
          digitalWrite(RED_LED, parsed["red"]);
          digitalWrite(BLUE_RED, parsed["yellow"]);
          digitalWrite(GREEN_LED, parsed["green"]);
        }

      }
      else 
      {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }
}