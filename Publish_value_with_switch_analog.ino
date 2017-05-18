#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <stdlib.h>

// Update these with values suitable for your network.
const int AnalogIn = A0;
const char* ssid = "kuyrai"; // ชื่อ เน็ต
const char* password = "1234567899"; // รหัส เน็ต
const char* mqtt_server = "broker.mqttdashboard.com"; // โบกเกอร์ อาจาร 

WiFiClient espClient;
PubSubClient client(espClient);

long count = 0;
long lastMsg = 0;
char destinationTopic[]   = "Pub_fill";

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

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    String clientId = "59070035"; 
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(destinationTopic, "hello, world");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

/* ส่งค่าขึ้นระบบ สำหรับใช้ สวิชหมุน สามารถใช้คู่กับฟังชั่น AnalogWrite */ 
  int val = 0;
  char value[3];
  val = analogRead(AnalogIn);
  itoa(val, value, 10);
  Serial.println(val);
  client.publish(destinationTopic, value);
  delay(0);

/* ส่งค่าแบบลูปโค้ดเดิมของอาจารย์ */
//  long now = millis();
//  if (now - lastMsg > 2000) {
//    lastMsg = now;
//
//    ++count;
//    String publishStr = "I'm Arduino. - ";
//    publishStr.concat(count); // Concatenate string
//   
//    Serial.print("Publishing... : ");
//    Serial.println(publishStr.c_str());
//    client.publish(destinationTopic, publishStr.c_str());
//  }
}
