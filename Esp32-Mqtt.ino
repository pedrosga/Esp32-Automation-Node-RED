#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "DHT.h"

//Credenciais do Wi-Fi
const char* ssid = "SSID";
const char* password = "SENHA";

// Endereço MQTT Broker
const char* mqtt_server = "examplo.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float temperature = 0;
float humidity = 0;

// Pino digital do DHT
#define DHTPIN 22  

// Tipo de DHT utilizado
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)   

// Inicializa o DHT
DHT dht(DHTPIN, DHTTYPE);

// LED Pin
const int ledPin1 = 2;
const int ledPin2 = 4;
const int ledPin3 = 16;
const int ledPin4 = 17;

void setup() {
  Serial.begin(115200);
  dht.begin();
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
  // Altera o estado de saída de acordo com a mensagem
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

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe Broker
      client.subscribe("output1");
      client.subscribe("output2");
      client.subscribe("output3");
      client.subscribe("output4");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
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

    //Temperatura
    temperature = dht.readTemperature();
    
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    client.publish("temperature", tempString);

    //Umidade
    humidity = dht.readHumidity();

    char humString[8];
    dtostrf(humidity, 1, 2, humString);
    Serial.print("Humidity: ");
    Serial.println(humString);
    client.publish("humidity", humString);
  }
}
