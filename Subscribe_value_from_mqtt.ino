#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = "kuyrai"; // ชื่อ เน็ต
const char* password = "1234567899"; // รหัส เน็ต
const char* mqtt_server = "broker.mqttdashboard.com"; // โบกเกอร์ อาจาร 

WiFiClient espClient;
PubSubClient client(espClient);

char clientID[15];
char destinationTopic[]   = "Pub_fill"; // ชื่อ Topic ที่จะ Subscribe

void callback(char* topic, byte* payload, unsigned int length) {
  String payloadStr = String();
  String topicStr = String(topic);
  int payloadInt = 0;
  
  // Convert byte to String
  for (int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  // Convert String to Int
  payloadInt = payloadStr.toInt();


  // payloadStr คือตัวแปรที่อ่านค่าจาก Subscribe มีสถานะเป็น String
  // payloadStr คือตัวแปรที่อ่านค่าจาก Subscribe มีสถานะเป็น Int

/* เลือกเอาว่าอาจารย์จะให้ทำอะไร 
  [ 1 ] ถ้าอาจารย์ให้รับค่ามาหรี่หลอดไฟ 
  [ 2 ] ถ้าอ่านเงือนไขตัวอักษรใช้ payloadStr
  [ 3 ] ถ้าอ่านเงือนไขตัวเลขก็ใช้ payloadInt
*/
   //[ 1 ] analogWrite ไว้ทำ แสดงไฟแบบ Fade 
   analogWrite(D0, payloadInt);
   analogWrite(D1, payloadInt);
   analogWrite(D2, payloadInt);
   
  
//  // [ 2 ] เงื่อนไขสำหรับ ค่า String
//  Serial.println(payloadStr);
//  if (payloadStr == "Blue"){
//    digitalWrite(D0, HIGH);
//    digitalWrite(D1, LOW);
//    digitalWrite(D2, LOW);
//  }
//  else if (payloadStr == "Red"){
//    digitalWrite(D0, LOW);
//    digitalWrite(D1, LOW);
//    digitalWrite(D2, HIGH);
//  }
//  else if (payloadStr == "Green"){
//    digitalWrite(D0, LOW);
//    digitalWrite(D1, HIGH);
//    digitalWrite(D2, LOW);
//  }
//  else{
//    digitalWrite(D0, LOW);
//    digitalWrite(D1, LOW);
//    digitalWrite(D2, LOW);
//  }

//     //[ 3 ] เงื่อนไขสำหรับ ค่า Int
//  if (payloadInt <= 300){
//    digitalWrite(D0, HIGH);
//    digitalWrite(D1, LOW);
//    digitalWrite(D2, LOW);
//  }
//  else if (payloadInt <= 500){
//    digitalWrite(D0, LOW);
//    digitalWrite(D1, HIGH);
//    digitalWrite(D2, LOW);
//  }
//  else if (payloadInt <= 700){
//    digitalWrite(D0, LOW);
//    digitalWrite(D1, LOW);
//    digitalWrite(D2, HIGH);
//  }
//  else{
//    digitalWrite(D0, LOW);
//    digitalWrite(D1, LOW);
//    digitalWrite(D2, LOW);
//  }
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
      // Subscribing...
      client.subscribe(destinationTopic);
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
  // pinMode ต้องประกาศให้ไฟ ออกรูไหนบ้าง ในที่นี้ออก 3 ตัว
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
