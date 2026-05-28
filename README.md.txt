# ESP32 Bidirectional MQTT Heart Rate Monitoring System

A bidirectional IoT-based heart rate monitoring and device control system using ESP32, MQTT, and Pulse Sensor. The ESP32 continuously publishes real-time BPM sensor data while also subscribing to MQTT topics for remote LED control and command handling.

## Features

* Real-time BPM calculation using Pulse Sensor
* Bidirectional MQTT communication
* MQTT publish and subscribe architecture
* JSON-based telemetry transmission
* Remote LED/device control through MQTT commands
* Last Will and Testament (LWT) support
* WiFi-enabled IoT connectivity
* Real-time sensor monitoring and actuator response

## Technologies Used

* ESP32
* Arduino IDE
* MQTT Protocol
* Mosquitto MQTT Broker
* ArduinoJson
* PubSubClient

## System Architecture

### ESP32 → MQTT Broker (Publishing)

The ESP32 publishes heart rate telemetry data to the MQTT broker.

**Topic:**

`esp32/harsh/bpm`

### MQTT Broker → ESP32 (Subscribing)

The ESP32 subscribes to control commands from the MQTT broker for remote LED/device control.

**Topic:**

`esp32/gpio32/control`

## Example Telemetry Payload

```json
{
  "person": "Harsh",
  "location": "Jalandhar",
  "bpm": 78,
  "led_status": 1
}
```

## Bidirectional MQTT Workflow

1. ESP32 reads pulse sensor data.
2. BPM value is calculated in real time.
3. ESP32 publishes BPM data to MQTT broker.
4. MQTT clients can monitor telemetry remotely.
5. MQTT broker sends control commands to ESP32.
6. ESP32 receives subscribed commands and controls onboard LED/device.

## Future Improvements

* Node-RED Dashboard Integration
* Cloud MQTT Broker Support
* Firebase Integration
* Data Logging and Analytics
* Web Dashboard for Monitoring
* Mobile App Integration
* Secure MQTT Authentication (TLS/SSL)
