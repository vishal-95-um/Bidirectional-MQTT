#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define led 32

const char* ssid = "WiFi";
const char* password = "Password";

const char* broker = "IP_Address";
const int port = 1883;

const int sensor_pin = 34;  
int sensorValue = 0;
float baseline = 0;
float pulseSignal = 0;
bool beatDetected = false;
unsigned long lastBeatTime = 0;
int BPM = 0;
float threshold = 50;

unsigned long lastSend = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(led, OUTPUT);
  WiFiConnect();
  
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

if (millis() - lastSend > 1000) {
  publishP(bpm);
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
  
  sensorValue = analogRead(sensor_pin);

  baseline = (baseline * 0.95) + (sensorValue * 0.05);

  
  pulseSignal = sensorValue - baseline;


  Serial.print("Signal: ");
  Serial.print(pulseSignal);


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


  if (pulseSignal < threshold) {
    beatDetected = false;
  }

  delay(50);
  return BPM;
}

void connectMQTT() {
  while(!client.connected()) {
    Serial.println("Trying to connect with MQTT..");
  

  if(client.connect("ESP_Client", "esp32/harsh/bpm", 0, true, "Setup is offline" )) {
    Serial.println("ESP + MQTT Connection Established");
    client.subscribe("esp32/gpio32/control");
  } else {
    Serial.print("MQTT Connection failed, rc = ");
    Serial.print(client.state());
    delay(2000);
  }

}
}

void publishP(int bpm) {
  StaticJsonDocument<200> doc;
  doc["person"] = "Harsh";
  doc["location"] = "Jalandhar";
  doc["bpm"] = bpm;
  doc["led_status"] = digitalRead(32);

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