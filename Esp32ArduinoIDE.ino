#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

TaskHandle_t Task1;
TaskHandle_t Task2;

// Replace the next variables with your SSID/Password combination
const char* ssid = "PedroEduardo";
const char* password = "01x10ps2021";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "dnsuga.duckdns.org";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

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

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    5,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    5,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
  
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

void Task1code( void * pvParameters ){
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());
    while (1){
    delay(1);
   }
}

void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    delay(1);
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
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
    //temperature = dht.readTemperature();
    // Uncomment the next line to set temperature in Fahrenheit 
    // (and comment the previous temperature line)
    //temperature = 1.8 * bme.readTemperature() + 32; // Temperature in Fahrenheit
    
    // Convert the value to a char array
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    client.publish("temperature", tempString);

    //humidity = dht.readHumidity();
    
    // Convert the value to a char array
    char humString[8];
    dtostrf(humidity, 1, 2, humString);
    Serial.print("Humidity: ");
    Serial.println(humString);
    client.publish("humidity", humString);
  }
}
