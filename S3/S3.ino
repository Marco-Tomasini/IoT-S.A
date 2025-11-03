#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

WiFiClientSecure client;            //cria o cliente wifi
PubSubClient mqtt(client);    //fala que o mqtt usa o cliene wifi

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const int PORT = 8883;
const String URL = "81e7fafe091e4b09b0b93bf45fb52950.s1.eu.hivemq.cloud";

const String broker_user = "s3_Marco";
const String broker_pass = "Loscrias#67";

const String s1 = "Servo1"; //define de onde vou receber as mensagem
const String p1 = "Presenca1"; //define para onde vou enviar as mensagens

const int servoPin1 = 16;
Servo servo1;
const int estado_00 = 120;
const int estado_01 = 60;

void setup() {
  Serial.begin(115200);
  Serial.println("Conectando ao WiFi");
  WiFi.begin(SSID,PASS);

  servo1.attach(servoPin1);

  pinMode(LED,OUTPUT);

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }

  Serial.println("\nConectado!");
  Serial.println("IP:");
  Serial.print(WiFi.localIP());

  client.setInsecure();

  Serial.print("Conectando ao broker...");
  mqtt.setServer(URL.c_str(),PORT);

  while(!mqtt.connected()){
    String ID = "S3-";
    ID += String(random(0xffff),HEX);

    mqtt.connect(ID.c_str(),broker_user.c_str(),broker_pass.c_str());
    delay(200);
    Serial.print(".");
  }
  mqtt.subscribe(s1.c_str());
  mqtt.setCallback(callback);
  Serial.println("\n Conecado ao broker com sucesso!");

}

void loop() {
  if (WiFi.status() != WL_CONNECTED){

    Serial.println("Conectando ao WiFi");
    WiFi.begin(SSID,PASS);

    while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(200);
    }

    Serial.println("\nConectado!");
    Serial.println("IP:");
    Serial.print(WiFi.localIP());
  }

  mqtt.loop();
  delay(500);
}

void callback(char* topic, byte* payload, unsigned int length){
  String mensagem = "";
  for(int i = 0; i < length; i++){
    mensagem += (char)payload[i];
  }
  Serial.print("Recebido: ");
  Serial.println(mensagem);

  if(topic == p1){
    if(mensagem == "Estado_0"){
      servo1.write(estado_00); 
      Serial.println("Estado 00 Ativado");
      mqtt.publish(s1.c_str(),"Estado_0");
    }

    if(mensagem == "Estado_1"){
      meuservo.write(estado_01); 
      Serial.println("Estado 01 Ativado");
      mqtt.publish(s1.c_str(),"Estado_1");
    }
  }
}
