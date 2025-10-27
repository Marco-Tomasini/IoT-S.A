#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client;
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const int PORT           = 1883;
const String URL         = "test.mosquitto.org";

const String MyTopic = "TopicoChat";
const String OtherTopic = "TopicoChat";

const String broker_user = "";
const String broker_pass = "";

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

  Serial.println("\nConectando ao broker");
  mqtt.setServer(URL.c_str(), PORT);

  while(!mqtt.connected()){
    String ID = "S2-";
    ID += String(random(0xffff), HEX);

    mqtt.connect(ID.c_str(), broker_user.c_str(), broker_pass.c_str());
    delay(200);
    Serial.print(".");
  }

  mqtt.subscribe(MyTopic.c_str());
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

    String mensagem ="Enzo: ";

    if(Serial.available()>0){
      mensagem += Serial.readStringUntil('\n');
      mqtt.publish(OtherTopic.c_str(),mensagem.c_str());
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

  if(mensagem == "Marco: AcendeR" || mensagem == "Carlos: AcendeR" || mensagem == "Brayan: AcendeR" || mensagem == "Enzo: AcendeR"){
    digitalWrite(LED, HIGH);
  }else if(mensagem == "Marco: ApagaR" || mensagem == "Carlos: ApagaR" || mensagem == "Brayan: ApagaR" || mensagem == "Enzo: ApagaR"){
    digitalWrite(LED, LOW);
  }else{
    Serial.println(mensagem);
  }
}