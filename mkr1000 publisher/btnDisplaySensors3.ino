#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal.h>
#include <WiFi101.h>
#include <PubSubClient.h>

WiFiClient wifi;
int status = WL_IDLE_STATUS;
const int IDSerra = 0;
unsigned long previousMesuramentMillis = 0;
unsigned long previousButtonChangeState = 0;
unsigned long inizio_acqua = 0;
const unsigned long interval = 1000;

IPAddress server(192,168,0,191); //192.168.0.191
int stato = 0;
//char server[] = "test.mosquitto.org";
char topic[]= "serra/0";
char topic_sub[] ="serra/acqua";

PubSubClient client(wifi);

Adafruit_BME280 bme; // crea un oggetto BME280
const int rs = 6, en = 5, d4 = 4, d5 = 3, d6 = 1, d7 = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String humidityTerrain = "";
String temperature = "";
String humidityAir = "";
String pressure = "";
String altitude = "";
int volte = 0;

void callback(char* topic_sub, byte* payload, unsigned int length) {
  // if(topic_sub){
  //   Serial.print("Dioboia");
  // }
  // Serial.print("Messaggio ricevuto su [");
  // Serial.print(topic_sub);
  // Serial.print("]: ");
  
  // Leggi il payload del messaggio
  inizio_acqua = millis();
  digitalWrite(7,HIGH);
  Serial.print("aqua");
}


void setup() {
  pinMode(7, OUTPUT);
  Serial.begin(9600); // inizializza la console seriale
  Wire.begin(); // inizializza la libreria Wire.h per la comunicazione I2C
  bme.begin(0x76); // inizializza il sensore BME280 con indirizzo I2C 0x76
  lcd.begin(16, 2);
  pinMode(8, INPUT_PULLUP);
  
  while(status != WL_CONNECTED){
    status = WiFi.begin("aldo","qwertyui");
    Serial.print(".");
    delay(1000);
  }

  //TIM-27984525,nuZSJtuQZkf2JFHnZE6FkTjr
  Serial.println("Connsesso");

  client.setServer(server,1883);
  client.setCallback(callback);
  delay(5000);

  if(client.connect("arduinocon")){
    Serial.print("mqtt connected");
    client.subscribe(topic_sub);
  }
  else{
    Serial.println("mqtt not cn");
    Serial.print(client.state());
  }

  mesure_all_sensor();
  change_display_state();
}

void Temp(){
  temperature = bme.readTemperature();
}

void Humidity(){
  humidityAir = bme.readHumidity();
}

void Pressure(){
  pressure = bme.readPressure() / 100.0F;
}

void Altitude(){
  altitude = bme.readAltitude(1013.25);
}

void SoilHumidity(){
  int msvalue = analogRead(A0);
  int mspercent = map(msvalue,0,1023,100,0);
  humidityTerrain = String(mspercent);
}

void LcdPrint(String name,String data,String unity){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(name);
  lcd.setCursor(0,1);
  lcd.print(data);
  lcd.print(unity);
}
void mesure_all_sensor(){
  Temp();
  Humidity();
  Pressure();
  Altitude();
  SoilHumidity();
}
void change_display_state(){
  switch(volte){
    case 0: 
      LcdPrint("Temperatura:",temperature," 'C");
      break;
    case 1:
      LcdPrint("Umidita'",humidityAir," %");
      break;
    case 2:
      LcdPrint("Pressione:",pressure," hPa");
      break;
    case 3:
      LcdPrint("Altitudine:",altitude," m");
    break;
    case 4:
      LcdPrint("Umidita'suolo:",humidityTerrain," %");
    break;
    default:
      Serial.println("Numero di clic superiore a 4");
    break;
  }
}

void loop() {
  if (millis() - previousMesuramentMillis >= interval) {
    previousMesuramentMillis = millis();
    mesure_all_sensor();
    change_display_state();
    String message = temperature + "," + humidityAir + "," + pressure + "," + altitude +","+humidityTerrain+","+IDSerra;
    client.publish(topic,message.c_str());
    client.loop();
  }
  if(!digitalRead(8) && millis() - previousButtonChangeState > 500){
    previousButtonChangeState = millis();
    volte++;
    if(volte > 4){
      volte = 0;
    }
    change_display_state();
  }
  if(millis() - inizio_acqua > 10000){
    digitalWrite(7,LOW);
  }
}