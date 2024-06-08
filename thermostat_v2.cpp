#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN1 2           // DHT22 sensörünün DATA pinini Arduino'nun dijital pin 2'sine bağladık
#define DHTPIN2 3           // DHT22 sensörünün DATA pinini Arduino'nun dijital pin 3'üne bağladık
#define DHTTYPE DHT22       // Sensör türü tanımlanır
DHT dht1(DHTPIN1, DHTTYPE); // DHT kütüphanesi nesnesini oluşturuyoruz
DHT dht2(DHTPIN2, DHTTYPE); // DHT kütüphanesi nesnesini oluşturuyoruz

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 20
#define LCD_ROWS 4
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

#define RED_PIN 10    // Kırmızı LED pinini tanımlandı
#define BLUE_PIN 9    // Mavi LED pinini tanımlandı
#define YELLOW_PIN 8  // Sarı LED pinini tanımlandı

void setup() {
  Serial.begin(9600);
  dht1.begin();              
  dht2.begin();              
  
  lcd.init();               // LCD ekranın başlatılması
  lcd.backlight();          // LCD ekranın arka ışığını açma

  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);

  digitalWrite(RED_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
}

void loop() {
  delay(5000); // 1 saniye aralıklarla sıcaklık ve nem ölçümü yapılır
  
  float Temperature1 = dht1.readTemperature();
  float Humidity1 = dht1.readHumidity();
  float Temperature2 = dht2.readTemperature();
  float Humidity2 = dht2.readHumidity();
  
  if (isnan(Temperature1) || isnan(Humidity1) || isnan(Temperature2) || isnan(Humidity2)) {  // Veri türünün geçerliliğini kontrol eder
    Serial.println(F("Sensör verileri okunamadi!"));
    lcd.setCursor(0, 0); 
    lcd.print("Veri okunamadi");
    return;
  }
  
  Serial.print("1. Sensör Sicaklik: ");
  Serial.print(Temperature1);
  Serial.print(" C   Nem: ");
  Serial.print(Humidity1);
  Serial.println(" %");
    
  lcd.clear();
  
  if (Temperature1 > 60 || Temperature1 < 0) { // Anormal sıcaklık kontrolü
    lcd.setCursor(0, 0);
    lcd.print("Anormal sicaklik!");

    Serial.print("2. Sensör Sicaklik: ");
    Serial.print(Temperature2);
    Serial.print(" C   Nem: ");
    Serial.print(Humidity2);
    Serial.println(" %");
    
    lcd.setCursor(0, 1);
    lcd.print("2. Sensor:");
    
    lcd.setCursor(0, 2); 
    lcd.print("Sicaklik: ");
    lcd.print(Temperature2);
    lcd.print(" C");

    lcd.setCursor(0, 3); 
    lcd.print("Nem: ");
    lcd.print(Humidity2);
    lcd.print(" %");
  } 
  else {
    lcd.setCursor(0, 0); 
    lcd.print("Sicaklik: ");
    lcd.print(Temperature1);
    lcd.print(" C");

    lcd.setCursor(0, 1); 
    lcd.print("Nem: ");
    lcd.print(Humidity1);
    lcd.print(" %");

    if (Temperature1 > 20) {
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(BLUE_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
      lcd.setCursor(0, 2);
      lcd.print("Yuksek sicaklik");
    } 
    else if (Temperature1 >= 10 && Temperature1 <= 20) {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
      digitalWrite(YELLOW_PIN, HIGH);
      lcd.setCursor(0, 2);
      lcd.print("Normal sicaklik");
    } 
    else {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(BLUE_PIN, HIGH);
      digitalWrite(YELLOW_PIN, LOW);
      lcd.setCursor(0, 2); 
      lcd.print("Dusuk sicaklik");
    }
  }
}
