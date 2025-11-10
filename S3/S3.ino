#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

WiFiClientSecure client;    //cria o cliente wifi
PubSubClient mqtt(client);  //fala que o mqtt usa o cliene wifi

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const int PORT = 8883;
const String URL = "81e7fafe091e4b09b0b93bf45fb52950.s1.eu.hivemq.cloud";

const String broker_user = "s3_Marco";
const String broker_pass = "Loscrias#67";

const String s1 = "Servo1";     //define de onde vou receber as mensagem
const String s2 = "Servo2";     //define de onde vou receber as mensagem

const String p1 = "Presenca1";  //define para onde vou enviar as mensagens
const String p2 = "Presenca2";  //define para onde vou enviar as mensagens
const String p3 = "Presenca3";  //define para onde vou enviar as mensagens

const int servoPin1 = 16;
const int servoPin2 = 17;

const int presenca3_ECHO = 24;
const int presenca3_TRIGG = 5;

Servo servo1;
Servo servo2;

const int estado_00 = 120;
const int estado_01 = 60;

void setup() {
  Serial.begin(115200);
  Serial.println("Conectando ao WiFi");
  WiFi.begin(SSID, PASS);

  servo1.attach(servoPin1);
  servo2.attach(servoPin1);

  pinMode(LED, OUTPUT);
  pinMode(presenca3_TRIGG, OUTPUT);
  pinMode(presenca3_ECHO, INPUT);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.println("\nConectado!");
  Serial.println("IP:");
  Serial.print(WiFi.localIP());

  client.setInsecure();

  Serial.print("Conectando ao broker...");
  mqtt.setServer(URL.c_str(), PORT);

  while (!mqtt.connected()) {
    String ID = "S3-";
    ID += String(random(0xffff), HEX);

    mqtt.connect(ID.c_str(), broker_user.c_str(), broker_pass.c_str());
    delay(200);
    Serial.print(".");
  }
  mqtt.subscribe(p1.c_str());
  mqtt.subscribe(p2.c_str());
  mqtt.subscribe(p3.c_str());

  mqtt.setCallback(callback);
  Serial.println("\n Conecado ao broker com sucesso!");
}


void loop() {
  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("Conectando ao WiFi");
    WiFi.begin(SSID, PASS);

    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(200);
    }

    Serial.println("\nConectado!");
    Serial.println("IP:");
    Serial.print(WiFi.localIP());
  }
  long distancia = lerDistancia();

  if (distancia >= 10){
    mqtt.publish(Presenca3.c_str(),"Estado_01"); //envia a mensagem para o tópico
  }

  mqtt.loop();
  delay(500);
}


long lerDistancia() {
  digitalWrite(presenca3_TRIGG, LOW);
  delayMicroseconds(2);
  digitalWrite(presenca3_TRIGG, HIGH);
  delayMicroseconds(10);
  digitalWrite(presenca3_TRIGG, LOW);
  
  long duracao = pulseIn(presenca3_ECHO, HIGH);
  long distancia = duracao * 349.24 / 2 / 10000;
  
  return distancia;
}


void callback(char* topic, byte* payload, unsigned int length) {
  String mensagem = "";
  for (int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }
  Serial.print("Recebido: ");
  Serial.println(mensagem);

  if (String(topic) == p1) {
    if (mensagem == String("Estado_00")) {
      servo1.write(estado_00);
      Serial.println("Estado 00 Ativado");
      mqtt.publish(s1.c_str(), "Estado_00");
    }
  }
  
  if (String(topic) == p2) {
    if (mensagem == String("Estado_01")) {
      servo2.write(estado_01);
      Serial.println("Estado 01 Ativado");
      mqtt.publish(s2.c_str(), "Estado_01");
    }
  }

  if (String(topic) == p3) {
      mensagem == String("Estado_01") {
      servo1.write(estado_01);
      servo2.write(estado_01);
      Serial.println("Estados Alterados");
      mqtt.publis../cdh(s1.c_str(), "Estado_01");
      mqtt.publish(s2.c_str(), "Estado_01");
    }
  }
}