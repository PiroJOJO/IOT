#include <ESP8266WiFi.h>

 

const char* ssid = "name";    //  Your Wi-Fi Name

const char* password = "password!";   // Wi-Fi Password

 

int LED = 2;   // led connected to GPIO2 (D4)

WiFiServer server(80);

 

void setup()

{

  Serial.begin(9600); //Default Baudrate

  pinMode(LED, OUTPUT);

  digitalWrite(LED, LOW);

 

  Serial.print("Connecting to the Newtork");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)

  {

    delay(500);

    Serial.print(".");

  }

  Serial.println("WiFi connected"); 

  server.begin();  // Starts the Server

  Serial.println("Server started");

 

  Serial.print("IP Address of network: "); // will IP address on Serial Monitor

  Serial.println(WiFi.localIP());

  Serial.println("Copy and paste the following URL: https://"); // Will print IP address in URL format

  Serial.print(WiFi.localIP());

  Serial.println("/");

}

 

void loop()

{

  WiFiClient client = server.available();

  if (!client)
  {
    Serial.println("Dont have a client");
    return;

  }

  Serial.println("Waiting for new client");

  while(!client.available())
  {

    delay(1);

  }

 

  String request = client.readStringUntil('\r');

  Serial.println(request);

  client.flush();

 

  int value = LOW;

  if(request.indexOf("/LED=ON") != -1)

  {

    digitalWrite(LED, LOW); // Turn LED ON

    value = LOW;
    Serial.println("ON");
  }

  if(request.indexOf("/LED=OFF") != -1)

  {

    digitalWrite(LED, HIGH); // Turn LED OFF

    value = HIGH;
    Serial.println("OFF");


  }
}