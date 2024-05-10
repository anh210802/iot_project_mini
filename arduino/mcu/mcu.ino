#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


#define LED 5
#define DHT_PIN 4

const int DHTTYPE = DHT22;

LiquidCrystal_I2C lcd(0x27, 20, 2);
DHT dht(DHT_PIN, DHTTYPE);
byte customChar[] = {
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

float temp = 0;
float humi = 0;
String data;
String received;
String value;
int counter = 0;


void setup() {
  pinMode(LED, OUTPUT);
  dht.begin();
  lcd.begin();
  lcd.backlight();
  lcd.print("Nhiet do: ");
  lcd.setCursor(0, 1);
  lcd.print("Do am: ");
  Serial.begin(115200);
  lcd.createChar(0, customChar);
  lcd.home();
  digitalWrite(LED, 0);
}

void sensorDHT(float &temp, float &humi) {
  temp = dht.readTemperature();
  humi = dht.readHumidity(); 
}

void setupLCD(float temp, float humi) {
  if (isnan(temp) || isnan(humi)) { 
    return;
  } 
  else {
    lcd.setCursor(10, 0);
    lcd.print(String(temp, 1)); 
    lcd.write(0);
    lcd.print("C");

    lcd.setCursor(10, 1);
    lcd.print(String(humi, 1)); 
    lcd.print(" %");    
  }
}

void uartTX(String &data, String &received, String &value) {
  if (Serial.available() > 0) {
    data = Serial.readStringUntil('#');
    int index = data.indexOf(':');
    received = data.substring(1, index);
    value = data.substring(index + 1, data.length());
    // Serial.println(received);
    // Serial.println(value);
    if (received == "LED") {
      if (value == "1") {
        digitalWrite(LED, 1);
      } else {
        digitalWrite(LED, 0);
      }
    }
  }
}

void uartRX(float &temp, float &humi) {
  Serial.print("!TEMP:");
  Serial.print(String(temp, 1));
  Serial.print(":HUMI:");
  Serial.print(String(humi, 1));
  Serial.print("#");
  Serial.println();
}

void loop() {
  counter += 1;
  if (counter == 5000){
    counter = 0;
    sensorDHT(temp, humi);
    setupLCD(temp, humi);
    uartRX(temp, humi);
  }
  uartTX(data, received, value);
  delay(1);
}
