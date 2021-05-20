// esp8266.cpp - For use during the IoT couse at TTÜ.
// Intened as quick solution for the short practial demonstartion.
// This is the sensor part of the project. It runs on an esp8266 with a button.
// Author: Erick Hunter


#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// global variables
const char* ssid     = "Whatever the SSID is"; // Enter SSID
const char* password = "Whatever the password is"; //Enter password
const int SWITCH_PIN = 0;           // Pin to switch
const char *TOPIC = "test_topic";  // Topic to subcribe to
const char *ID = "Example_Switch_1";  // Name of device, must be unique
const char *PAYLOAD_OK = "SENSOR_1_OK";
const char *PAYLOAD_FALUT = "SENSOR_1_TRIP";
const int TIME_TO_SLEEP = 1000; // in 1/1000's of a second
const bool RANDOM_STATE = true; // Uncomment to enable simulated button state

IPAddress broker(192,168,1,222); // address of MQTT broker.
WiFiClient wclient;

PubSubClient client(wclient); // Setup MQTT client.
// end of config


void setup_wifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA); // So ESP won't run as AP.
  WiFi.begin(ssid, password);

  Serial.println("");
  Serial.println("connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until reconnected
    while (!client.connected())
    {
      Serial.print("Attempting MQTT connection...");
      if (client.connect(ID)) // Attempt to connect.
      {
        Serial.println("connected");
        Serial.print("Publishing to: ");
        Serial.println(TOPIC);
        Serial.println('\n');
      }
      else
      {
        Serial.println(" try again in 5 seconds");
        delay(5000); // Wait 5 seconds before retrying
      }
    }
}


void setup() {
  Serial.begin(9600);
  pinMode(SWITCH_PIN, INPUT);  // Configure SWITCH_PIN as an input.
  delay(100);
  setup_wifi(); // Connect to wifi.
  client.setServer(broker, 1883); // Connects to broker (IP, PORT).
}


void loop() {

  int switch_state;

  if(RANDOM_STATE == true) // Selects between real and simulated switch state.
  {
    switch_state = random(0,5); // Simulated switch state.
  }
  else
  {
    switch_state = digitalRead(SWITCH_PIN); // Actual switch state.
  }

  if (!client.connected())  // Reconnect if connection is lost.
  {
    reconnect();
  }
  client.loop();

  if(switch_state == 0) // If the switch is being pressed.
  {
    client.publish(TOPIC, PAYLOAD_OK);
    Serial.println((String)TOPIC + " => on");
  }
  else // If the swith is at rest.
  {
    client.publish(TOPIC, PAYLOAD_FALUT);
    Serial.println((String)TOPIC + " => off");
  }

  delay(TIME_TO_SLEEP); // used insted of deep sleep for demonstartion.
}
