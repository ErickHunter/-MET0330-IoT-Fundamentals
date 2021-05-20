// nodeMCU.cpp - For use during the IoT couse at TTÜ.
// Intened as quick solution for the short practial demonstartion.
// This is the LED part of the project. It runs on an nodeMCU with a induvddualy
// addresable LED strip.
// Author: Erick Hunter


#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <cstring>
#include <iostream>


// global variables
const char* ssid     = "Whatever the SSID is"; // Enter SSID.
const char* password = "Whatever the password is"; //Enter password.
const char *TOPIC = "test_topic";  // Topic to subcribe to.
const char *ID = "Example_Light_1";  // Name of device, must be unique.

IPAddress broker(192,168,1,222); // Address of MQTT broker.
WiFiClient wclient;
PubSubClient client(wclient); // Setup MQTT client.

#define PIN D8
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);
const int LED_PIN = 0;           // Pin to LED.
const int LED_COUNT = 3; // Define number of led pixels in strip.

const int TIME_TO_SLEEP = 0; // In 1/1000's of a second.

// end of config


void setup_wifi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA); //So ESP won't run as AP.
  WiFi.begin(ssid, password);

  Serial.println("");
  Serial.println("connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
    while (!client.connected()) // Loop until reconnected
    {
      Serial.print("Attempting MQTT connection...");
      if (client.connect(ID)) // Attempt to connect.
      {
        Serial.println("connected");
        Serial.print("Publishing to: ");
        Serial.println(TOPIC);
        Serial.println('\n');
        client.subscribe(TOPIC); // Subscribe
      }
      else
      {
        Serial.println(" try again in 5 seconds");
        delay(5000); // Wait 5 seconds before retrying
      }
    }

}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String message;

  for (int i = 0; i < length; i++) {
      message = message + (char) payload[i];  // convert *byte to string
  }
  Serial.print(message);
  if (message == "SENSOR_1_OK") { pixels.setPixelColor(0,255,0,0); } // Red
  if (message == "SENSOR_1_TRIP") { pixels.setPixelColor(0,0,0,255); } // Blue

  if (message == "SENSOR_2_OK") { pixels.setPixelColor(1,255,0,0); } // Red
  if (message == "SENSOR_2_TRIP") { pixels.setPixelColor(1,0,0,255); } // Blue

  if (message == "SENSOR_3_OK") { pixels.setPixelColor(2,255,0,0); } // Red
  if (message == "SENSOR_3_TRIP") { pixels.setPixelColor(2,0,0,255); } // Blue
  pixels.show();
}


void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);  // Configure LED_PIN as an OUTPUT.
  pixels.begin(); // Initalize NeoPixel
  setup_wifi(); // Connect to wifi.
  client.setServer(broker, 1883); // Connects to broker (IP, PORT).
  client.subscribe(TOPIC); // Subscibe
  client.setCallback(callback); // Set callback
}


void loop() {
  if (!client.connected())  // Reconnect if connection is lost.
  {
    reconnect();
  }
  client.loop();

  delay(TIME_TO_SLEEP); // used insted of deep sleep for demonstartion.
}
