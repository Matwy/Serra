#include <WiFi101.h>
#include <PubSubClient.h>

WiFiClient wifi;
int status = WL_IDLE_STATUS;

char server[] = "broker.hivemq.com";
char topic[]= "Serra5CIIgruppoMGA";
PubSubClient client(wifi);
void setup() {
  
  Serial.begin(9600);
  Serial.println("OK");
  Serial.print("Connessione..");
  while(status != WL_CONNECTED){
    status = WiFi.begin("TIM-27984525","nuZSJtuQZkf2JFHnZE6FkTjr");
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connsesso");

  client.setServer(server,1883);
  delay(5000);

  if(client.connect("arduinocon")){
    Serial.print("mqtt connected");
    client.subscribe(topic);
  }
  else{
    Serial.println("mqtt not cn");
    Serial.print(client.state());
  }
}


int IDSerra = 1;
void loop() {

  int umiditaTerreno = 35;
  float temperatura = 24.9;
  float umidita = 65.3;
  float pressione = 0.99;
  float altitudine = 70.5;
  String message = String(umiditaTerreno) + "," + String(temperatura) + "," + String(umidita)+ "," + String(pressione)+ "," + String(altitudine)+","+String(IDSerra);
  delay(1000);
  client.publish(topic,message.c_str());
  // delay(1000);
  // Serial.println("mandato");
  client.loop();
}