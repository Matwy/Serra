#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal.h>
#include <WiFi101.h>
#include <PubSubClient.h>

WiFiClient wifi;
int status = WL_IDLE_STATUS;
const int IDSerra = 0;

IPAddress server(192,168,1,213);
char topic[]= "serra/0";
PubSubClient client(wifi);

Adafruit_BME280 bme; // crea un oggetto BME280
const int rs = 7, en = 6, d4 = 4, d5 = 3, d6 = 1, d7 = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int volte = 0;
int lastButtonState = LOW;
int reading;
int buttonState;    
const int debounceDelay = 50;
unsigned long lastDebounceTime = 0;
int tempo = 0;

String humidityTerrain = "";
String temperature = "";
String humidityAir = "";
String pressure = "";
String altitude = "";

void setup() {
  Serial.begin(9600); // inizializza la console seriale
  Wire.begin(); // inizializza la libreria Wire.h per la comunicazione I2C
  bme.begin(0x76); // inizializza il sensore BME280 con indirizzo I2C 0x76
  lcd.begin(16, 2);
  pinMode(5, INPUT);
  // attachInterrupt(digitalPinToInterrupt(5),changeVolte,LOW);
  
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

void Temp(){
  
  // lcd.clear();
  temperature = bme.readTemperature();
  // lcd.setCursor(0, 0);
  // lcd.print("Temperatura:");
  // lcd.setCursor(0,1);
  // lcd.print(temperature);
  // lcd.print(" 'C");
}

void Humidity(){
  // lcd.clear();
  humidityAir = bme.readHumidity();
  // lcd.setCursor(0, 0);
  // lcd.print("Umidita':");
  // lcd.setCursor(0,1);
  // lcd.print(humidityAir);
  // lcd.print(" %");
}

void Pressure(){
  
  // lcd.clear();
  pressure = bme.readPressure() / 100.0F;
  // lcd.setCursor(0, 0);
  // lcd.print("Pressione:");
  // lcd.setCursor(0,1);
  // lcd.print(pressure);
  // lcd.print(" hPa");
}

void Altitude(){
  // lcd.clear();
  altitude = bme.readAltitude(1013.25);
  // lcd.setCursor(0, 0);
  // lcd.print("Altitudine:");
  // lcd.setCursor(0,1);
  // lcd.print(altitude);
  // lcd.print(" m");
}

void SoilHumidity(){
  lcd.clear();
  int msvalue = analogRead(A0);
  int mspercent = map(msvalue,0,1023,100,0);
  humidityTerrain = String(mspercent);
  lcd.setCursor(0, 0);
  lcd.print("Umidita'suolo:");
  lcd.setCursor(0,1);
  lcd.print(humidityTerrain + " %");
}

void LcdPrint(String name,String data,String unity){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(name);
  lcd.setCursor(0,1);
  lcd.print(data);
  lcd.print(unity);
}

void confrontaVolte(){
  Temp();
  Humidity();
  Pressure();
  Altitude();
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
      SoilHumidity();
    break;
    default:
      Serial.println("Numero di clic superiore a 4");
    break;
  }
}

// void changeVolte(){
//   Serial.println(digitalRead(5));
//   unsigned long currentMillis = millis();

//   if (currentMillis - lastDebounceTime > debounceDelay) {
//     lastDebounceTime = currentMillis;
//     Serial.print("bottone");
//     volte++;
//     if(volte>3){//cambia
//       volte=0;
//     }
//   }
// }

void loop() {
  confrontaVolte();
  String message = temperature + "," + humidityAir + "," + pressure + "," + altitude +","+ IDSerra;
  delay(1000);
  client.publish(topic,message.c_str());
  // delay(1000);
  // Serial.println("mandato");
  client.loop();
}