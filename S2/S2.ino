#include <WiFiClientSecure.h>
#include <PubSubClient.h>

WiFiClient client;
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const int PORT           = 8883;
const String URL         = "81e7fafe091e4b09b0b93bf45fb52950.s1.eu.hivemq.cloud";

const String PresencaUm = "presenca1";
const String PresencaDois = "presenca2";
const String Ilum = "ilum";

const String broker_user = "s2_Enzo";
const String broker_pass = "Loscrias#67";

const int LED = 2;

void setup() {
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
  client.setInsecure();

  Serial.println("\nConectando ao broker");
  mqtt.setServer(URL.c_str(), PORT);

  while(!mqtt.connected()){
    String ID = "S2-";
    ID += String(random(0xffff), HEX);

    mqtt.connect(ID.c_str(), broker_user.c_str(), broker_pass.c_str());
    delay(200);
    Serial.print(".");
  }

  mqtt.subscribe(Ilum.c_str());
  mqtt.setCallback(callback);
  Serial.println("\nConectado ao broker com sucesso!");

  pinMode(LED, OUTPUT);
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

    String mensagem ="Nó 2: ";

    

    mensagem += Serial.readStringUntil('\n');
    mqtt.publish(PresencaUm.c_str(),mensagem.c_str());
    mqtt.publish(PresencaDois.c_str(),mensagem.c_str());
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

  if(Ilum = "Nó 1: Iluminação Baixa!"){
    digitalWrite(LED, HIGH);
  }else if(Ilum = "Nó 1: Iluminação Adequada!"){
    digitalWrite(LED, LOW);
  }else{
    Serial.println(mensagem);
  }
}