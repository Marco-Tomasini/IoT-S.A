#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClientSecure client;            //cria o cliente wifi
PubSubClient mqtt(client);    //fala que o mqtt usa o cliene wifi

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const int PORT = 8883;
const String URL = "81e7fafe091e4b09b0b93bf45fb52950.s1.eu.hivemq.cloud";

const String broker_user = "trem_Carlos";
const String broker_pass = "Loscrias#67";

const String MyTopic = "trem_Carlos"; //define de onde vou receber as mensagem

const int ledVerde = 18;
const int ledVermelho = 21;

void setup() {
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  client.setInsecure();

  Serial.begin(115200);
  Serial.println("Conectando ao WiFi");
  WiFi.begin(SSID,PASS);

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }

  Serial.println("\nConectado!");
  Serial.println("IP:");
  Serial.print(WiFi.localIP());

  Serial.print("Conectando ao broker...");
  mqtt.setServer(URL.c_str(),PORT);

  while(!mqtt.connected()){
    String ID = "S3-";
    ID += String(random(0xffff),HEX);

    mqtt.connect(ID.c_str(),broker_user.c_str(),broker_pass.c_str());
    delay(200);
    Serial.print(".");
  }
  mqtt.subscribe(MyTopic.c_str());
  mqtt.setCallback(callback);
  Serial.println("\n Conecado ao broker com sucesso!");

}

void loop() {
  mqtt.loop();
}

void callback(char* topic, byte* payload, unsigned int length){
  String mensagem = "";
  for(int i = 0; i < length; i++){
    mensagem += (char)payload[i];
  }
  Serial.print("Recebido: ");
  Serial.println(mensagem);

  int msg = mensagem.toInt();

  if (msg > 0){
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
  } else if(msg < 0){
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);
  } else{
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, LOW);
  }

}