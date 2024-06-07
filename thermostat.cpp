#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2           // DHT22 sensörünün DATA pinini Arduino'nun dijital pin 2'sine bağladık
#define DHTTYPE DHT22      
DHT dht(DHTPIN, DHTTYPE);  // DHT kütüphanesi nesnesini oluşturuyoruz

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 20
#define LCD_ROWS 4
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

#define RED_PIN 10    // Kırmızı LED pinini tanımlandı
#define BLUE_PIN 9    // Mavi LED pinini  tanımlandı
#define YELLOW_PIN 8  // Sarı LED pinini  tanımlandı

void setup() {
  Serial.begin(2000);
  dht.begin();
  
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
  delay(5000); // 5 saniye aralıklarla sıcaklık ve nem ölçümü yapılır
  
  float Temperature = dht.readTemperature();
  float Humidity = dht.readHumidity();
  
  if (isnan(Temperature) || isnan(Humidity)) {
    Serial.println(F("Sensör verileri okunamadi!"));
    lcd.setCursor(0, 0); 
    lcd.print("Veri okunamadi");
    return;
  }
  
  Serial.print("Okunan Sicaklik: ");
  Serial.print(Temperature);
  Serial.print(" C   Nem: ");
  Serial.print(Humidity);
  Serial.println(" %");
    
  lcd.clear();         
  
  lcd.setCursor(0, 0); 
  lcd.print("Sicaklik: ");
  lcd.print(Temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1); 
  lcd.print("Nem: ");
  lcd.print(Humidity);
  lcd.print(" %");

  if (Temperature > 20) {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    lcd.setCursor(0, 2);
    lcd.print("Yuksek sicaklik");
  } else if (Temperature >= 10 && Temperature <= 20) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(YELLOW_PIN, HIGH);
    lcd.setCursor(0, 2);
    lcd.print("Normal sicaklik");
  } else {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(YELLOW_PIN, LOW);
    lcd.setCursor(0, 2); 
    lcd.print("Dusuk sicaklik");
  }
}
