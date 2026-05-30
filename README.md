Overview

This project implements a bidirectional IoT-based heart rate monitoring and device control system using ESP32, MQTT, and sensors. The system publishes real-time physiological data and subscribes to remote control commands. This branch implements secure communication using MQTT over TLS.

The ESP32 publishes sensor data (BPM and temperature) while also receiving control commands to manage an onboard LED.

Features
Real-time BPM calculation using Pulse Sensor
Temperature monitoring using DHT11 sensor
Bidirectional MQTT communication (publish and subscribe)
JSON-based telemetry format
Remote LED control via MQTT commands
MQTT over TLS secure communication
WiFi connectivity for IoT communication
Technologies Used
ESP32
Arduino IDE
MQTT protocol
WiFiClientSecure (TLS support)
PubSubClient library
ArduinoJson library
DHT11 sensor
Pulse Sensor
System Architecture
ESP32 → MQTT Broker (Publishing)

The ESP32 publishes sensor data to the MQTT broker.

Topic:

esp32/harsh/bpm
MQTT Broker → ESP32 (Subscribing)

The ESP32 subscribes to control commands from the broker.

Topic:

esp32/gpio32/control
Security Implementation (MQTT over TLS)

This branch uses MQTT over TLS for secure communication.

Port: 8883
Client: WiFiClientSecure
Secure connection enabled using TLS

Note: setInsecure() is used for testing purposes. In production, a CA certificate should be configured using setCACert().

Example Telemetry Payload
{
  "patient": {
    "name": "Harsh",
    "id": 69
  },
  "deviceID": "telekit_Jalandhar",
  "sensors": {
    "bpm": 78,
    "temperature": 36.5
  },
  "actuators": {
    "ledStatus": 1
  }
}
Bidirectional Workflow
ESP32 reads pulse sensor data
BPM is calculated in real time
Temperature is read from DHT11 sensor
Data is published to MQTT broker over TLS
External clients subscribe to telemetry topic
MQTT broker sends control commands
ESP32 receives commands and controls LED
MQTT Commands

Topic:

esp32/gpio32/control

Commands:

ON → Turns LED ON
OFF → Turns LED OFF
Key Implementation Details
Secure Client
WiFiClientSecure espClient;
MQTT Broker Port
8883
TLS Configuration
espClient.setInsecure();
Future Improvements
Replace insecure TLS mode with certificate-based authentication
Node-RED dashboard integration
Cloud database logging (Firebase / InfluxDB)
Web-based monitoring dashboard
Mobile application support
Role-based MQTT access control (ACL)
Summary

This branch upgrades the system from standard MQTT communication to secure MQTT over TLS, enabling encrypted data transmission between ESP32 and MQTT broker.