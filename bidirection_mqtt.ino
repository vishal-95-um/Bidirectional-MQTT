#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT11.h>

#define led 32

const char* ssid = "wifi_username";
const char* password = "wifi_password";

const char* broker = "broker url";
const int port = 8883;

const int sensor_pin = 34;  
int sensorValue = 0;
float baseline = 0;
float pulseSignal = 0;
bool beatDetected = false;
unsigned long lastBeatTime = 0;
int BPM = 0;
float threshold = 50;

unsigned long lastSend = 0;

WiFiClientSecure espClient;
PubSubClient client(espClient);

DHT11 dht11(4);

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(led, OUTPUT);
  WiFiConnect();

  espClient.setInsecure();
  
  client.setServer(broker, port);
  client.setCallback(callback);
  connectMQTT();
  
}

void loop() {

  if(!client.connected()) {
    connectMQTT();
  }

  client.loop();

  int bpm = getBPM();
  float temp = getTemp();

if (millis() - lastSend > 1000) {
  publishP(bpm, temp);
  lastSend = millis();
}
  
}


void WiFiConnect() {

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Wifi Connected");
  Serial.print("The IP Address: ");
  Serial.println(WiFi.localIP());

}

int getBPM() {
  // 1. Read raw sensor
  sensorValue = analogRead(sensor_pin);

  // 2. Build baseline (DC removal filter)
  baseline = (baseline * 0.95) + (sensorValue * 0.05);

  // 3. Extract pulse signal (AC component)
  pulseSignal = sensorValue - baseline;

  // 4. Debug output
  Serial.print("Signal: ");
  Serial.print(pulseSignal);

  // 5. Beat detection
  if (pulseSignal > threshold && beatDetected == false) {

    beatDetected = true;

    unsigned long currentTime = millis();

    if (lastBeatTime > 0) {
      int interval = currentTime - lastBeatTime;
      BPM = 60000 / interval;
    }

    lastBeatTime = currentTime;

    Serial.print("  >>> BEAT!");
  }

  // 6. Reset beat flag
  if (pulseSignal < threshold) {
    beatDetected = false;
  }

  delay(50);
  return BPM;
}

void connectMQTT() {
  if(!client.connected()) {
    Serial.println("Trying to connect with MQTT..");

    if(client.connect("ESP_Client", "username", "password")) {
    Serial.println("ESP + MQTT Connection Established");
    client.subscribe("esp32/gpio32/control");
  } else {
    Serial.print("MQTT Connection failed, rc = ");
    Serial.print(client.state());
    delay(2000);
  }

}
}

void publishP(int bpm, float temp) {
  StaticJsonDocument<200> doc;

  doc["patient"]["name"] = "Harsh";
  doc["patient"]["id"] = 69;

  doc["deviceID"] = "telekit_Jalandhar";

  doc["sensors"]["bpm"] = bpm;
  doc["sensors"]["temperature"] = temp;

  doc["actuators"]["ledStatus"] = digitalRead(32);

  char buffer[200];

  serializeJson(doc, buffer);

  client.publish("esp32/harsh/bpm", buffer);
  Serial.print("Payload Sent: ");
  Serial.println(buffer);
}

void callback(char* topic, byte* message, unsigned int length) {

  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  String data = "";

  for (int i = 0; i < length; i++) {
    data += (char)message[i];
  }

  Serial.print("Message: ");
  Serial.println(data);

  if(data == "ON") {
     digitalWrite(led, HIGH);
  } 

  if(data == "OFF") {
    digitalWrite(led, LOW);
  }
}

float getTemp() {
  float temp = dht11.readTemperature();
    if (temp != DHT11::ERROR_CHECKSUM && temp != DHT11::ERROR_TIMEOUT) {
     return temp;
    } else {
        return -555;
    }
}