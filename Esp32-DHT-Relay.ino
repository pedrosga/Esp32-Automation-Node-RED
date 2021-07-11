#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "DHT.h"
//#include <Adafruit_BME280.h>
//#include <Adafruit_Sensor.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "******";
const char* password = "*****";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "example.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//uncomment the following lines if you're using SPI
/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

//Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
float temperature = 0;
float humidity = 0;

// Digital pin connected to the DHT sensor
#define DHTPIN 22  

// Uncomment whatever DHT sensor type you're using
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)   

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// LED Pin
const int ledPin1 = 2;
const int ledPin2 = 4;
const int ledPin3 = 16;
const int ledPin4 = 17;

void setup() {
  Serial.begin(115200);
  dht.begin();
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  //status = bme.begin();  
  /*if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }*/
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "output1") {
    Serial.print("Changing output to ");
    if(messageTemp == "11"){
      Serial.println("on");
      digitalWrite(ledPin1, HIGH);
    }
    else if(messageTemp == "10"){
      Serial.println("off");
      digitalWrite(ledPin1, LOW);
    }
  }
//---------------------------------------
  if (String(topic) == "output2") {
    Serial.print("Changing output to ");
    if(messageTemp == "21"){
      Serial.println("on");
      digitalWrite(ledPin2, HIGH);
    }
    else if(messageTemp == "20"){
      Serial.println("off");
      digitalWrite(ledPin2, LOW);
    }
  }
//---------------------------------------
  if (String(topic) == "output3") {
    Serial.print("Changing output to ");
    if(messageTemp == "31"){
      Serial.println("on");
      digitalWrite(ledPin3, HIGH);
    }
    else if(messageTemp == "30"){
      Serial.println("off");
      digitalWrite(ledPin3, LOW);
    }
  }
//---------------------------------------
  if (String(topic) == "output4") {
    Serial.print("Changing output to ");
    if(messageTemp == "41"){
      Serial.println("on");
      digitalWrite(ledPin4, HIGH);
    }
    else if(messageTemp == "40"){
      Serial.println("off");
      digitalWrite(ledPin4, LOW);
    }
  }
//---------------------------------------
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("output1");
      client.subscribe("output2");
      client.subscribe("output3");
      client.subscribe("output4");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // Temperature in Celsius
    //temperature = bme.readTemperature();
    temperature = dht.readTemperature();
    // Uncomment the next line to set temperature in Fahrenheit 
    // (and comment the previous temperature line)
    //temperature = 1.8 * bme.readTemperature() + 32; // Temperature in Fahrenheit
    
    // Convert the value to a char array
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    client.publish("temperature", tempString);

    //humidity = bme.readHumidity();
    humidity = dht.readHumidity();
    
    // Convert the value to a char array
    char humString[8];
    dtostrf(humidity, 1, 2, humString);
    Serial.print("Humidity: ");
    Serial.println(humString);
    client.publish("humidity", humString);
  }
}
